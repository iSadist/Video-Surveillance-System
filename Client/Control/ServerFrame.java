package Control;

import java.util.Date;

public class ServerFrame {

	private Date sent;
	private Date received;
	private byte[] image;
	private int server;
	
	public ServerFrame(byte[] image, Date sent, int server) {
		this.image = image;
		this.sent = sent;
		this.server = server;
		received = new Date();
	}
	
	public byte[] getImage() {
		return image;
	}
	
	public Date getReceivedTime() {
		return received;
	}
	
	public Date getSentTime() {
		return sent;
	}
	
	public int getServer() {
		return server;
	}
}
