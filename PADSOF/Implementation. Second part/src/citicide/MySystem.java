package citicide;

import java.util.*;
import es.uam.eps.sadp.grants.*;
import modifiableDates.ModifiableDate;
import java.time.LocalDate;
import java.io.*;

/**
* This is the MySystem singleton class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial MySystem class implements Serializable
*/
public class MySystem implements Serializable {

	private static final long serialVersionUID = 1L;
	private static MySystem INSTANCE;
	private Administrator admin;
	private ArrayList<User> users;
	private ArrayList<Project> projects;
	private ArrayList<Notification> notifications ;
	private ArrayList<Group> groups;
	private ArrayList<String> districts;
	private LocalDate date;
	
	/**
	* This is the constructor method
	* 
	* @throws FileNotFoundException in case the districts file could not be found. 
	* This should be placed in the project folder and named "District_names.txt"
	* @throws IOException in case there is an error reading the file
	*/
	private MySystem() throws FileNotFoundException, IOException { 
		admin = new Administrator("Juan", "uameps"); /*The password of the administrator will be provided to the
																user in question. It could be change in the future by this user*/
		users = new ArrayList<User>();
		projects = new ArrayList<Project>();
		groups = new ArrayList<Group>();
		districts = new ArrayList<String>();
		notifications = new ArrayList<>();
		this.date = ModifiableDate.getModifiableDate();		// Initialize the system Time
		
		FileInputStream stream = new FileInputStream("District_names.txt");
		InputStreamReader reader = new InputStreamReader(stream);
		BufferedReader buffer = new BufferedReader(reader);
		
		String line;
		while((line = buffer.readLine()) != null) { 
			this.districts.add(line);
		}
		
		buffer.close();
		CCGG.getGateway().setDate(this.date);
		
	}
	
	/**
	* This method creates, if it is not already done and return the instance
	* 
	* @throws FileNotFoundException in case the districts file could not be found. 
	* This should be placed in the project folder and named "District_names.txt"
	* @throws IOException in case there is an error reading the file
	*/
	public static MySystem getInstance(){
		if(INSTANCE == null) {
			try {
				MySystem.INSTANCE = Serialization.serializationRead();
			}catch(FileNotFoundException sys_1){	
				
				
			}catch(IOException sys_2){
				//System.out.println(sys_2);
			}
			
			if(INSTANCE == null) {
				try {
					INSTANCE = new MySystem();	
				}catch(FileNotFoundException sys_3){
					//Do nothing just continue if the Districts file does not exits
				}catch(IOException sys_4){
					//Do nothing just continue if the Districts file does not exits
				}
			}
		
		}
		
		// if it already exits just works as a getter
		return INSTANCE;
	}

	/**
	* This method sets the administrator of MySystem
	*
	* @param admin the new administrator to be set
	*/
	public void setAdmin(Administrator admin) {
		this.admin = admin;
	}

	/**
	* This method adds a user to MySystem
	*
	* @param user, a new user to be added
	*/
	public void addUser(User user) {
		users.add(user);
	}
	
	/**
	* This method adds a project to MySystem
	*
	* @param project, a new project to be added
	*/
	public void addProject(Project project) {
		projects.add(project);
	}
	
	/**
	* This method adds a notification to MySystem
	*
	* @param notification, a new notification to be added
	*/
	public void addNotification(Notification notification) {
		notifications.add(notification);
	}
	
	/**
	* This method adds a group to MySystem
	*
	* @param group, a new group to be added
	*/
	public void addGroup(Group group) {
		groups.add(group);
	}
	
	/**
	* This method sets the districts ArrayList of MySystem
	*
	* @param districts the new districts to be set
	*/
	public void setDistricts(ArrayList<String> districts) {
		this.districts = districts;
	}
	
	/**
	* 
	* @return the administrator of the application 
	*/
	public Administrator getAdmin() {
		return admin;
	}
	
	/**
	* This method returns the string passed as argument if it exists
	* in the districts ArrayList, if not null returned
	* 
	* @param district, district name that is desired be returned
	* @return the string passed as argument if it exists in the 
	* districts ArrayList, if not null
	*/
	public String getSingleDistrict(String district) {
		for(String d: districts) {
			if(district.equals(d))
				return d;
		}
		
		return null;
	}
	
	/**
	* 
	* @return the array of all the districts read from the file 
	*/
	public ArrayList<String> getDistricts() {
		return districts;
	}

	/**
	* 
	* @return the array of all the users of the application 
	*/
	public ArrayList<User> getUsers(){
		return users;
	}
	
	/**
	* 
	* @return the array of all the notification of the application 
	*/
	public ArrayList<Notification> getNotification(){
		return notifications;
	}
	
	/**
    * This method iterates through the array of users and returns
    * the user whose nickname is equal to the one passed as parameter,
    * or null otherwise 
	* 
	* @param nickname of the user that we want to get
	* @return user with nickname equal to the one passed as parameter, or
	* null if this user does not exist
	*/
	public User getSingleUser(String nickname) {
		for(User u: this.users) {
			if(u.getNickname().equals(nickname)) {
				return u;
			}
		}
		
		return null;
	}
	
