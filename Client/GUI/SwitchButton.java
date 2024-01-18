package GUI;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import javax.swing.JToggleButton;

import Model.CameraControlModel;
import Model.SettingsModel;

public class SwitchButton extends JPanel implements ActionListener {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private boolean isModeOn = true;
	private JToggleButton offOption;
	private JToggleButton onOption;
	
	private SettingsModel model;
	private CameraControlModel cameraModel;

	public SwitchButton(String left, String right, SettingsModel model) {
		this.model = model;
		setLayout(new FlowLayout());
		onOption = new JToggleButton(right);
		offOption = new JToggleButton(left);
		
		offOption.setSelected(true);
		
		onOption.addActionListener(this);
		offOption.addActionListener(this);
		
		add(offOption, BorderLayout.WEST);
		add(onOption, BorderLayout.EAST);
	}
	
	public SwitchButton(String left, String right, CameraControlModel model) {
		cameraModel = model;
		setLayout(new FlowLayout());
		onOption = new JToggleButton(right);
		offOption = new JToggleButton(left);
		
		offOption.setSelected(true);
		
		onOption.addActionListener(this);
		offOption.addActionListener(this);
		
		add(offOption, BorderLayout.WEST);
		add(onOption, BorderLayout.EAST);
	}
	
	public boolean getMode() {
		return isModeOn;
	}
	
	public void setActiveMode(boolean mode) {
		onOption.setSelected(mode);
		offOption.setSelected(!mode);
		isModeOn = mode;
	}
	
	private void setOn() {
		onOption.setSelected(true);
		offOption.setSelected(false);
	}
	
	private void setOff() {
		onOption.setSelected(false);
		offOption.setSelected(true);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		
		switch (e.getActionCommand()) {
		case "Movie":
			cameraModel.setMovieMode(true);
			setOn();
			break;
		case "Idle":
			cameraModel.setMovieMode(false);
			setOff();
			break;
		case "Synchronous":
			model.setSyncMode(true);
			setOn();
			break;
		case "Asynchronous":
			model.setSyncMode(false);
			setOff();
			break;
		default:
			break;
		}
	}
}
