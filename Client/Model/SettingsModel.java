package Model;

import java.util.Observable;

public class SettingsModel extends Observable{

	private boolean syncModeOn;
	private boolean autoActivated;
	
	public SettingsModel(boolean sync) {
		autoActivated = false;
	}
	
	public boolean isSyncModeOn() {
		return syncModeOn;
	}
	
	public void setSyncMode(boolean sync) {
		syncModeOn = sync;
		update();
	}
	
	public boolean isAuto() {
		return autoActivated;
	}
	
	public void setAutoMode(boolean mode) {
		autoActivated = mode;
		update();
	}
	
	private void update() {
		setChanged();
		notifyObservers();
		System.out.println("Set changed in settings model");
	}	
}
