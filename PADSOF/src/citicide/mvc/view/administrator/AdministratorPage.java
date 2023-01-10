package citicide.mvc.view.administrator;

import java.awt.*;
import java.awt.event.*;
import java.util.*;

import javax.swing.*;

import citicide.*;
import citicide.mvc.view.Gui;
import citicide.mvc.view.*;

/**
* This is the AdministratorPage class from the citicide.mvc.view.administrator package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class AdministratorPage extends JPanel implements ActionListener{

	private static final long serialVersionUID = 1L;
	
	public final static String ACTIVEUSERS = "Active Users";
	public final static String PENDINGUSERS = "Pending Users";
	public final static String BANNEDUSERS = "Banned Users";
	public final static String PENDINGPROJECTS = "Pending Projects";
	
	private final String[] menu = {ACTIVEUSERS, PENDINGUSERS, BANNEDUSERS , PENDINGPROJECTS};
	private JComboBox<String> comboBox;
	private JButton button;
	private JPanel cards;
	
	private JPanel west;
	private JPanel north;
	private JPanel center;
	
	private JList <String> listUser;
	private JList <String> listUserPending;
	private JList <String> listUserBanned;
	private JList <String> listProject;
	
	private ArrayList<User> users;
	private ArrayList<Project> projects;
	
	private Gui gui;

	/**
	* This is the constructor class of AdministratorPage
	* 
	* @param gui page the class administrator is using
	*/
	public AdministratorPage(Gui gui) {
		this.gui = gui;
		this.setLayout(new BorderLayout());
		
        //LogOut button
        this.north = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        button = new JButton("LogOut");
        button.addActionListener(this);
        this.north.setBackground(gui.getColorLogoLight());
        this.north.add(button);
        this.add(this.north, BorderLayout.NORTH);
		
        //Creates the cards
        JPanel cards1 = new JPanel(new BorderLayout());
		JPanel cards2 = new JPanel(new BorderLayout());
		JPanel cards3 = new JPanel(new BorderLayout());
		JPanel cards4 = new JPanel(new BorderLayout());
		this.cards = new JPanel(new CardLayout());
		
		this.cards.add(cards1, ACTIVEUSERS);
		this.cards.add(cards2, PENDINGUSERS);
		this.cards.add(cards3, BANNEDUSERS);
		this.cards.add(cards4, PENDINGPROJECTS);
		
		this.comboBox = new JComboBox<String>(this.menu);
     
		MySystem system = MySystem.getInstance();
		
		try {
			users = system.getUsers();
	        String [] stringUsers= new String[users.size()];
	        
	        int i = 0;
	        for(User u: users) {
	        	if(u.getSituation().equals(UserPrivateStatus.ACCEPTED)){
	        		stringUsers[i] = u.getNickname();
	        		i++;
	        	}
	        }
	        
	        //Creates the list
	        listUser = new JList<>(stringUsers);// this will be not visible
	        listUser.setVisible(true); // Do not show it until a menu is selected
	        cards1.add(listUser, BorderLayout.CENTER);
		}catch(NullPointerException e) {}
		
        
		try {
	        ArrayList<User> usersPending = system.getUsers();
	        String [] stringPendingUsers= new String[usersPending.size()];
	        int i = 0;
	        for(User u: usersPending) {
	        	if(u.getSituation().equals(UserPrivateStatus.PENDING)){
	        		stringPendingUsers[i] = u.getNickname();
	        		i++;
	        	}
	        }
	        
	        //Creates the list
	        listUserPending = new JList<>(stringPendingUsers);// this will be not visible
	        listUserPending.setVisible(true); // Do not show it until a menu is selected
	        cards2.add(listUserPending, BorderLayout.CENTER);
		}catch(NullPointerException e) {}

		try {
	        ArrayList<User> usersBanned = system.getUsers();
	        String [] stringBannedUsers= new String[usersBanned.size()];
	        
	        int i = 0;
	        for(User u: usersBanned) {
	        	if(u.getSituation().equals(UserPrivateStatus.BANNED)){
	        		stringBannedUsers[i] = u.getNickname();
	        		i++;
	        	}
	        }
	        
	        //Creates the list
	        listUserBanned = new JList<>(stringBannedUsers);// this will be not visible
	        listUserBanned.setVisible(true); // Do not show it until a menu is selected
	        cards3.add(listUserBanned, BorderLayout.CENTER);
		}catch(NullPointerException e) {}

		try {
			projects = system.getProjects();
	        String [] stringproject= new String[projects.size()];
	        
	        int i = 0;
	        for(Project p: projects) {
	        	if(p.getCheckAdmin() == false){
	        		stringproject[i] = p.getTitle();
	        		i++;
	        	}
	        }
	        
	        //Creates the list
	        listProject = new JList<>(stringproject);// this will be not visible
	        listProject.setVisible(true); // Do not show it until a menu is selected
	        cards4.add(listProject, BorderLayout.CENTER);
		} catch (NullPointerException e){}
		
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
		JPanel mainCards1 = new ActiveUsersPage(this);
		JPanel mainCards2 = new PendingUsersPage(this);
		JPanel mainCards3 = new BannedUsersPage(this);
		JPanel mainCards4 = new PendingProjectPage(this);
		
		this.center.add(mainCards1, ACTIVEUSERS);
		this.center.add(mainCards2, PENDINGUSERS);
		this.center.add(mainCards3, BANNEDUSERS);
		this.center.add(mainCards4, PENDINGPROJECTS);

        //Create the main window -- For the group menu 
		this.center.setVisible(true);
        this.add(this.center, BorderLayout.CENTER);
    
        //creates the actions of the comboBox
        comboBox.setSelectedIndex(0);
        comboBox.addItemListener(toActiveUser->{
			if(this.gui.getController() != null)//Refresh the left list
				this.listUser.setModel(this.getGui().getController().updateList(AdministratorPage.ACTIVEUSERS));
			
			CardLayout cl = (CardLayout)(this.cards.getLayout()); // Get cards layout
			cl.show(cards, (String)toActiveUser.getItem());		
			
			CardLayout cl1 = (CardLayout)(this.center.getLayout()); // Get cards layout
			cl1.show(this.center, (String)toActiveUser.getItem());
		});
        
		comboBox.setSelectedIndex(1);
		comboBox.addItemListener(toPendingUser->{
			if(this.gui.getController() != null)//Refresh the left list
				this.listUserPending.setModel(this.getGui().getController().updateList(AdministratorPage.PENDINGUSERS));
			CardLayout c2 = (CardLayout)(this.cards.getLayout()); // Get cards layout
			c2.show(cards, (String)toPendingUser.getItem());	
			
			CardLayout cl2 = (CardLayout)(this.center.getLayout()); // Get cards layout
			cl2.show(this.center, (String)toPendingUser.getItem());
		});
		
		comboBox.setSelectedIndex(2);
		comboBox.addItemListener(toBannedUser->{
			if(this.gui.getController() != null)//Refresh the left list
				this.listUserBanned.setModel(this.getGui().getController().updateList(AdministratorPage.BANNEDUSERS));
			
			CardLayout c3 = (CardLayout)(this.cards.getLayout()); // Get cards layout
			c3.show(cards, (String)toBannedUser.getItem());	
			
			CardLayout cl3 = (CardLayout)(this.center.getLayout()); // Get cards layout
			cl3.show(this.center, (String)toBannedUser.getItem());
		});
		
		comboBox.setSelectedIndex(3);
		comboBox.addItemListener(toPendingProject->{
			if(this.gui.getController() != null)//Refresh the left list
				this.listProject.setModel(this.getGui().getController().updateList(AdministratorPage.PENDINGPROJECTS));
			
			CardLayout c4 = (CardLayout)(this.cards.getLayout()); // Get cards layout
			c4.show(cards, (String)toPendingProject.getItem());	   
			
			CardLayout cl4 = (CardLayout)(this.center.getLayout()); // Get cards layout
			cl4.show(this.center, (String)toPendingProject.getItem());
		});
		
        comboBox.setSelectedIndex(0); //the combobox starts in the 0 component
	}
	
	
	/**
	 * This method allows the administrator to log out to the 
	 * initial page
	 * 
	 * @param e action of the window
	 * */
	@Override
	public void actionPerformed(ActionEvent e) {//  Return to the log in
		if(e.getSource() == button) {
			
			this.setVisible(false);
			this.gui.removeJPanel(this);
			
			InitialPage initialPage= this.gui.getInitialPage();
			initialPage.setVisible(true);
			this.gui.addJPanel(initialPage, BorderLayout.CENTER);
			
			LogInPage logInPage = this.gui.getLogInPage();
			logInPage.setVisible(true);
			this.gui.addJPanel(logInPage, BorderLayout.EAST);
			
			JPanel north = this.gui.getNorth();
			north.setVisible(true);
			this.gui.addJPanel(north, BorderLayout.NORTH);
			
		}	
	}
	
	/**
	 * @return listUser a list of active users
	 * */
	public JList <String> getListuser(){
		return this.listUser;
	}
	
	/**
	 * @return listUserPending a list of pending users
	 * */
	public JList <String> getListuserpending(){
		return this.listUserPending;
	}
	
	/**
	 * @return listUserBanned a list of banned users
	 * */
	public JList <String> getListuserbanned(){
		return this.listUserBanned;
	}
	
	/**
	 * @return listProject a list of pending projects
	 * */
	public JList <String> getListproject(){
		return this.listProject;
	}
	
	/**
	 * @return users a list of users of all the application
	 * */
	public ArrayList <User> getUsers(){
		return this.users;
	}
	
	/**
	 * @return projects a list of projects of all the application
	 * */
	public ArrayList <Project> getprojects(){
		return this.projects;
	}
	
	/**
	 * @return gui the gui the class administrator page is using
	 * */
	public Gui getGui() {
		return this.gui;
	}
	
	/**
	 * 
	 * 
	 * @param m
	 * */
	public void setUsersModel(DefaultListModel<String> m) {
		this.listUser.setModel(m);
	}
	
	/**
	 * 
	 * 
	 * @param m
	 * */
	public void setUserPendingModel(DefaultListModel<String> m) {
		this.listUserPending.setModel(m);
	}
	
	/**
	 * 
	 * 
	 * @param m
	 * */
	public void setUserBannedListModel(DefaultListModel<String> m) {
		this.listUserBanned.setModel(m);
	}
	
	/**
	 * 
	 * 
	 * @param m
	 * */
	public void setListProjectListModel(DefaultListModel<String> m) {
		this.listProject.setModel(m);
	}
}
