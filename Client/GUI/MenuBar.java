package GUI;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

public class MenuBar extends JMenuBar implements ActionListener{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public MenuBar() {
		JMenu fileMenu = new JMenu("File");
		JMenuItem closeItem = new JMenuItem("Close");
		closeItem.addActionListener(this);
		
		fileMenu.add(closeItem);
		add(fileMenu);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		System.exit(1);
	}
	
}
