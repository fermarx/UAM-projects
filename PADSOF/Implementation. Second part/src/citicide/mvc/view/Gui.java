package citicide.mvc.view;

import javax.swing.*;

import citicide.MySystem;
import citicide.mvc.controller.Controller;
import citicide.mvc.view.administrator.AdministratorPage;

import java.awt.*;
import java.awt.event.*;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
* This is the Gui class from the citicide.mvc.view package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class Gui extends JFrame {

	private static final long serialVersionUID = 1L;
	private LogInPage logInPage;
	private LogUpPage logUpPage;
	private InitialPage initialPage;
	private GeneralPage generalPage;
	private AdministratorPage administratorPage;
	private Controller controller;
	private JPanel north;
	private JPanel south;
	private WindowAdapter windowAdapter;
	private Dimension screenSize;
	private Color colorLogo = new Color(93,188,210);
	private Color colorLogoLight = new Color(131,217,239);
	private Color colorLogoDark = new Color(61,154,178);
	
	/**
	* This is the constructor method of the gui class
	*  
	* @param title of the gui
	*/
	public Gui(String title) {
		super(title);
		
		Container container = this.getContentPane(); 
		
		// Adjust to your screen resolution 
		screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		setBounds(0,0,screenSize.width, screenSize.height);
		
		container.setLayout(new BorderLayout());
		
		// Add the logo add the right side
		this.initialPage = new InitialPage();
		container.add(initialPage, BorderLayout.CENTER);
		
		// Set the north part
		this.north = new JPanel(new FlowLayout() );
		this.north.add(new JLabel("This is the Citicide Application. Enhance the world with us"));
		this.north.setBackground(getColorLogo());
		container.add(this.north, BorderLayout.NORTH);
		
		// Set the south part
		this.south = new JPanel(new FlowLayout() );
		this.south.add(new JLabel("This application is concerned about all troubles in the City. Help us to fix them."));
		this.south.setBackground(getColorLogo());
		container.add(this.south, BorderLayout.SOUTH);
		
		// generate a login panel
		this.logInPage = new LogInPage(this);
		container.add(logInPage, BorderLayout.EAST);
		
		// generates the pages
		this.logUpPage = new LogUpPage(this);
		this.logUpPage.setVisible(false);
		
		this.generalPage = new GeneralPage(this);
		this.generalPage.setVisible(false);
		
		this.administratorPage = new AdministratorPage(this);
		this.administratorPage.setVisible(false);
		
		this.windowAdapter = new WindowAdapter() {
			
			@Override
			public void windowClosing(WindowEvent arg0) {//Serializes before exiting

				MySystem sys;
				try { // Add it to the system
					sys = MySystem.getInstance();
					sys.write(); //Save the app information
					System.exit(0);
				}catch(FileNotFoundException sys_1){			
					System.out.println(sys_1);
				}catch(IOException sys_2){
					System.out.println(sys_2);
				}
				
				UIManager.put("OptionPane.message", Color.red);
				JOptionPane.showMessageDialog(getGui(), "There was an error trying to close the application. Try again", "Close error", JOptionPane.ERROR_MESSAGE);
			}
			
		};
		
		this.addWindowListener(this.windowAdapter);
		
		// Gives visibility to the the initial Components
		this.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		this.setVisible(true);
		setResizable(true);
		
	}

	/**
	 * sets the visibility of the log in page to false
	 * 
	 * @param logInOK 
	 * */
	public void logInResult(boolean logInOK) {
		if (logInOK) {
			logInPage.setVisible(false);
			
		} else {
			this.logInPage.setError();
		}
	}
	
	/**
	 * sets the visibility of the log in page to false if you 
	 * enter as the administrator
	 * 
	 * @param logInAdminOK 
	 * */
	public void logInAdminResult(boolean logInAdminOK) {
		if (logInAdminOK) { 
			logInPage.setVisible(false);
		}
	}
	
	/**
	 * sets the visibility of the log up page to false
	 * 
	 * @param logUpOk, the result of the register operation
	 * */
	public void registerResult(boolean logUpOk) {
		if (logUpOk) { 
			logUpPage.setVisible(false);
		} else {
			this.logUpPage.setError();
		}
	}
	
	/**
	 * @return colorLogo the color of the logo
	 * */
	public Color getColorLogo() {
		return colorLogo;
	}

	/**
	 * @return colorLogoLight the color of the logo
	 * */
	public Color getColorLogoLight() {
		return colorLogoLight;
	}

	/**
	 * @return colorLogoDArk the color of the logo
	 * */
	public Color getColorLogoDark() {
		return colorLogoDark;
	}
	
	/**
	 * @return north the north part of the application
	 * */
	public JPanel getNorth() {
		return north;
	}

	/**
	 * @param north sets the north part of the application
	 * */
	public void setNorth(JPanel north) {
		this.north = north;
	}

	/**
	 * @return south the south part of the application
	 * */
	public JPanel getSouth() {
		return south;
	}

	/**
	 * @param south sets the south part of the application
	 * */
	public void setSouth(JPanel south) {
		this.south = south;
	}
	
	/**
	 * @param title sets the title of the application
	 * */
	public void setTitle(String title) {
		super.setTitle(title);
	}
	

	/**
	 * @return administratorPage the administrator page the application is using
	 * */
	public AdministratorPage getAdministratorPage() {
		return administratorPage;
	}

	/**
	 * @param initialPage the initial page of the application 
	 * */
	public void setInitialPage(InitialPage initialPage) {
		this.initialPage = initialPage;
	}

	/**
	 * @return generalPage the general page the application is using
	 * */
	public GeneralPage getGeneralPage() {
		return generalPage;
	}

	/**
	 * @param generalPage the general page of the application 
	 * */
	public void setGeneralPage(GeneralPage generalPage) {
		this.generalPage = generalPage;
	}

	/**
	 * @param c the controller of the application 
	 * */
	public void setController(Controller c) {
		this.controller = c;
	}
	
	/**
	 * @return controller the controller the application is using
	 * */
	public Controller getController() {
		return this.controller;
	}
	
	/**
	 * @return logInPage the logIn page the application is using
	 * */
	public LogInPage getLogInPage() {
		return this.logInPage;
	}
	
	/**
	 * @return logUpPage the logUp page the application is using
	 * */
	public LogUpPage getLogUpPage() {
		return this.logUpPage;
	}
	
	/**
	 * @return initialPage the initial page the application is using
	 * */
	public InitialPage getInitialPage() {
		return this.initialPage;
	}
	
	/**
	 * @return this the gui the application is using
	 * */
	public Gui getGui() {
		return this;
	}
	
	/**
	 * @return screenSize the screen size of the computer
	 * */
	public Dimension getScreenSize() {
		return this.screenSize;
	}
	
	/**
	 * @param toDelete JPanel you want to remove
	 * */
	public void removeJPanel(JPanel toDelete) {
		this.getContentPane().remove(toDelete);
	}
	
	/**
	 * @param toAdd JPanel you want to add
	 * @param zone you want to add the JPanel to
	 * */
	public void addJPanel(JPanel toAdd, String zone) {
		this.getContentPane().add(toAdd, zone);
	}
	
}
