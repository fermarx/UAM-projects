package citicide.mvc.view.administrator;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.SwingConstants;

import citicide.MySystem;
import citicide.User;
import citicide.UserPrivateStatus;

/**
* This is the PendingUsersPage class from the citicide.mvc.view.administrator package
*
* It allows the administrator to see the pending users and to accept
* or reject the ones he/she wants.
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class PendingUsersPage extends JPanel implements ActionListener{

	private static final long serialVersionUID = 1L;

	private AdministratorPage administratorPage;
	
	private JLabel text = new JLabel("Username");
	private JLabel userId = new JLabel("User ID: ");
	private JButton autorizeButton = new JButton("Autorize");
	private JButton rejectButton = new JButton("Reject");
	
	/**
	* This is the constructor class of PendingUsersPage
	* 
	* @param administratorPage administrator page the class pending users is using
	*/
	public PendingUsersPage(AdministratorPage administratorPage) {

		this.administratorPage = administratorPage;
		
		this.setLayout(new BorderLayout());
		
		String username = administratorPage.getListuser().getSelectedValue();
		
		for(User u: administratorPage.getUsers()) {
			if(u.getNickname().equals(username)) {
				break;
			}
		}
		
		JPanel pn = new JPanel(new GridLayout(3, 6, 5, 5)); //Rows, columns, separation between rows, separation columns
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
        constraints.weightx = .05;
        constraints.weighty = .05;
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;  
		
		c.add(userId, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 1; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 2; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		autorizeButton.setBackground(Color.GREEN);
		c.add(autorizeButton, constraints);
		
		constraints.gridx = 2; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		rejectButton.setBackground(Color.RED);
		c.add(rejectButton, constraints);
		
		this.add(c,BorderLayout.CENTER);

		//Updates the pending users, when you chose one
        this.administratorPage.getListuserpending().getSelectionModel().addListSelectionListener(listAc->{
        	String currentuser = administratorPage.getListuserpending().getSelectedValue();
        	updateUser(currentuser);
        });
        
        /* autorizeButton action listener. When you push the autorize
         * button it calls the methods of the code of user
         * it needs to use to set the user he picked as autorized.
         * */
        autorizeButton.addActionListener(e->{
    		  	MySystem  sys = MySystem.getInstance();
  				String username2 = administratorPage.getListuserpending().getSelectedValue(); 							
  				User actualUser = sys.getSingleUser(username2);
  				try{
  					actualUser.setSitutation(UserPrivateStatus.ACCEPTED);
  				}catch(NullPointerException ex) {return;};
  				JOptionPane.showMessageDialog(administratorPage, "User accepted",
  						"Information message", JOptionPane.INFORMATION_MESSAGE);
  				this.administratorPage.setUserPendingModel(this.administratorPage.getGui().getController().updateList(AdministratorPage.PENDINGUSERS));
  				this.administratorPage.setUsersModel(this.administratorPage.getGui().getController().updateList(AdministratorPage.ACTIVEUSERS));
        	  });
        
        /* rejectButton action listener. When you push the reject
         * button it calls the methods of the code of user
         * it needs to use to set the user he picked as rejected.
         * */
        rejectButton.addActionListener(e->{ 
      	  		MySystem  sys = MySystem.getInstance();
				String username2 = administratorPage.getListuserpending().getSelectedValue(); 							
				User actualUser = sys.getSingleUser(username2);
				try{
					actualUser.setSitutation(UserPrivateStatus.REJECTED);
				}catch(NullPointerException ex) {return;}
				JOptionPane.showMessageDialog(administratorPage, "User rejected",
  						"Information message", JOptionPane.ERROR_MESSAGE);	
				this.administratorPage.setUserPendingModel(this.administratorPage.getGui().getController().updateList(AdministratorPage.PENDINGUSERS));
  	      		});
	}

	@Override
	public void actionPerformed(ActionEvent e) {
	}
	
	/**
	 * This method allows the administrator to update the 
	 * page any time he makes a change
	 * 
	 * @param u user you want to interact with
	 * */
	public void updateUser(String u) {
		
		try {
			MySystem  sys = MySystem.getInstance();
			String username = u; 							
			User actualUser = sys.getSingleUser(username); 
			
			this.text.setText( actualUser.getNickname());
			this.userId.setText( "User ID: " + actualUser.getiDNumber());
			
		}catch(NullPointerException e) {
			this.text.setText( "Username");
			this.userId.setText( "User ID: Select an user");
		}
		
	}

}