package citicide.mvc.controller;

import java.awt.*;
import java.io.IOException;
import java.util.*;
import javax.swing.*;
import citicide.*;
import citicide.mvc.view.GeneralPage;
import citicide.mvc.view.Gui;
import citicide.mvc.view.administrator.AdministratorPage;
import es.uam.eps.sadp.grants.InvalidIDException;
import es.uam.eps.sadp.grants.InvalidRequestException;

/**
* This is the Controller class from the citicide.mvc.controller package
* This is class facilitates the creation of objects according to the
* view's necessities
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class Controller {	
	private Gui gui;
	private MySystem app;
	
	/**
	* This is the constructor method
	*  
	* @param gui, the Graphical User Interface
	* @param app, the MySystem class of the application
	*/
	public Controller(Gui gui, MySystem app) {
		this.gui = gui;
		this.app = app;
	}
	
	/**
	* This method notifies sends a notification of the users passed as parameter 
	*  
	* @param title of the notification
	* @param body of the notification
	* @param to, array list with all the users that the notification is going
	* to be sent
	*/
	public void notify(String title, String body, ArrayList<User> to) {
		Notification newNot = new Notification(title, body, to);
		newNot.sendNotification();
		return;
	}
	
	/**
	* This method notifies sends a notification of the users passed as parameter 
	*  
	* @param title of the notification
	* @param body of the notification
	* @param to, array list with all the users that the notification is going
	* to be sent if they have the notifications turned on
	*/
	public void notify(String title, String body, ArrayList<User> to, Boolean goThrough) {
		Notification newNot = new Notification(title, body, to);
		newNot.sendNotification(goThrough);
		return;
	}
	
	/**
	* This method calls the logIn method of the system 
	*  
	* @param name of the user
	* @param passwd, the password of the user
	* 
	* @return true if all the information is correct and the user can log in,
	* false otherwise
	*/
	public boolean logIn(String name, String passwd) {
		boolean bool = this.app.logIn(name, passwd);
		this.gui.logInResult(bool);
		return bool;
	}
	
	/**
	* This method calls the logInAdmin method of the system 
	*  
	* @param name of the administrator
	* @param passwd, the password of the administrator
	* 
	* @return true if all the information is correct and the administrator can log in,
	* false otherwise
	*/
	public boolean logInAdmin(String name, String passwd) {
		boolean bool = this.app.logInAdmin(name, passwd);
		this.gui.logInAdminResult(bool);
		return bool;
	}
	
	/**
	* This method calls the logUp method of the system 
	*  
	* @param name of the new user
	* @param iD, the user ID
	* @param passwd, the password of the new user
	* @param passwd2, the password of the new user that must match with passwd
	* 
	* @return true if all the information is correct and the user can log up,
	* false otherwise
	*/
	public boolean logUp(String name, String iD, String passwd, String passwd2) {
		if(!passwd.equals(passwd2)) {
		    UIManager.put("OptionPane.message", Color.red);
			JOptionPane.showMessageDialog(gui, "Passwords do not match", "Log up - Error", JOptionPane.ERROR_MESSAGE);
			return false;
		}
		
		boolean bool = this.app.logUp(name, iD, passwd);
		if(bool == false){
			UIManager.put("OptionPane.message", Color.red);
			JOptionPane.showMessageDialog(gui, "This nickname is already used. Choose another one.", "Log up - Error", JOptionPane.ERROR_MESSAGE);
			return false;
		}
		
		return bool;
	}
	
	/**
	* This is a general update function that updates the list needed, which is
	* passed as parameter
	*  
	* @param which, the list that it is wanted to update
	* 
	* @return all the information that has the list we have updated
	*/
	public DefaultListModel<String> updateList(String which){
		MySystem system = MySystem.getInstance();
		User currentUser = null;
		
		try {
			currentUser = system.getSingleUser(this.gui.getGeneralPage().getCurrentUser());
		}catch(NullPointerException e) {
			//Do nothing
		}
		
        DefaultListModel<String> updateModel = new DefaultListModel<String>();
        
        if(which.contentEquals(GeneralPage.GROUP)) {
	        try {
	        	ArrayList<Group> userGroups = currentUser.getMembership(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(Group g: userGroups) {
	            	updateModel.addElement( g.getName() );
	            }
	            
	            if(updateModel.isEmpty()) updateModel.addElement("No groups in the system" );

	        }catch(NullPointerException e) {
	        	String groupNames ="Nothing to see here. Try to join a group";
	        	updateModel.addElement(groupNames);
	        	
	        }
        }else if (which.contentEquals(GeneralPage.PROJECT)) {
	        try {
	        	ArrayList<Project> userProjects = currentUser.getIndividualVote(); //Throws a NullPointerException if the user doesn't have groups
	            ArrayList<Group> aux = currentUser.getMembership();
	            
	        	
	            for(Project p: userProjects) {
	            	if(p.isAccepted() && !p.isExpired()) // Only show accepted project to the users
	            		updateModel.addElement( p.getTitle() );
	            }
	            
	            for(Group g: aux) {
	            	for(Project p: g.getGroupVote()) {
	            		if(!updateModel.contains(p))
	            			updateModel.addElement( p.getTitle() );
	            	}
	            }
	            
	            
	            if(updateModel.isEmpty()) updateModel.addElement("No projects in the system" );

	        }catch(NullPointerException e) {
	        	String p1 = "Nothing to see here. You do not "; 
	        	String p2 = "or wait until your group does it";
	        	updateModel.addElement(p1);
	        	updateModel.addElement(p2);
	        	
	        }
        }else if (which.contentEquals(GeneralPage.SEARCH_PROJECT)) {
	        try {
	        	ArrayList<Project> userProjects = system.getProjects(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(Project p: userProjects) {
	            	if(p.isAccepted() && !p.isExpired())
	            		updateModel.addElement( p.getTitle() );
	            }
	            
	            if(updateModel.isEmpty()) updateModel.addElement("No projects in the system" );

	        }catch(NullPointerException e) {
	        	String p1 = "No projects in the system"; 
	        	updateModel.addElement(p1);
	        }
	        
        }else if (which.contentEquals(GeneralPage.SEARCH_GROUP)) {
	        try {
	        	ArrayList<Group> userGroups = system.getGroups(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(Group g: userGroups) {
	            	updateModel.addElement( g.getName() );
	            }
	            
	            if(updateModel.isEmpty()) updateModel.addElement("No groups in the system" );

	        }catch(NullPointerException e) {
	        	String groupNames ="No groups in the system";
	        	updateModel.addElement(groupNames);
	        	
	        }
        }
        
        else if (which.contentEquals(AdministratorPage.ACTIVEUSERS)) {
	        try {
	        	ArrayList<User> user= system.getUsers(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(User u: user) {
	            	if (u.getSituation().equals(UserPrivateStatus.ACCEPTED))
	            	updateModel.addElement( u.getNickname() );
	            }

	        }catch(NullPointerException e) {
	        	String activeUsers="No active users at the moment";
	        	updateModel.addElement(activeUsers);
	        	
	        }
        } else if (which.contentEquals(AdministratorPage.PENDINGUSERS)) {
	        try {
	        	ArrayList<User> user= system.getUsers(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(User u: user) {
	            	if (u.getSituation().equals(UserPrivateStatus.PENDING))
	            	updateModel.addElement( u.getNickname() );
	            }

	        }catch(NullPointerException e) {
	        	String activeUsers="No pending users at the moment";
	        	updateModel.addElement(activeUsers);
	        	
	        }
        }else if (which.contentEquals(AdministratorPage.BANNEDUSERS)) {
	        try {
	        	ArrayList<User> user= system.getUsers(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(User u: user) {
	            	if (u.getSituation().equals(UserPrivateStatus.BANNED))
	            	updateModel.addElement( u.getNickname() );
	            }

	        }catch(NullPointerException e) {
	        	String activeUsers="No banned users at the moment";
	        	updateModel.addElement(activeUsers);
	        	
	        }
        }else if(which.contentEquals(GeneralPage.NOTIFICATION)){
	        try {
	        	ArrayList<Notification> myNots = currentUser.getNotification(); //Throws a NullPointerException if the user doesn't have groups
	            
	            for(Notification n: myNots) {
	            	updateModel.addElement( n.getTitle() );
	            }
	            
	            if(updateModel.isEmpty()) updateModel.addElement("You do not have notifications" );

	        }catch(NullPointerException e) {
	        	String empNot = "You do not have notifications";
	        	updateModel.addElement(empNot);
	        }
	    }else if (which.contentEquals(AdministratorPage.PENDINGPROJECTS)) {
	        try {
	        	ArrayList<Project> project= system.getProjects(); //Throws a NullPointerException if the project doesn't have groups
	            
	            for(Project p: project) {
	            	if (p.getCheckAdmin() == false)
	            		updateModel.addElement( p.getTitle() );
	            }
	        }catch(NullPointerException e) {
	        	String activeUsers="No panding projects at the moment";
	        	updateModel.addElement(activeUsers);
	        	
	        }
	      }
        
        return updateModel;
	}
	
	/**
	* This method calls the askForMoneyGranted method of the project passed as
	* parameter 
	*  
	* @param p, the project we want to ask for money
	* 
	* @return true if there was no error trying to communicate with the City Hall,
	* false otherwise
	*/
	public boolean askForMoneyGrantedController(Project p) {
		try {
			p.askForMoneyGranted();
		}catch(InvalidRequestException | IOException | InvalidIDException e) {
			return false;
		}
		
		return true;
	}
	
	/**
	* This method calls the setTurnOnNotification method of the user passed as
	* parameter 
	*  
	* @param u, the user that we want to change its notification flag
	* @param notification, the boolean value that it is wanted to be set in the
	* turnOnNotifcation variable of the user
	*/
	public void setTurnOnNotificationsController(User u, boolean notification) {
		u.setTurnOnNotification(notification);
	}
	
	/**
	* This method creates a new group
	*  
	* @param name of the group
	* @param manager of the group
	* @param desc, description of the group
	* 
	* @return the new group created
	*/
	public Group groupController(String name, User manager, String desc) throws IllegalArgumentException {
		return new Group(name, manager, desc);
	}
	
	/**
	* This method creates a new subgroup
	*  
	* @param name of the subgroup
	* @param manager of the subgroup
	* @param description, description of the subgroup
	* @param father, the group that is in the upper level of this one
	* 
	* @return the new subgroup created
	* @throws IllegalArgumentException when it has passed an illegal argument
	*/
	public Subgroup subgroupController(String name, User manager, String description, Group father) throws IllegalArgumentException {
		return new Subgroup(name, manager, description, father);
	}
	
	/**
	* This method creates a new infrastructure project
	*  
	* @param name the name of the project
	* @param proponent the nickname of the user who has created the project
	* @param description the description of the project with a maximum of 500 characters
	* @param moneyRequested the money that the proponent is asking for to accomplish the project
	* @param district the place where the project is going to be performed
	* @param picturePath path where the picture of the project can be found
	* 
	* @return the new infrastructure project created
	*/
	public InfrastructureProject InfrastructureProjectController(String name, String proponent, String description, Double moneyRequested, String district, String picturePath) {
		return new InfrastructureProject(name, proponent, description, moneyRequested, district, picturePath);
	}
	
	/**
	* This method creates a new social project
	*  
	* @param name the name of the project
	* @param proponent the nickname of the user who has created the project
	* @param description the description of the project with a maximum of 500 characters
	* @param moneyRequested the money that the proponent is asking for to accomplish the project
	* @param addressedTo the social group who is the target of the project
	* @param scope whether it is national or international
	* 
	* @return the new social project created
	*/
	public SocialProject socialProjectController(String name, String proponent, String description, Double moneyRequested, String addressedTo, Scope scope) {
		return new SocialProject(name, proponent, description, moneyRequested, addressedTo, scope);
	}
}
