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
	private ArrayList<Group> groups;
	private ArrayList<String> districts;
	private LocalDate date;
	
	/**
	* This is the constructor class
	* 
	* @throws FileNotFoundException in case the districts file could not be found. 
	* This should be placed in the project folder and named "District_names.txt"
	* @throws IOException in case there is an error reading the file
	*/
	private MySystem() throws FileNotFoundException, IOException { 
		admin = new Administrator("Juan", "F58d12"); /*The password of the administrator will be provided to the
																user in question. It could be change in the future by this user*/
		users = new ArrayList<User>();
		projects = new ArrayList<Project>();
		groups = new ArrayList<Group>();
		districts = new ArrayList<String>();
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
	public static MySystem getInstance() throws FileNotFoundException, IOException {
		if(INSTANCE == null) INSTANCE = new MySystem();
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
	* @return the array of all the projects of the application 
	*/
	public ArrayList<Project> getProjects() {
		return projects;
	}

	/**
	* 
	* @return the array of all the groups of the application 
	*/
	public ArrayList<Group> getGroups() {
		return groups;
	}
	
	/**
	* 
	* @return the local date of the system
	*/
	public LocalDate getDate() {
		return this.date;
	}
	
	/**
	* This method checks whether  the information of group g is up to date,
	* depending if we want to add or remove information from system we need to
	* specified it with its parameters
	* 
	* @param u, the user to be checked
	*/
	public void update(User u) {
			
		if(!this.users.contains(u)) {
			this.users.add(u);
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
	* This method returns true or false whether true or false depending on
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
