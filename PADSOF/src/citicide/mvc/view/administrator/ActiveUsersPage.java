package citicide.mvc.view.administrator;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.Color;
import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

import citicide.*;

/**
* This is the ActiveUsersPage class from the citicide.mvc.view.adiminstrator package
*
* It allows the administrator to see the active users and to ban
* the ones he/she wants.
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class ActiveUsersPage extends JPanel implements ActionListener{

	private static final long serialVersionUID = 1L;
	
	private AdministratorPage administratorPage;
	
	private KeyAdapter keyAdapter;
	
	private JLabel text = new JLabel("Username");
	private JLabel userId = new JLabel("User ID: ");
	
	private JTextField banField = new JTextField(50);
	private JButton banButton = new JButton("Ban");
	
	/**
	* This is the constructor class of ActiveUsersPage
	* 
	* @param administratorPage administrator page the class active users is using
	*/
	public ActiveUsersPage(AdministratorPage administratorPage) {
		
		this.administratorPage = administratorPage;
		
		this.setLayout(new BorderLayout());
		
		JPanel pn = new JPanel(new GridLayout(3, 10, 5, 5)); //Rows, columns, separation between rows, separation columns
		//North panel
		Font myFont = new Font("Sans Regular", Font.BOLD, 20);
	    text.setFont(myFont);
	    
	    JPanel n1 = new JPanel(new FlowLayout(FlowLayout.CENTER));
		n1.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 1) );
		n1.add(text);
		pn.add(n1);
		
		this.add(pn, BorderLayout.NORTH);
	
		//Central part
		JPanel c = new JPanel(new GridBagLayout());
		GridBagConstraints constraints = new GridBagConstraints();
		
		//separator
		constraints.gridx = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 1; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;  
		
		c.add(userId, constraints);
		
		//separator
		constraints.gridx = 0; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_END;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 1; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_END;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 2; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_END;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		
		constraints.gridx = 1; 
		constraints.gridy = 5; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		JLabel text2 = new JLabel("Ban User");
		Font myFont2 = new Font("Sans Regular", Font.BOLD, 18);
		text2.setFont(myFont2);
		c.add(text2, constraints);
		
		constraints.gridx = 1; 
		constraints.gridy = 6; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		c.add(new JLabel("Give a reason. 50 characters maximum."), constraints);
		
		constraints.gridx = 1; 
		constraints.gridy = 7; 
		constraints.gridwidth = 1;
		constraints.gridheight = 2;
        banField.setPreferredSize( new Dimension( 800, 50 ) );
		c.add(banField, constraints);
		
		constraints.gridx = 1; 
		constraints.gridy = 9; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		banButton.setBackground(Color.RED);
		c.add(banButton, constraints);
		
		constraints.gridx = 2; 
		constraints.gridy = 6; 
        constraints.weightx = 0.1;
		c.add(new JLabel(""), constraints);
		
        this.add(c,BorderLayout.CENTER);
        
        //Updates the active users, when you chose one
        this.administratorPage.getListuser().getSelectionModel().addListSelectionListener(listAc->{
        	String currentuser = administratorPage.getListuser().getSelectedValue();
        	updateUser(currentuser);
        });
        
        /* banButton action listener. When you push the ban
         * button it calls the methods of the code of user
         * it needs to use to set the user he picked as banned.
         * */
        banButton.addActionListener(e->{
      		MySystem  sys = MySystem.getInstance();
			String username2 = administratorPage.getListuser().getSelectedValue(); 							
			User actualUser = sys.getSingleUser(username2); 
			try {
				actualUser.setBanReason(banField.getText());
			}catch(NullPointerException ex) {return;}
			actualUser.ban();
			JOptionPane.showMessageDialog(administratorPage, "User banned",
					"ACTION PERFORMED", JOptionPane.INFORMATION_MESSAGE);
			this.administratorPage.setUsersModel(this.administratorPage.getGui().getController().updateList(AdministratorPage.ACTIVEUSERS));
			this.administratorPage.setUserBannedListModel(this.administratorPage.getGui().getController().updateList(AdministratorPage.BANNEDUSERS));
        });
        
        /* keyPressed. When you push enter on the ban field
         * it calls the methods of the code of user
         * it needs to use to set the user he picked as banned.
         * */
        this.keyAdapter = new KeyAdapter() {
			@Override
			public void keyPressed(KeyEvent event) {
				if(event.getKeyCode() == KeyEvent.VK_ENTER) {
					MySystem  sys = MySystem.getInstance();
					String username = administratorPage.getListuser().getSelectedValue(); 							
					User actualUser = sys.getSingleUser(username);
					actualUser.setBanReason(banField.getText());
					actualUser.ban();
					JOptionPane.showMessageDialog(administratorPage, "User banned",
							"Information message", JOptionPane.INFORMATION_MESSAGE);
				}
			}
		};
		this.banField.addKeyListener(this.keyAdapter);
	}

	/**
	* This method responses to the different actions made by the user
	* 
	* @param e, the action performed by the user
	*/
	@Override
	public void actionPerformed(ActionEvent e) {
	}
	
	/**
	 * This method allows the administrator to update the 
	 * page any time he makes a change
	 * 
	 * @param u, user you want to interact with
	 * */
	public void updateUser(String u) {
		
		try {
			MySystem  sys = MySystem.getInstance();
			String username = u; 							
			User actualUser = sys.getSingleUser(username); 
			
			this.text.setText( actualUser.getNickname());
			this.userId.setText( "User ID: " + actualUser.getiDNumber());
			this.banField.setText("");
			
		}catch(NullPointerException e) {
			this.text.setText( "Username");
			this.userId.setText( "User ID: Select an user");
			this.banField.setText("");
		}
		
	}

}