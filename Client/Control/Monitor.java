package Control;

import java.util.Observable;
import java.util.Observer;

import javax.swing.ImageIcon;

import Model.CameraControlModel;
import Model.SettingsModel;

public class Monitor implements Observer{
	
	protected static final int SERVER_STATE_ONE = 1;
	protected static final int SERVER_STATE_TWO = 2;
	
	protected static final boolean ASYNCHRONOUS_MODE = false;
	protected static final boolean SYNCHRONOUS_MODE = true;
	
	private static final long SYNC_TIME_MS = 500;
	private static final double SYNC_TIME_HYSTERESIS_MS = 50;

	private volatile long movingAverageServerOne;
	private volatile long movingAverageServerTwo;
	private long currentSyncTimeMs;
	private static final double FILTER_CONSTANT = 0.8;
	
	private FrameBuffer frames;
	private volatile Mode currentCameraOneMode;
	private volatile Mode currentCameraTwoMode;
	private boolean currentState;
	private boolean syncAuto;

	private CameraControlModel camera1;
	private CameraControlModel camera2;
	private SettingsModel settings;
	
	public Monitor(CameraControlModel camera1, CameraControlModel camera2, SettingsModel settings){
		currentState = ASYNCHRONOUS_MODE;
		currentCameraOneMode = Mode.IDLE;
		currentCameraTwoMode = Mode.IDLE;
		this.camera1 = camera1;
		this.camera2 = camera2;
		this.settings = settings;
		
		movingAverageServerOne = 0;
		movingAverageServerTwo = 0;
		currentSyncTimeMs = SYNC_TIME_MS;
		
		settings.addObserver(this);
		camera1.addObserver(this);
		camera2.addObserver(this);
		
		syncAuto = false;
		frames = new FrameBuffer();
	}
	
	/**
	 * Updates the camera images in the GUI.
	 * @throws InterruptedException 
	 */
	synchronized void update() throws InterruptedException {
		ServerFrame frameToShow = null;
		
		while (0 == frames.size()) {
			wait();
		}
		
		/*
		 * If in ASYNCHRONOUS_MODE, display images as soon as they are put
		 * in the FrameBuffer.
		 */
		if (currentState == ASYNCHRONOUS_MODE) {
			frameToShow = frames.pop();
			
			if (syncAuto &&
					(Math.abs(movingAverageServerOne - movingAverageServerTwo) <
					(SYNC_TIME_MS - SYNC_TIME_HYSTERESIS_MS))) {
				currentState = SYNCHRONOUS_MODE;
				settings.setSyncMode(true);
				System.out.println("Changing to sync");
			}		
			
		/*
		 * If in SYNCHRONOUS_MODE, make sure that the time delay between
		 * when the image was taken and shown remains constant for all frames.
		 */
		} else if (currentState == SYNCHRONOUS_MODE) {
			long currentTime;
			long sentTime;
			long nextDispTime;
			
			if ((currentSyncTimeMs * 2) < Math.max(movingAverageServerOne, movingAverageServerOne))
			{
				currentSyncTimeMs = 4 * currentSyncTimeMs;
				System.out.println("Sync time increased to " + currentSyncTimeMs + "ms");
			}
			
			currentTime = getTime();			
			sentTime = frames.peek().getSentTime().getTime();
			nextDispTime = sentTime + currentSyncTimeMs;

			if (nextDispTime > currentTime) {
				
				currentTime = getTime();
				while (nextDispTime > currentTime) {
					
					wait(nextDispTime - currentTime);
					
					// Update display time in case a new image has been added.
					sentTime = frames.peek().getSentTime().getTime();
					nextDispTime = sentTime + currentSyncTimeMs;
					currentTime = getTime();
				}
				
				currentTime = getTime();
			}
			
			if (syncAuto &&
					(Math.abs(movingAverageServerOne - movingAverageServerTwo) >
					(SYNC_TIME_MS + SYNC_TIME_HYSTERESIS_MS))) {
				currentState = ASYNCHRONOUS_MODE;
				settings.setSyncMode(false);
				System.out.println("Changing to async");
			}
			
			frameToShow = frames.pop();
			
		}

		ImageIcon img = new ImageIcon(frameToShow.getImage());

		if (1 == frameToShow.getServer()) {
			camera1.setNewImage(img);
			camera1.setDelayTime((int)(frameToShow.getReceivedTime().getTime() -
					frameToShow.getSentTime().getTime()));
			
		} else if (2 == frameToShow.getServer()){
			camera2.setNewImage(img);
			camera2.setDelayTime((int)(frameToShow.getReceivedTime().getTime() -
					frameToShow.getSentTime().getTime()));
		}
	}
	
