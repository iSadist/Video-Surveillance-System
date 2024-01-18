package GUI;

import javax.swing.JLabel;

public class DelayLabel extends JLabel {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public DelayLabel() {
		this.setText("Current delay: 0");
	}
	
	public void setTime(int delayTime) {
		setText("Current delay: " + delayTime);
	}
}
