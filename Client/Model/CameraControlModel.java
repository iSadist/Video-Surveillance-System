package Model;

import java.util.Observable;

import javax.swing.ImageIcon;

public class CameraControlModel extends Observable {

	private ImageIcon image;
	private int delayTime;
	private boolean movieModeOn;
	private int cameraNumber;
	private boolean motionDetected;
	
	public CameraControlModel(ImageIcon icon, int time, boolean sync, boolean movie, int number) {
		image = icon;
		delayTime = time;
		movieModeOn = movie;
		cameraNumber = number;
		motionDetected = false;
	}
	
	public int getID() {
		return cameraNumber;
	}
	
	public ImageIcon getImage() {
		return image;
	}
	
	public void setNewImage(ImageIcon image) {
		this.image = image;
		update();
	}
	
	public int getDelay() {
		return delayTime;
	}
	
	public void setDelayTime(int time) {
		delayTime = time;
		update();
	}
	
	public boolean isMovieModeOn() {
		return movieModeOn;
	}
	
	public void setMovieMode(boolean movie) {
		movieModeOn = movie;
		update();
	}
	
	public void setMotionDetected(boolean mode) {
		motionDetected = mode;
		update();
	}
	
	public boolean motionDetected() {
		return motionDetected;
	}
	
	private void update() {
		setChanged();
		notifyObservers();
		//System.out.println("Set changed in camera model");
	}
	
}
