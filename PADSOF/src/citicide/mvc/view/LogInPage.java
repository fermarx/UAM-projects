package citicide.mvc.view;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import citicide.MySystem;
import citicide.User;
import citicide.UserPrivateStatus;
import citicide.mvc.view.administrator.AdministratorPage;

/**
* This is the LogInPage class from the citicide.mvc.view package
* It is the first window that appear in the application and it
* uses the InitialPage as base
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class LogInPage extends JPanel implements ActionListener, MouseListener {

	private static final long serialVersionUID = 1L;
	
	private Font myFont = new Font("Sans Regular", Font.PLAIN, 15);
	private JLabel nicknameLabel = new JLabel("Nickname");
	private JTextField nicknameField = new JTextField(20);
	private JLabel passwordLabel = new JLabel("Password");
	private JPasswordField passwordField = new JPasswordField(20);
	private JCheckBox passwordBox = new JCheckBox("Show password");
	private JButton logInButton = new JButton("Log in");
	private String welcomeText = "New to Citicide?";
	private JLabel welcomeLabel = new JLabel(welcomeText);
	private String registerText = "Create an account";
	private JLabel registerLabel = new JLabel(registerText);
	private KeyAdapter keyAdapter;
	private Gui gui;
	
	/**
	* This is the constructor method
	*  
	* @param gui, the Graphical User Interface
	*/
	public LogInPage(Gui gui) {	
		this.gui = gui;
		this.setLayout(new GridBagLayout());
		
		this.nicknameField.setFont(myFont);
		this.passwordField.setFont(myFont);
		
		JPanel emp = new JPanel(new FlowLayout());//Just a miscellaneous 
		emp.add(new JLabel(""));
		
		GridLayout grid = new GridLayout(7, 1, 5, 5); //Rows, columns, separation between rows, separation columns
		JPanel pn = new JPanel(grid);
		pn.add(nicknameLabel, BorderLayout.EAST);
		pn.add(nicknameField, BorderLayout.EAST);
		pn.add(passwordLabel, BorderLayout.EAST);
		pn.add(passwordField, BorderLayout.EAST);
		
		JPanel checkBox = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0)); //Alignment, horizontal separation, vertical separation
		checkBox.add(this.passwordBox);
		pn.add(checkBox, BorderLayout.EAST);
		
		pn.add(logInButton, BorderLayout.EAST);
		
		grid.setVgap(10);
		JPanel welcome = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));
		welcome.add(this.welcomeLabel);
		welcome.add(new JLabel("          ")); //To have space between labels and the first one is aligned to the left
		welcome.add(this.registerLabel);
		pn.add(welcome, BorderLayout.EAST);
		
		pn.setPreferredSize(new Dimension(400, 250)); //width, height

		this.setPreferredSize(new Dimension(650, 500)); //width, height
        
		GridBagConstraints constraints = new GridBagConstraints();
		constraints.gridx = 0; 
		constraints.gridy = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.weighty = 1.0;
        constraints.anchor = GridBagConstraints.CENTER;
        
        this.add (emp, constraints); 
        
		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 2; 
        constraints.weighty = 1.0;
        constraints.anchor = GridBagConstraints.NORTHWEST;
        constraints.fill = GridBagConstraints.NONE;

        this.add (pn, constraints);	
        
        logInButton.addActionListener(this); 
		
		this.passwordBox.addActionListener(this);
			
		this.keyAdapter = new KeyAdapter() {
			
			@Override
			public void keyPressed(KeyEvent event) { 
				 
				if(event.getKeyCode() == KeyEvent.VK_ENTER) {
					if(gui.getController().logInAdmin(nicknameField.getText(), 	String.valueOf( passwordField.getPassword() )) == true) {
						goToAdminstratorPage();
						
					}else if(gui.getController().logIn(nicknameField.getText(), String.valueOf( passwordField.getPassword() ))) {
						goToGeneralPage();	
					}			
				}
		    }
		};
		
		this.passwordField.addKeyListener(this.keyAdapter);
        this.nicknameField.addKeyListener(this.keyAdapter);
        
        registerLabel.addMouseListener(this);
        
        registerLabel.setForeground(Color.BLUE.darker()); //Changes the text color to blue
        registerLabel.setCursor(new Cursor(Cursor.HAND_CURSOR)); //Changes the cursor to a hand when it is placed on the text
        		
	}

	/**
	* This method responses to the different actions made by the user
	* 
	* @param event, the action performed by the user
	*/
	@Override
	public void actionPerformed(ActionEvent event) {
		
		if(passwordBox.isSelected()) {
			passwordField.setEchoChar((char)0);
		}else {
			passwordField.setEchoChar('\u2022'); //Change entered password to bullet points
		}
		
		if(event.getSource() == logInButton) {
			MySystem sys = MySystem.getInstance();
			User u = null;
			
			try {
				u = sys.getSingleUser(nicknameField.getText());
				if(u.getSituation().equals(UserPrivateStatus.BANNED)){
					JOptionPane.showMessageDialog(this, ("You are banned, reason:  "
							+ u.getBanReason()), "Log in - Error", JOptionPane.ERROR_MESSAGE);
					return;
				}
			}catch(NullPointerException e1) {
				JOptionPane.showMessageDialog(this, "This user does not exist",
						"Log in - Error", JOptionPane.ERROR_MESSAGE);
				return;
			}
			
			
			if(gui.getController().logInAdmin(nicknameField.getText(), String.valueOf( passwordField.getPassword() ))) {
				goToAdminstratorPage();

			}else if(gui.getController().logIn(nicknameField.getText(), String.valueOf( passwordField.getPassword() ))) {
				goToGeneralPage();
			}
		}
    }
	
	/**
	* This method shows a message when the user password is incorrect.
	* Finally it cleans the fields
	*/
	public void setError() {	
	    UIManager.put("OptionPane.message", Color.red);
		JOptionPane.showMessageDialog(gui, "Password is incorrect", "Log in - Error", JOptionPane.ERROR_MESSAGE);
		cleanFields();
	}

	/**
	* This method changes the view from this one, to the AdministratorPage
	*/
	private void goToAdminstratorPage() {
		cleanFields();
		gui.getLogInPage().setVisible(false);
		gui.getInitialPage().setVisible(false);
		gui.removeJPanel(gui.getLogInPage());
		gui.removeJPanel(gui.getInitialPage());
		AdministratorPage administratorPanel = gui.getAdministratorPage();
		administratorPanel.setVisible(true);
		gui.addJPanel(administratorPanel, BorderLayout.CENTER);
	}
	
	/**
	* This method changes the view from this one, to the LogUpPage
	*/
	private void goToLogUpPage() {
		cleanFields();
		gui.getLogInPage().setVisible(false);
		gui.removeJPanel(gui.getLogInPage());
		LogUpPage aux = gui.getLogUpPage();
		aux.setVisible(true);
		gui.addJPanel(aux, BorderLayout.EAST);
	}
	
	/**
	* This method changes the view from this one, to the GeneralPage
	*/
	private void goToGeneralPage() {
		GeneralPage generalPage = gui.getGeneralPage();
		if(generalPage == null) {
			generalPage = new GeneralPage(this.gui);
			this.gui.setGeneralPage(generalPage);
		}
		
		generalPage.setCurrentUser(nicknameField.getText());
		this.gui.getGeneralPage().setGroupListModel(this.gui.getController().updateList(GeneralPage.GROUP));
		cleanFields();
		gui.getLogInPage().setVisible(false);
		gui.getInitialPage().setVisible(false);
		gui.getNorth().setVisible(false);
		gui.removeJPanel(gui.getLogInPage());
		gui.removeJPanel(gui.getInitialPage());
		gui.removeJPanel(gui.getNorth());
		generalPage.setVisible(true);
		gui.addJPanel(generalPage, BorderLayout.CENTER);
	}
	
	/**
	* This method cleans the nickname and password fields when an error is produced
	*/
	private void cleanFields() {
		nicknameField.setText("");
		passwordField.setText("");
		this.passwordBox.setSelected(false); //In case the checkBox is selected
		this.passwordField.setEchoChar('\u2022'); //Change entered password to bullet points
	}
	
	/**
	* This method produces a change of view to the LogUpPage when the mouse
	* is clicked on the "Create an account" text
	* 
	* @param e, event that is produced
	*/
	@Override
	public void mouseClicked(MouseEvent e) {
		if(e.getSource() == registerLabel) {	
			goToLogUpPage();
		}	
	}
	
	/**
	* This method underlines the "Create an account" text when the mouse is placed
	* over it
	* 
	* @param arg0, event that is produced
	*/
	@Override
	public void mouseEntered(MouseEvent arg0) {
		registerLabel.setText("<html><a href=''>" + registerText + "</a></html>");
	}

	/**
	* This method just makes the "Create an account" text return to its original shape
	* after the mouseEntered method is executed
	* 
	* @param arg0, event that is produced
	*/
	@Override
	public void mouseExited(MouseEvent arg0) {
		registerLabel.setText(registerText);	
	}

	/**
	* This method is just needed to make the implementation of the MouseListener
	* possible
	* 
	* @param arg0, event that is produced
	*/
	@Override
	public void mousePressed(MouseEvent arg0) {
		
	}

	/**
	* This method is just needed to make the implementation of the MouseListener
	* possible
	* 
	* @param arg0, event that is produced
	*/
	@Override
	public void mouseReleased(MouseEvent arg0) {
		
	}
}