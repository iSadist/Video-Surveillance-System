package GUI;

import javax.swing.JSplitPane;

public class SplitView extends JSplitPane {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	public SplitView(CameraControlView rightComponent, CameraControlView leftComponent) {
		super(JSplitPane.HORIZONTAL_SPLIT, true, rightComponent, leftComponent);
	}
}