	/**
	 * This method iterates through the array of notifications and returns
	 * the notification whose title is equal to the one passed as parameter,
	 * or null otherwise 
	 * 
	 * @param not of the notification that we want to get
	 * @return notification whose title is equal to the one passed as parameter, or
	 * null if this user does not exist
	 */
	public Notification getSingleNotification(String not) {
		for(Notification n: this.notifications) {
			if(n.getTitle().contentEquals(not)) {
				return n;
			}
		}

		return null;
	}

	/**
	* 
	* @return the array of all the projects of the application 
	*/
	public ArrayList<Project> getProjects() {
		return projects;
	}
	
	/**
	 * This method iterates through the array of projects and returns
	 * the project whose title is equal to the one passed as parameter,
	 * or null otherwise 
	 * 
	 * @param projectTitle of the project that we want to get
	 * @return project with title equal to the one passed as parameter, or
	 * null if this project does not exist
	 */
	public Project getSingleProject(String projectTitle) {
		for(Project p: this.projects) {
			if(p.getTitle().equals(projectTitle)) {
				return p;
			}
		}

		return null;
	}

	/**
	* 
	* @return the array of all the groups of the application 
	*/
	public ArrayList<Group> getGroups() {
		return groups;
	}
	
	/**
	 * This method iterates through the array of groups and returns
	 * the group whose name is equal to the one passed as parameter,
	 * or null otherwise 
	 * 
	 * @param groupName of the group that we want to get
	 * @return group with name equal to the one passed as parameter, or
	 * null if this group does not exist
	 */
	public Group getSingleGroup(String groupName) {
		for(Group g: this.groups) {
			if(g.getName().equals(groupName)) {
				return g;
			}
		}

		return null;
	}
	
	/**
	* 
	* @param date the new local date for the system
	*/
	public void setDate(LocalDate date) {
	  this.date = date;
	}
	
	/**
	* 
	* @return the local date of the system
	*/
	public LocalDate getDate() {
		return this.date;
	}
	
	/**
	* This method checks whether  the information of user u is up to date,
	* depending if we want to add or remove information from system we need to
	* specified it with its parameters
	* 
	* @param u, the user to be checked
	*/
	public void update(User u) {
			
		if(!users.contains(u)) {
			users.add(u);
		}
		return;
	}
	
	/**
	* This method checks whether  the information of group g is up to date,
	* depending if we want to add or remove information from system we need to
	* specified it with its parameters
	* 
	* @param g, the group to be checked
	*/
	public void update(Group g) {
		
		if(!this.groups.contains(g)) {
			this.groups.add(g);
		}
		return;
	}
	
	/**
	* This method checks whether the information of project p is up to date,
	* depending if we want to add or remove information from system we need to
	* specified it with its parameters
	* 
	* @param p, the project to be checked
	*/
	public void update(Project p) {
		
		if(!this.projects.contains(p)) {
			this.projects.add(p);
		}
		return;
	}
	
	/**
	* This method returns true or false depending on
	* the user's nickname, password and private status
	* 
	* @param nickname of the user who wants to log in
	* @param password of the user who wants to log in
	* 
	* @return true if there is a user with the given nickname, the password was correct
	* and its private status is "accepted"; or false otherwise
	*/
	public boolean logIn(String nickname, String password) {
		for(User u: users) {
			if(u.getNickname().equals(nickname)) {
				if((u.getUserPrivateStatus() == UserPrivateStatus.ACCEPTED.getValue()) && u.getPassword().equals(password)) {
					return true;
				}
			}
		}
		return false;
	}
	
	/**
	* This method returns true or false depending on
	* the administrator's nickname and password
	* 
	* @param nickname of the administrator
	* @param password of the administrator
	* 
	* @return true if the nickname and password of the administrator are correct,
	* false otherwise
	*/
	public boolean logInAdmin(String nickname, String password) {
		if (admin.getPassword().equals(password) && admin.getNickname().equals(nickname)) {
			return true;
		}
		return false;
	}
	
	/**
	* This method returns true or false depending on the length of the ID
	* and the nickname
	* 
	* @param name, nickname of the new user
	* @param iD, the user ID
	* @param passwd, the password of the new user
	* 
	* @return false if the ID does not have 9 character or the name is already user,
	* true otherwise
	*/
	public boolean logUp(String name, String iD, String passwd) {
		User user = null;
		
		try{
			user = new User(name, iD, passwd, UserPublicStatus.OFFLINE);
		}catch(IllegalArgumentException e){
			return false;
		}
		
		return true;
	}
	
	/**This method calls the serialization class to load all the saved
	* data of the application
	* 
	* @throws FileNotFoundException when the file to work with is not found
	* @throws IOException in case there is an error reading, writing the file
	*/
	public void read() throws FileNotFoundException, IOException {
		Serialization.serializationRead();
	}

	/**This method calls the serialization class to write all the data of
	* the application
	* 
	* @throws FileNotFoundException when the file to work with is not found
	* @throws IOException in case there is an error reading, writing the file
	*/
	public void write() throws FileNotFoundException, IOException {
		Serialization.serializationWrite();
	}
	
}
