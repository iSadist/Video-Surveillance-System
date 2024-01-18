package Control;

import java.net.*;
import java.io.*;

public class ServerWriter extends Thread {
	static final char MODE_IDLE  = 0x30;
	static final char MODE_MOVIE = 0x31;
	static final char MODE_ACK   = 0x32;
	
	
	private Monitor monitor;
	private boolean closeThread;
	private Socket cameraSocket;
	private OutputStream oStream;
	private int cameraNbr;
	
	public ServerWriter(Monitor m, Socket socket, int camera)throws
			UnknownHostException, IOException {
		this.monitor = m;
		this.cameraSocket = socket;
		this.cameraNbr = camera;
		
		oStream = cameraSocket.getOutputStream();
		
		closeThread = false;
	}
	
	public void modeRequest() {
		//TODO What does this method do?
	}
	
	public void run() {
		Mode nextMode;
		
		System.out.println("ServerWriter " + cameraNbr + "opened.");
		
		while (!closeThread)
		{
			try {
				nextMode = monitor.nextMode(cameraNbr);
				
				System.out.println("ServerWriter " + cameraNbr + " mode change");

				if (Mode.IDLE == nextMode) {
					oStream.write(MODE_IDLE);
				} else if (Mode.MOVIE == nextMode) {
					oStream.write(MODE_MOVIE);
				}
				
				System.out.println("ServerWriter " + cameraNbr + " mode written");
			} catch (IOException e) {
				closeThread = true;
				e.printStackTrace();
			} catch (InterruptedException e) {
				closeThread = true;
			}
		}
		
		try {
			cameraSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		System.out.println("ServerWriter " + cameraNbr + " closed");
	}
}
