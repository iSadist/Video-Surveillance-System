package Control;

import java.io.IOException;
import java.net.*;
import java.util.Date;
import java.io.*;

public class ServerReader extends Thread{
	private Monitor monitor;
	private String url;
	private int portNbr;
	private int serverNumber;
	private Socket cameraSocket;
	private boolean closeThread;
	private InputStream iStream;
	byte mode;
	boolean motion;
	
	private ServerFrame receivedFrame;
	
	private static final byte[] START_FLAG = {
		'S', 'T', 'A', 'R', 'T', 'F', 'R', 'A', 'M', 'E'};
	
	public ServerReader(Monitor m, Socket socket, int serverNumber)
			throws UnknownHostException, IOException {
		this.monitor = m;
		this.cameraSocket = socket;
		this.serverNumber = serverNumber;
		
		closeThread = false;
		receivedFrame = null;
		mode = 0;
		motion = false;
		
		// Throws UnknownHostException or IOException if connection fails
		iStream = cameraSocket.getInputStream();
	}
	
	public void read() throws IOException {
		// ================================= //
		// Header protocol:_________________ //
		//   Timestamp  		- 8 bytes    //
		//   Image size 		- 4 bytes    //
		//   Mode of operation 	- 1 byte     //
		//   Image size                      //
		// ================================= //
		long timestamp;
		int imageSize;
		byte newMode;
		byte[] image;
		
		int TIMESTAMP_SIZE = 8;
		int IMAGE_SIZE_SIZE = 4;
		int IMAGE_SIZE_INDEX = TIMESTAMP_SIZE;
		int MODE_OF_OP_SIZE = 1;
		int MODE_OF_OP_INDEX = IMAGE_SIZE_INDEX + IMAGE_SIZE_SIZE;
		int HEADER_SIZE = TIMESTAMP_SIZE + IMAGE_SIZE_SIZE + MODE_OF_OP_SIZE;
		byte[] header = new byte[HEADER_SIZE];
		int headerIndex = 0;
		
		//
		// Await start flag
		//
		boolean startFlagReceived = false;
		int startFlagIndex = 0;
		int startFlagLen = START_FLAG.length;
		
		while (!startFlagReceived) {
			
			byte received = (byte)iStream.read();
			
			if (received == START_FLAG[startFlagIndex]) {
				startFlagIndex += 1;
			} else {
				startFlagIndex = 0;
			}
			
			startFlagReceived = (startFlagLen == startFlagIndex);
		}
		
		//
		// Receive header
		//
		boolean headerReceived = false;
		
		while (!headerReceived && !closeThread) {
			header[headerIndex++] = (byte)iStream.read();
			headerReceived = (headerIndex == HEADER_SIZE);					
		}
		
		//
		// Parse header
		//
		/*System.out.println("Header:");
		System.out.print("\t");
		for (int i = 0; i != HEADER_SIZE; i++) {
			System.out.print(Integer.toHexString((header[i] & 0xFF)) + " ");
		}
		System.out.print("\n");*/
		
		timestamp = 0;
		for (int i = 0; i != TIMESTAMP_SIZE; i++) {
			timestamp |= (((long)header[i] & (long)0xFF) << (TIMESTAMP_SIZE - 1 - i) * 8) &
					((long)0xFF << (TIMESTAMP_SIZE - 1 - i) * 8);
		}
		
		imageSize = 0;
		for (int i = IMAGE_SIZE_INDEX; i != IMAGE_SIZE_INDEX + IMAGE_SIZE_SIZE; i++) {
			imageSize |= (((int)header[i] & 0xFF) << (IMAGE_SIZE_SIZE - 1 - i) * 8) & 
					(0xFF << (IMAGE_SIZE_SIZE - 1 - i) * 8);
		}
		
		newMode = header[MODE_OF_OP_INDEX];
		boolean motionDetected = (0x02 == (newMode & 0x02));
		
		if (motionDetected && (motionDetected != motion)) {
			mode = newMode;
			motion = true;
			Mode m = Mode.MOVIE;
			
			monitor.changeMode(serverNumber, m);
		} else if (motion && (0 == (newMode & 0x01))) {
			motion = false;
		}
		
		if (timestamp < 0) {
			System.out.println("\tThrowing frame due to invalid timestamp.");
			return;
		} else if (imageSize > 100000000) {
			System.out.println("\tThrowing frame due to invalid image size.");
			return;
		}
		
		//
		// Receive image
		//
		image = new byte[imageSize];
		int imageIndex = 0;
		boolean imageReceived = false;
		long checksum = 0;
		
		while (!imageReceived && !closeThread) {
			while ((iStream.available() == 0) && !closeThread) {
				try {
					this.sleep(10);
				} catch (InterruptedException e) {
					closeThread = true;
				}
			}
			
			image[imageIndex] = (byte)iStream.read();
			checksum += (long)image[imageIndex];
			imageReceived = (++imageIndex == imageSize);					
		}
		
		//
		// Create frame
		//
		if (headerReceived && imageReceived && !closeThread) {
			Date sentTime = new Date(timestamp);
			receivedFrame = new ServerFrame(image, sentTime, serverNumber);
		}
	}
	
	public void run() {
		System.out.println("ServerReader " + serverNumber + " opened.");
		
		while (!closeThread)
		{
			try {
				read();
			} catch (IOException e) {
				closeThread = true;
				e.printStackTrace();
			}
			
			if (null != receivedFrame) {
				monitor.addFrame(receivedFrame);
				receivedFrame = null;
			}
		}
		
		System.out.println("ServerReader closed with: " + url + ", " +
				portNbr + ", " + serverNumber);
	}
	
}
