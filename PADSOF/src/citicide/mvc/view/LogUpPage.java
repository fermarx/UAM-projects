package citicide.mvc.view;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import citicide.mvc.view.administrator.AdministratorPage;

/**
* This is the LogUpPage class from the citicide.mvc.view package
* It uses the InitialPage as base, it is used when a user wants to register
* in the application
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class LogUpPage extends JPanel implements ActionListener {

	private static final long serialVersionUID = 1L;
	private JLabel text= new JLabel("Create your account");
	private JLabel userLabel = new JLabel("Nickname");
	private JLabel numberLabel = new JLabel("ID number");
	private JLabel passwordLabel = new JLabel("Password");
	private JLabel passwordConfirmLabel = new JLabel("Confirm password");
	
	private JTextField userField = new JTextField(20);
	private JTextField numberField = new JTextField(9);
	private JPasswordField passwordField = new JPasswordField(20);
	private JPasswordField passwordConfirmField = new JPasswordField(20);
	
	private JButton logUpButton = new JButton("Log up");
	private JButton backButton = new JButton("Back");
	private JCheckBox passwordBox = new JCheckBox("Show password");
	private JCheckBox passwordConfirmBox = new JCheckBox("Show password");
	private String emptyText = "";
	private JLabel emptyLabel = new JLabel(emptyText);
	
	private KeyAdapter keyAdapter;
	private Gui gui;
	
	/**
	* This is the constructor method
	*  
	* @param gui, the Graphical User Interface
	*/
	public LogUpPage(Gui gui) {	
		this.gui = gui;
		this.setLayout(new GridBagLayout());
		
		JPanel emp = new JPanel(new FlowLayout());
		emp.add(new JLabel(""));
		
		JPanel pn = new JPanel(new GridLayout(14,1, 5, 5)); //Rows, columns, separation between rows, separation columns
		Font myFont = new Font("Sans Regular", Font.BOLD, 20);
	    text.setFont(myFont);
		pn.add(text);
		pn.add(userLabel);
		pn.add(userField);
		pn.add(numberLabel);
		pn.add(numberField);
		
		pn.add(passwordLabel);
		pn.add(passwordField);
		JPanel passwordCheckBox = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0)); //Alignment, horizontal separation, vertical separation
		passwordCheckBox.add(this.passwordBox);
		pn.add(passwordCheckBox, BorderLayout.EAST);
		
		pn.add(passwordConfirmLabel);
		pn.add(passwordConfirmField);
		JPanel passwordConfirmCheckBox = new JPanel(new FlowLayout(FlowLayout.RIGHT, 0, 0));
		passwordConfirmCheckBox.add(this.passwordConfirmBox);
		pn.add(passwordConfirmCheckBox, BorderLayout.EAST);
		
		logUpButton.addActionListener(this);
		pn.add(logUpButton);
		
		pn.add(emptyLabel);
		backButton.addActionListener(this);
		pn.add(backButton);
		
		pn.setPreferredSize(new Dimension(400, 400)); //width, height

		this.setPreferredSize(new Dimension(650, 500)); 
     
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
        
		this.passwordBox.addActionListener(this);
		this.passwordConfirmBox.addActionListener(this);
		
		this.keyAdapter = new KeyAdapter() {
			
			@Override
			public void keyPressed(KeyEvent e) {
				if(e.getKeyCode() == KeyEvent.VK_ENTER) {
					System.out.println(userField.getText().length());
					if(userField.getText().endsWith(" ") || userField.getText().equals("")) {
						setNicknameError();
						return;
						
					}else if(numberField.getText().length() != 9) {
						setIdError();
						return;
					}
					
					if(gui.getController().logUp(userField.getText(),
											  numberField.getText(),
											  String.valueOf(passwordField.getPassword()),
											  String.valueOf(passwordConfirmField.getPassword()) ) == true) {
						goToLogInPage();
					}
					
				}
			}
			
		};
		
		this.passwordField.addKeyListener(this.keyAdapter);
        this.userField.addKeyListener(this.keyAdapter);
        this.numberField.addKeyListener(this.keyAdapter);
        this.passwordConfirmField.addKeyListener(this.keyAdapter);
	}
	
	/**
	* This method responses to the different actions made by the user
	* 
	* @param e, the action performed by the user
	*/
	@Override
	public void actionPerformed(ActionEvent e) {
		
		if(passwordBox.isSelected()) {
			passwordField.setEchoChar((char)0);
		}else {
			passwordField.setEchoChar('\u2022'); //Change entered password to bullet points
		}
		
		if(passwordConfirmBox.isSelected()) {
			passwordConfirmField.setEchoChar((char)0);
		}else {
			passwordConfirmField.setEchoChar('\u2022'); //Change entered password to bullet points
		}
		
		if(e.getSource() == logUpButton) {
			
			if(userField.getText().endsWith(" ") || userField.getText().equals("")) {
				setNicknameError();
				return;
				
			}else if(numberField.getText().length() != 9) {
				setIdError();
				return;
			}
			
			if(gui.getController().logUp(userField.getText(),
									  numberField.getText(),
									  String.valueOf(passwordField.getPassword()),
									  String.valueOf(passwordConfirmField.getPassword()) ) == true) {
				
				this.gui.getAdministratorPage().setUsersModel(this.gui.getController().updateList(AdministratorPage.ACTIVEUSERS));
				this.gui.getAdministratorPage().setUserPendingModel(this.gui.getController().updateList(AdministratorPage.PENDINGUSERS));
				this.gui.getAdministratorPage().setUserBannedListModel(this.gui.getController().updateList(AdministratorPage.BANNEDUSERS));
				this.gui.getAdministratorPage().setListProjectListModel(this.gui.getController().updateList(AdministratorPage.PENDINGPROJECTS));
				
				goToLogInPage();
			}
		}else if(e.getSource() == backButton) {
			goToLogInPage();
		}
	}
	
	/**
	* This method shows a message when the nickname is already used
	*/
	public void setError() {
	    UIManager.put("OptionPane.message", Color.red);
		JOptionPane.showMessageDialog(gui, "This nickname is already used. Choose another one.", "Log up - Error", JOptionPane.ERROR_MESSAGE);
	}

	/**
	* This method shows a message when the ID does not have 9 characters
	*/
	public void setIdError() {	
	    UIManager.put("OptionPane.message", Color.red);
		JOptionPane.showMessageDialog(gui, "The ID has to have 9 characters", "Log up - Error", JOptionPane.ERROR_MESSAGE);
	}
	
	/**
	* This method shows a message when the nickname does not contain any character or it ends with a space
	*/
	public void setNicknameError() {	
	    UIManager.put("OptionPane.message", Color.red);
		JOptionPane.showMessageDialog(gui, "The nickname must contain some character and not end with a space", "Log up - Error", JOptionPane.ERROR_MESSAGE);
	}
	
	/**
	* This method changes the view from this one, to the LogInPage
	*/
	private void goToLogInPage() {
		cleanFields();
		gui.getLogUpPage().setVisible(false);
		gui.removeJPanel(gui.getLogUpPage());
		LogInPage aux = gui.getLogInPage();
		aux.setVisible(true);
		gui.addJPanel(aux, BorderLayout.EAST);
	}
	
	/**
	* This method cleans all the fields and resets the show passwords checkboxes
	*/
	private void cleanFields() {
		this.userField.setText("");
		this.numberField.setText("");
		this.passwordField.setText("");
		this.passwordConfirmField.setText("");
		this.passwordBox.setSelected(false); //In case the checkBox is selected
		this.passwordField.setEchoChar('\u2022'); //Change entered password to bullet points
		this.passwordConfirmBox.setSelected(false); //In case the checkBox is selected
		this.passwordConfirmField.setEchoChar('\u2022'); //Change entered password to bullet points
	}

}
