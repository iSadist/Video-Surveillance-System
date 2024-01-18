package GUI;

import java.awt.FlowLayout;
import java.io.IOException;
import java.util.Observable;
import java.util.Observer;
import java.net.*;
import java.io.*;

import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;

import Control.*;
import Model.CameraControlModel;
import Model.SettingsModel;


public class Window extends JFrame implements Observer{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private MenuBar menu;
	private CameraControlView leftView;
	private CameraControlView rightView;
	private SettingsModel settings;
	
	private AutoButton auto;
	private SwitchButton syncModeButton;
	
	public Window(int width, int height) {
		super.setSize(width, height);
	}
	
	public void setup(CameraControlModel left, CameraControlModel right, SettingsModel settingsModel) {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		JPanel cameraPanel = new JPanel();
		cameraPanel.setLayout(new BoxLayout(cameraPanel, BoxLayout.X_AXIS));
		JPanel settingsPanel = new JPanel();
		settingsPanel.setLayout(new BoxLayout(settingsPanel, BoxLayout.LINE_AXIS));
		
		leftView = new CameraControlView("Camera 1",left);
		rightView = new CameraControlView("Camera 2",right);
		
		settings = settingsModel;
		settings.addObserver(this);
		
		menu = new MenuBar();
		setJMenuBar(menu);
		
		SplitView splitView = new SplitView(rightView, leftView);
		splitView.setDividerLocation(this.getWidth()/2);
		
		syncModeButton = new SwitchButton("Asynchronous", "Synchronous", settingsModel);
		auto = new AutoButton(settingsModel);
		
		
		cameraPanel.add(splitView);
		settingsPanel.add(auto);
		settingsPanel.add(syncModeButton);
		mainPanel.add(cameraPanel);
		mainPanel.add(settingsPanel);
		add(mainPanel);
		
		this.setVisible(true);
	}
	
	public static void main(String args[]) {
		
		CameraControlModel camera1 = new CameraControlModel(null, 0, false, false,1);
		CameraControlModel camera2 = new CameraControlModel(null, 0, false, false,2);
		
		SettingsModel settingsModel = new SettingsModel(false);
		
		Window gui = new Window(1600, 800);
		Monitor mon = new Monitor(camera1,camera2, settingsModel);
		
		int cameraOnePort = 9090;
		int cameraTwoPort = 9090;
		
		ServerReader cameraOneReader = null;
		ServerWriter cameraOneWriter = null;
		boolean cameraOneOk = false;
		
		ServerReader cameraTwoReader = null;
		ServerWriter cameraTwoWriter = null;
		boolean cameraTwoOk = false;
		
		if (args.length == 1) {
			try {
				Socket socketOne = new Socket(args[0], cameraOnePort);
				cameraOneReader = new ServerReader(mon, socketOne, 1);
				cameraOneWriter = new ServerWriter(mon, socketOne, 1);
				cameraOneOk = true;
			} catch (IOException e) {
				e.printStackTrace();
			}
		} else if (args.length == 2) {
			try {
				Socket socketOne = new Socket(args[0], cameraOnePort);
				Socket socketTwo = new Socket(args[1], cameraTwoPort);
				cameraOneReader = new ServerReader(mon, socketOne, 1);
				cameraOneWriter = new ServerWriter(mon, socketOne, 1);
				cameraOneOk = true;
				
				cameraTwoReader = new ServerReader(mon, socketTwo, 2);
				cameraTwoWriter = new ServerWriter(mon, socketTwo, 2);
				cameraTwoOk = true;
			} catch (IOException e) {
				e.printStackTrace();
			}
		} else {
			System.out.println("No valid camera urls were entered as command line arguments.");
			System.out.println("No cameras connected.");
			return;
		}
		Updater updaterThread = new Updater(mon);
		
		gui.setup(camera1, camera2, settingsModel);
		
		/*ImageIcon image = new ImageIcon("image/Cool_Face.jpg");
		camera1.setNewImage(image);
		camera2.setNewImage(image);*/
		
		if (cameraOneOk) {
			cameraOneReader.start();
			cameraOneWriter.start();
		}
		
		if (cameraTwoOk) {
			cameraTwoReader.start();
			cameraTwoWriter.start();
		}
		
		updaterThread.start();
	}

	@Override
	public void update(Observable o, Object arg) {
		SettingsModel info = (SettingsModel) o;
		
		if (info != null) {
			syncModeButton.setActiveMode(info.isSyncModeOn());
			auto.setActivate(info.isAuto());
		}
	}
	
}