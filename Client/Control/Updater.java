package Control;

public class Updater extends Thread{

	private Monitor monitor;
	
	private boolean closeThread;
	
	public Updater(Monitor monitor) {
		this.monitor = monitor;
	}
	
	public void run() {
		System.out.println("Updater running");
		
		while (!closeThread) {
			try {
				monitor.update();
			} catch (InterruptedException e) {
				closeThread = true;
			}
		}
		
		System.out.println("Updater closed");
	}
	
}