	private long getTime() {
		return System.currentTimeMillis();
	}
	
	/**
	 * This function is called from a ServerWriter thread.
	 * 
	 * Blocks the calling thread until a request for a new mode of operation
	 * has been received.
	 * 
	 * @return next mode of operation.
	 * @throws InterruptedException 
	 */
	synchronized Mode nextMode(int camera) throws InterruptedException {
		Mode next = Mode.IDLE;
		
		if (1 == camera) {
			Mode oldMode = currentCameraOneMode;
			
			while (currentCameraOneMode == oldMode) {
					wait();
			}
			
			next = currentCameraOneMode;
		} else if (2 == camera) {
			Mode oldMode = currentCameraTwoMode;
			
			while (currentCameraTwoMode == oldMode) {
					wait();
			}
			
			next = currentCameraTwoMode;
		}
	
		return next;
	}
	
	/**
	 * Changes the mode of operation.
	 * @param server
	 * @param mode the new mode to change to.
	 */
	synchronized void changeMode(int server, Mode mode) {
		if (mode == Mode.MOVIE) {
			if (1 == server) {
				camera1.setMotionDetected(true);
				camera2.setMotionDetected(false);
				
				if (!camera1.isMovieModeOn()) {
					camera1.setMovieMode(true);
				}
				if (!camera2.isMovieModeOn()) {
					camera2.setMovieMode(true);
				}
				
			} else if (2 == server) {
				camera1.setMotionDetected(false);
				camera2.setMotionDetected(true);
				
				if (!camera1.isMovieModeOn()) {
					camera1.setMovieMode(true);
				}
				if (!camera2.isMovieModeOn()) {
					camera2.setMovieMode(true);
				}
			}
			
			currentCameraOneMode = mode;
			currentCameraTwoMode = mode;
		} else {
			if (1 == server) {
				currentCameraOneMode = mode;
				camera1.setMotionDetected(false);
				
				if (camera1.isMovieModeOn()) {
					camera1.setMovieMode(false);
				}
			} else if (2 == server){
				currentCameraTwoMode = mode;
				camera2.setMotionDetected(false);
				
				if (camera2.isMovieModeOn()) {
					camera2.setMovieMode(false);
				}
			}
		}
		
		notifyAll();
	}

	synchronized void addFrame(ServerFrame frame) {
		frames.add(frame);
		long delay = frame.getReceivedTime().getTime() - frame.getSentTime().getTime();
		
		if (1 == frame.getServer()) {
			movingAverageServerOne = (long)(movingAverageServerOne * FILTER_CONSTANT +
					delay * (1.0 - FILTER_CONSTANT));
		} else if (2 == frame.getServer()) {
			movingAverageServerTwo = (long)(movingAverageServerTwo * FILTER_CONSTANT +
					delay * (1.0 - FILTER_CONSTANT));
		}
		
		notifyAll();
	}
	
	synchronized void updateGui(Observable o, Object arg) {
		if (o instanceof CameraControlModel) {
			CameraControlModel m = (CameraControlModel)o;
			
			switch (m.getID()) {
			case 1:
				if (m.isMovieModeOn() && (currentCameraOneMode != Mode.MOVIE)) {
					currentCameraOneMode = Mode.MOVIE;
					notifyAll();
				} else if (!m.isMovieModeOn() && (currentCameraOneMode != Mode.IDLE)) {
					if (m.motionDetected()) {
						m.setMotionDetected(false);
					}
					currentCameraOneMode  = Mode.IDLE;
					notifyAll();
				}
				break;
				
			case 2:
				if (m.isMovieModeOn() && (currentCameraTwoMode != Mode.MOVIE)) {
					currentCameraTwoMode = Mode.MOVIE;
					notifyAll();
				} else if (!m.isMovieModeOn() && (currentCameraTwoMode != Mode.IDLE)) {
					if (m.motionDetected()) {
						m.setMotionDetected(false);
					}
					currentCameraTwoMode = Mode.IDLE;
					notifyAll();
				}
				break;
			}
		} else if (o instanceof SettingsModel) {
			SettingsModel m = (SettingsModel)o;
			
			if (m.isSyncModeOn() != currentState) {
				currentState = m.isSyncModeOn();
				
				if (m.isAuto()) {
					m.setAutoMode(false);
				}
			} else if (m.isAuto() != syncAuto) {
				syncAuto = m.isAuto();
			}
		}
	}

	@Override
	public void update(Observable o, Object arg) {
		updateGui(o, arg);
	}
}
