package GUI;

import java.awt.Color;
import java.util.Observable;
import java.util.Observer;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

import Model.CameraControlModel;

public class CameraControlView extends JPanel implements Observer{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JLabel imageLabel;
	private JLabel motionDetected;
	private DelayLabel delay;
	private SwitchButton movieModeButton;

	public CameraControlView(String title, CameraControlModel model) {
		setLayout(new BoxLayout(this, BoxLayout.PAGE_AXIS));
		
		model.addObserver(this);
		
		JLabel titleLabel = new JLabel(title);
		titleLabel.setOpaque(true);
		titleLabel.setBackground(Color.WHITE);
		titleLabel.setHorizontalAlignment(JLabel.CENTER);
		
		imageLabel = new JLabel();
		motionDetected = new JLabel();
		delay = new DelayLabel();
		
		motionDetected.setText("Motion detected");
		motionDetected.setForeground(Color.RED);
		
		movieModeButton = new SwitchButton("Idle", "Movie", model);
		
		add(titleLabel);
		add(imageLabel);
		add(motionDetected);
		add(delay);
		add(movieModeButton);
	}

	@Override
	public void update(Observable o, Object arg) {
		
		CameraControlModel info = (CameraControlModel) o;
		
		if (info != null) {
			imageLabel.setIcon(info.getImage());
			delay.setTime(info.getDelay());
			movieModeButton.setActiveMode(info.isMovieModeOn());
			if (info.motionDetected()) {
				motionDetected.setForeground(Color.GREEN);
			} else {
				motionDetected.setForeground(Color.RED);
			}
			repaint();			
		}	
	}
	
}
