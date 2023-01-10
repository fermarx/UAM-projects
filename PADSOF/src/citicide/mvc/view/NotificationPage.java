package citicide.mvc.view;

import java.awt.*;
import javax.swing.*;
import citicide.*;

/**
* This is the NotificationPage class from the citicide.mvc.view package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class NotificationPage extends JPanel{

	private static final long serialVersionUID = 1L;
	private GeneralPage genPage;
	private final String noNot = "Select a Notification";
	private final String noBod = "Select a Notification in order to read it";
	private JLabel title = new JLabel("");
	private JLabel body = new JLabel("");;
	private JPanel north;
	private JPanel center;
	private JPanel south;
	
	/**
	* This is the constructor class of NotificationPage
	* 
	* @param gp general page the class notification is using
	*/
	public NotificationPage(GeneralPage gp) {
		this.genPage = gp;
		
		this.setLayout(new BorderLayout());
		
		// Generates the North Part
		this.north = new JPanel((new FlowLayout(FlowLayout.CENTER)));
		this.north.add(new JLabel("Welcome to the Notification Menu, here you can see a summary of your activities as User"));
		this.add(this.north, BorderLayout.NORTH);
		
		//Generates the south part
		this.south = new JPanel((new FlowLayout(FlowLayout.CENTER)));
		this.south.add(new JLabel("User can not write any notification but you will "
								+ "receive them while interacting with our GUI"));
		this.add(this.south, BorderLayout.SOUTH);
		
		// Generates the Central part
		this.center = new JPanel(new GridBagLayout());
		this.center.setVisible(true);
		
		GridBagConstraints constraints = new GridBagConstraints();
		constraints.gridx = 0; 
		constraints.gridy = 0;
		constraints.ipadx = 1;
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
        this.center.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
        
        constraints.insets = new Insets(0,20,0,0);
		constraints.gridx = 0; 
		constraints.gridy = 1;
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		this.title.setText(this.noNot);
		this.center.add(this.title, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 2;
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		this.body.setText(this.noBod);
		this.center.add(this.body, constraints);
		
		constraints.insets = new Insets(0,0,0,0);
		constraints.gridx = 0; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
        constraints.weightx = 1.0;
        constraints.weighty = 1.0;
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_END;
        this.center.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
        
        this.add(this.center, BorderLayout.CENTER);
        this.setVisible(true);
        
        //Updates the Notification, when you chose one
        this.genPage.getNotificationList().getSelectionModel().addListSelectionListener(listAc->{
        	MySystem sys = MySystem.getInstance();
        	String currentNot = genPage.getNotificationList().getSelectedValue();
        	try {
        		sys.getSingleNotification(currentNot).setRead(true);	//Mark as read
        		updateNotification(currentNot); 						// Updates the notification page
        		
        	}catch(NullPointerException e) {
        		//If the notification does not exists do nothing
        	}
        	
        });
	}
	
	/**
	 * This method allows the general page to update the 
	 * page any time he makes a change
	 * 
	 * @param currentNot notification you want to interact with
	 * */
	private void updateNotification(String currentNot) {
		try {
			MySystem  sys = MySystem.getInstance();
			Notification currentNotification = sys.getSingleNotification(currentNot);
	
			
			this.title.setText( "<html><p style=\"width:750px\">" + "You reading the notification:  " + currentNotification.getTitle() +"</p></html>"); 
			this.body.setText( "<html><p style=\"width:750px\">" +"Content of the notification: <br> <br>" + currentNotification.getBody() + "</p></html>");
			
			
		}catch(NullPointerException e) {
			this.title.setText( this.noNot );
			this.body.setText( this.noBod );
		}
	}
	
}
