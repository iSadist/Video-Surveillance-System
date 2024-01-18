package GUI;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JToggleButton;

import Model.SettingsModel;

public class AutoButton extends JToggleButton implements ActionListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private boolean autoActivated;
	private SettingsModel model;
	
	public AutoButton(SettingsModel model) {
		super("Auto");
		autoActivated = false;
		addActionListener(this);
		this.model = model;
	}

	public boolean isSetToAuto() {
		return autoActivated;
	}
	
	public void setActivate(boolean on) {
		autoActivated = on;
		setSelected(on);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		autoActivated = !autoActivated;
		model.setAutoMode(autoActivated);
	}
}
