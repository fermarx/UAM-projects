package citicide.mvc.view;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import javax.swing.*;
import citicide.*;
import citicide.mvc.view.administrator.AdministratorPage;

/**
* This is the GeneralPage class from the citicide.mvc.view package
* This is the base for the rest of view of a normal user
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class GeneralPage extends JPanel implements ActionListener{
	
	private final static long serialVersionUID = 1L;
	public final static String GROUP = "Your groups";
	public final static String PROJECT = "Voted projects";
	public final static String SEARCH_GROUP = "New groups";
	public final static String SEARCH_PROJECT = "New projects";
	public final static String NOTIFICATION = "Your notifications";
	private Gui gui;
	private String currentUser;
	private JButton out;
	private JButton notifTurnOn;
	private JButton notifTurnOff;
	private final String[] menus = {GROUP, PROJECT, SEARCH_GROUP, SEARCH_PROJECT, NOTIFICATION};
	private JComboBox<String> comboBox;
	private JPanel cards;
	private JPanel west;
	private JPanel north;
	private JPanel rightNorth;
	private JPanel leftNorth;
	private JPanel empty;
	private JPanel center;
	private JLabel userLabel;
	private JLabel notificationLabel;
	private JLabel emptyLabel;
	private JList <String> groupList;
	private JList <String> notificationList;
	private JList <String> projectList;
	private JList <String> searchGroupList;
	private JList <String> searchProjectList;

	/**
	* This is the constructor method
	*  
	* @param gui, the Graphical User Interface
	*/
	public GeneralPage(Gui gui){
		this.gui = gui;
		this.setLayout(new BorderLayout());
		
        //Creates the north panel of the application
		this.north = new JPanel(new GridLayout(1,3, 10, 10)); //Rows, columns, separation between rows, separation columns
		this.north.setBackground(gui.getColorLogo());
		
		this.leftNorth = new JPanel(new FlowLayout(FlowLayout.LEFT, 3, 3));
		this.leftNorth.setBackground(gui.getColorLogo());
		this.leftNorth.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 10) );
		this.notificationLabel = new JLabel("Notifications: ", FlowLayout.LEFT);
		this.notifTurnOn = new JButton("Turn on");
		this.notifTurnOn.addActionListener(this);
        this.notifTurnOff = new JButton("Turn off");
        this.notifTurnOff.addActionListener(this);
        
        this.leftNorth.add(notificationLabel);
        this.leftNorth.add(notifTurnOn);
        this.leftNorth.add(notifTurnOff);
        this.north.add(this.leftNorth);
        
        this.empty = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 10));
        this.empty.setBackground(gui.getColorLogo());
        this.emptyLabel = new JLabel(" ");
        this.empty.add(this.emptyLabel);
        this.north.add(this.empty);
		
        this.rightNorth = new JPanel(new FlowLayout(FlowLayout.RIGHT, 3, 3));
        this.rightNorth.setBackground(gui.getColorLogo());
        this.rightNorth.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 10) );
        out = new JButton("LogOut");
        out.addActionListener(this);
        userLabel = new JLabel("", FlowLayout.LEFT);
        this.rightNorth.add(userLabel);
        this.rightNorth.add(out);
        this.north.add(this.rightNorth);
        
        this.add(this.north, BorderLayout.NORTH);
       
		
		//Creates the left combobox
		JPanel cards1 = new JPanel(new BorderLayout());
		JPanel cards2 = new JPanel(new BorderLayout());
		JPanel cards3 = new JPanel(new BorderLayout());
		JPanel cards4 = new JPanel(new BorderLayout());
		JPanel cards5 = new JPanel(new BorderLayout());
		this.cards = new JPanel(new CardLayout());
		
		this.cards.add(cards1, GROUP);
		this.cards.add(cards2, PROJECT);
		this.cards.add(cards3, SEARCH_GROUP);
		this.cards.add(cards4, SEARCH_PROJECT);
		this.cards.add(cards5, NOTIFICATION);
		
		this.comboBox = new JComboBox<String>(this.menus);
		
		// Start loading the main menus
		MySystem system = MySystem.getInstance();
        User currentUser = system.getSingleUser(this.currentUser);
        
        // Initialize the first menu (Load the initial groups for the user)
        try {
        	ArrayList<Group> userGroups = currentUser.getMembership(); //Throws a NullPointerException if the user doesn't have groups
            String [] groupNames = new String[userGroups.size()];
                        
            int i = 0;
            for(Group g: userGroups) {
            	groupNames[i] = g.getName();
            	i++;
            }

            //Creates the list
            this.groupList = new JList<>(groupNames);// this will be not visible
            groupList.setVisible(true); // Do not show it until a menu is selected
            cards1.add(this.groupList, BorderLayout.CENTER);
            
        }catch(NullPointerException e) {
        	String [] groupNames = {"Nothing to see here. Try to join a group"};
        	groupList = new JList<>(groupNames);// this will be not visible
        	groupList.setVisible(true); // Do not show it until a menu is selected
            cards1.add(groupList, BorderLayout.CENTER);
        }
        
        
        // Initialize the second menu (Load the initial projects for the user)
        ArrayList<Project> userProjects = null;
        try {
        	userProjects = currentUser.getIndividualVote(); //Throws a NullPointerException if the user doesn't have projects
            
        }catch(NullPointerException e) {
        	//Try with projects of the user groups
        }
    	
    	try {
    		ArrayList<Group> userGroups = currentUser.getMembership(); //Throws a NullPointerException if the user doesn't have groups
        	ArrayList<Project> groupProjects = new ArrayList<>();;
    		
        	for(Group g: userGroups) {
    			try {
    				ArrayList<Project> projects = g.getGroupVote();
        			for(Project p: projects) {
        				groupProjects.add(p);
        			}
    			}catch(NullPointerException e) {
    	        	//If group g didn't vote any project try with the next one
    	        }
    		}
    		
    		String [] projectNames = new String[userGroups.size() + groupProjects.size()];

    		int i = 0;
            for(Project p: userProjects) {
            	if(p.isAccepted() && !p.isExpired()) {
            		projectNames[i] = p.getTitle();
            		i++;            		
            	}
            }

            for(Project p: groupProjects) {
            	if(p.isAccepted() && !p.isExpired()) {
            		projectNames[i] = p.getTitle();
            		i++;
            	}
            }

        	this.projectList = new JList<>(projectNames);// this will be not visible
        	this.projectList.setVisible(true); // Do not show it until a menu is selected
            cards2.add(this.projectList, BorderLayout.CENTER);
        
    	}catch(NullPointerException e) {
    		String [] projectNames = {"Nothing to see here. Try to vote a project", "or wait until your group does it"};
        	this.projectList = new JList<>(projectNames);// this will be not visible
        	projectList.setVisible(true); // Do not show it until a menu is selected
            cards2.add(projectList, BorderLayout.CENTER);
    	}
    	
    	// Initialize the third menu (Load the initial groups of all the system)
    	 try {
         	ArrayList<Group> allGroups = system.getGroups(); //Throws a NullPointerException if the user doesn't have groups
            
         	if(allGroups.size() == 0) throw new NullPointerException();
         	
         	String [] allGroupNames = new String[allGroups.size()];
            allGroupNames[0] = "No groups in the system";            
            int i = 0;
            for(Group g: allGroups) {
             	allGroupNames[i] = g.getName();
             	i++;
            }
             
            //Creates the list
            this.searchGroupList = new JList<String>(allGroupNames);// this will be not visible
            this.searchGroupList.setVisible(true); // Do not show it until a menu is selected
            cards3.add(this.searchGroupList, BorderLayout.CENTER);
             
         }catch(NullPointerException e) {
         	String [] allGroupNames = {"No groups in the system"};
         	this.searchGroupList = new JList<>(allGroupNames);// this will be not visible
         	this.searchGroupList.setVisible(true); // Do not show it until a menu is selected
            cards3.add(this.searchGroupList, BorderLayout.CENTER);
         }
    	 
    	 
     	// Initialize the fourth menu (Load the initial groups of all the system)
    	 try {
         	ArrayList<Project> allProjects = system.getProjects(); //Throws a NullPointerException if the user doesn't have groups
            
         	if (allProjects.size() == 0) throw new NullPointerException();
         	
         	String [] allProjectsNames = new String[allProjects.size()];
            allProjectsNames[0] = "No projects in the system";             
            int i = 0;
            for(Project p: allProjects) {
            	if(p.isAccepted() && !p.isExpired()) {
            		allProjectsNames[i] = p.getTitle();
            		i++;
            	}
            }

            //Creates the list
            this.searchProjectList = new JList<String>(allProjectsNames);// this will be not visible
            this.searchProjectList.setVisible(true); // Do not show it until a menu is selected
            cards4.add(this.searchProjectList, BorderLayout.CENTER);
             
         }catch(NullPointerException e) {
         	String [] allProjectsNames = {"No projects in the system"};
         	this.searchProjectList = new JList<>(allProjectsNames);// this will be not visible
         	this.searchProjectList.setVisible(true); // Do not show it until a menu is selected
            cards4.add(this.searchProjectList, BorderLayout.CENTER);
         }

    	// Initialize the fifth menu (Load the initial notifications for the user)
    	 try {
          	ArrayList<Notification> myNotifications = currentUser.getNotification(); //Throws a NullPointerException if the user doesn't have groups
            
          	if(myNotifications.size() == 0) throw new NullPointerException();
          	
          	String [] myNotificationNames = new String[myNotifications.size()];
          	myNotificationNames[0] = "No notifications in the system";             
            int i = 0;
            for(Notification n: myNotifications) {
            	myNotificationNames[i] = n.getTitle();
	            i++;
	             
            }

  
            //Creates the list
            this.notificationList = new JList<String>(myNotificationNames);// this will be not visible
            this.notificationList.setVisible(true); // Do not show it until a menu is selected
            cards5.add(this.notificationList, BorderLayout.CENTER);
              
          }catch(NullPointerException e) {
          	String [] allProjectsNames = {"No notifications in the system"};
          	this.notificationList = new JList<>(allProjectsNames);// this will be not visible
          	this.notificationList.setVisible(true); // Do not show it until a menu is selected
             cards5.add(this.notificationList, BorderLayout.CENTER);
          }
    	 
    	 
		
		this.west = new JPanel(new GridBagLayout());
		GridBagConstraints constraints = new GridBagConstraints();
		constraints.gridx = 0; 
		constraints.gridy = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
		this.west.add(this.comboBox, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 1; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 20; 
        constraints.weightx = 1.0;
        constraints.weighty = 1.0;
        constraints.fill =  GridBagConstraints.BOTH;
		this.west.add(this.cards, constraints);

		this.add(this.west, BorderLayout.WEST);
		
		// Now, load the different part of the menus
		this.center = new JPanel(new CardLayout());
		JPanel mainCards1 = new GroupPage(this);
		JPanel mainCards2 = new ProjectPage(this);
		JPanel mainCards3 = new NewGroupPage(this);
		JPanel mainCards4 = new NewProjectPage(this);
		JPanel mainCards5 = new NotificationPage(this);
		
		this.center.add(mainCards1, GROUP);
		this.center.add(mainCards2, PROJECT);
		this.center.add(mainCards3, SEARCH_GROUP);
		this.center.add(mainCards4, SEARCH_PROJECT);
		this.center.add(mainCards5, NOTIFICATION);

        //Create the main window -- For the group menu 
		this.center.setVisible(true);
        this.add(this.center, BorderLayout.CENTER);
    
        // Load the cards
		comboBox.setSelectedIndex(0);
		comboBox.addItemListener(toGroup->{
			if(this.gui.getController() != null && this.currentUser != null) //Do not do it the first time (we do not have controller until the GUI is set up)
				this.setGroupListModel(this.getGui().getController().updateList(GeneralPage.GROUP));//Refresh the left list
			
			CardLayout cl = (CardLayout)(this.cards.getLayout()); // Get cards layout
			cl.show(cards, (String)toGroup.getItem());
			
			CardLayout cl2 = (CardLayout)(this.center.getLayout()); 
			cl2.show(this.center, (String)toGroup.getItem());
		});
		
		comboBox.setSelectedIndex(1);
		comboBox.addItemListener(toProject->{
			if(this.gui.getController() != null && this.currentUser != null)//Refresh the left list
				this.setProjectListModel(this.getGui().getController().updateList(GeneralPage.PROJECT));
			
			CardLayout cl = (CardLayout)(this.cards.getLayout()); // Get cards layout
			cl.show(cards, (String)toProject.getItem());
			
			CardLayout cl2 = (CardLayout)(this.center.getLayout()); 
			cl2.show(this.center, (String)toProject.getItem());
		});
		
		comboBox.setSelectedIndex(2);
		comboBox.addItemListener(srcGroup->{
			if(this.gui.getController() != null && this.currentUser != null)//Refresh the left list
				this.setGroupListModel(this.getGui().getController().updateList(GeneralPage.SEARCH_GROUP));
			
			CardLayout cl = (CardLayout)(this.cards.getLayout()); // Get cards layout
			cl.show(cards, (String)srcGroup.getItem());

			CardLayout cl2 = (CardLayout)(this.center.getLayout()); 
			cl2.show(this.center, (String)srcGroup.getItem());
		});
		
		comboBox.setSelectedIndex(3);
		comboBox.addItemListener(srcProject->{
			if(this.gui.getController() != null && this.currentUser != null)//Refresh the left list
				this.setSearchProjectListModel(this.getGui().getController().updateList(GeneralPage.SEARCH_PROJECT));
			
			CardLayout cl = (CardLayout)(this.cards.getLayout()); // Get cards layout
			cl.show(cards, (String)srcProject.getItem());
			
			CardLayout cl2 = (CardLayout)(this.center.getLayout()); 
			cl2.show(this.center, (String)srcProject.getItem());
		});
		
		comboBox.setSelectedIndex(4);
		comboBox.addItemListener(notifications->{
			if(this.gui.getController() != null && this.currentUser != null)//Refresh the left list
				this.setNotificationListModel(this.getGui().getController().updateList(GeneralPage.NOTIFICATION));
			
			CardLayout cl = (CardLayout)(this.cards.getLayout()); // Get cards layout
			cl.show(cards, (String)notifications.getItem());
			
			CardLayout cl2 = (CardLayout)(this.center.getLayout()); 
			cl2.show(this.center, (String)notifications.getItem());
		});
		
		comboBox.setSelectedIndex(0); //By default it selects first item (Start the app in the group menu)
        
	}
	
	/**
	* This method responses to the different actions made by the user
	* 
	* @param e, the action performed by the user
	*/
	@Override
	public void actionPerformed(ActionEvent e) {//  Return to the log in
		if(e.getSource() == this.out) {
			
			this.setVisible(false);
			this.gui.removeJPanel(this);
			
			InitialPage initialPage = this.gui.getInitialPage();
			initialPage.setVisible(true);
			this.gui.addJPanel(initialPage, BorderLayout.CENTER);
			
			LogInPage logInPage = this.gui.getLogInPage();
			logInPage.setVisible(true);
			this.gui.addJPanel(logInPage, BorderLayout.EAST);
			
			JPanel north = this.gui.getNorth();
			north.setVisible(true);
			this.gui.addJPanel(north, BorderLayout.NORTH);
			
			this.gui.getAdministratorPage().setUsersModel(this.gui.getController().updateList(AdministratorPage.ACTIVEUSERS));
			this.gui.getAdministratorPage().setUserPendingModel(this.gui.getController().updateList(AdministratorPage.PENDINGUSERS));
			this.gui.getAdministratorPage().setUserBannedListModel(this.gui.getController().updateList(AdministratorPage.BANNEDUSERS));
			this.gui.getAdministratorPage().setListProjectListModel(this.gui.getController().updateList(AdministratorPage.PENDINGPROJECTS));
			
			this.gui.setGeneralPage(null); //Reset the general page
			
		}else if(e.getSource() == this.notifTurnOn) {
			MySystem system = MySystem.getInstance();
			
			User currentUser = system.getSingleUser(this.currentUser);
			
			if(currentUser.getTurnOnNotification()) {
				JOptionPane.showMessageDialog(this, "You already have the notifications turned on",
						"Turn on notifications - Error", JOptionPane.ERROR_MESSAGE);
			}else {
				this.gui.getController().setTurnOnNotificationsController(currentUser, true);
				JOptionPane.showMessageDialog(this, "You have turned on your notifications succesfully",
						"Turn on notifications - Success", JOptionPane.INFORMATION_MESSAGE);
			}
					
		}else if(e.getSource() == this.notifTurnOff) {
			MySystem system = MySystem.getInstance();
			
			User currentUser = system.getSingleUser(this.currentUser);
			
			if(!currentUser.getTurnOnNotification()) {
				JOptionPane.showMessageDialog(this, "You already have the notifications turned off",
						"Turn off notifications - Error", JOptionPane.ERROR_MESSAGE);
			}else {
				this.gui.getController().setTurnOnNotificationsController(currentUser, false);
				JOptionPane.showMessageDialog(this, "You have turned off your notifications succesfully",
						"Turn off notifications - Success", JOptionPane.INFORMATION_MESSAGE);
			}
		}
	}

	/**
	* This method sets the right upper part with the user nickname
	* that has logged in
	* 
	* @param currentUser, nickname of the user who has logged in
	*/
	public void setCurrentUser(String currentUser) {
		this.currentUser = currentUser;
		this.userLabel.setText("Your account: " + this.currentUser);
	}
	
	/**
	* @return gui, the Graphical User Interface
	*/
	public Gui getGui() {
		return gui;
	}
	
	/**
	* @return the nickname of the current user
	*/
	public String getCurrentUser() {
		return this.currentUser;
	}
	
	/**
	* @return the JList with the voted projects of the current user
	*/
	public JList<String> getProjectList() {
		return projectList;
	}
	
	/**
	* @return the JList with all the groups and subgroups of the system
	*/
	public JList<String> getSearchGroupList() {
		return searchGroupList;
	}

	/**
	* @return the JList with the all the admitted and not expired projects
	* in the system
	*/
	public JList<String> getSearchProjectList() {
		return searchProjectList;
	}
	
	/**
	* @return the JList with all the groups and subgroups the current user is in
	*/
	public JList<String> getGroupList() {
		return groupList;
	}
	
	/**
	* @return the JList with all the current user notifications
	*/
	public JList<String> getNotificationList() {
		return notificationList;
	}
	
	/**
	* This method updates the list of groups and subgroups the current user is in
	* 
	* @param m the list to be updated
	*/
	public void setGroupListModel(DefaultListModel<String> m) {
		this.groupList.setModel(m);
	}
	
	/**
	* This method updates the list of voted projects of the current user
	* 
	* @param m the list to be updated
	*/
	public void setProjectListModel(DefaultListModel<String> m) {
		this.projectList.setModel(m);
	}
	
	/**
	* This method updates the list of groups and subgroups the current user is in
	* 
	* @param m the list to be updated
	*/
	public void setSearchGroupListModel(DefaultListModel<String> m) {
		this.searchGroupList.setModel(m);
	}
	
	/**
	* This method updates the list of all the admitted and not expired projects
	* 
	* @param m the list to be updated
	*/
	public void setSearchProjectListModel(DefaultListModel<String> m) {
		this.searchProjectList.setModel(m);
	}
	
	/**
	* This method updates the list with all the current user notifications
	* 
	* @param m the list to be updated
	*/
	public void setNotificationListModel(DefaultListModel<String> m) {
		this.notificationList.setModel(m);
	}
	
}
