package citicide;

import java.io.Serializable;
import java.util.*;

/**
* This is the Group class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Group class implements Serializable
*/
public class Group implements Serializable{

	private static final long serialVersionUID = 1L;
	private static final int MAX_DESC = 500;
	private static final int MAX_NAME = 25;
	
	protected String name;
	protected long level = 0;
	protected Group father = null; //Null if this group it is not a subgroup
	protected User manager;
	protected String description;
	protected final long id;
	protected ArrayList<User> groupUsers = new ArrayList<User>();
	protected ArrayList<Project> groupVote = new ArrayList<Project>();
	protected ArrayList<Subgroup> subgroups = new ArrayList<Subgroup>();
	
	/**
	* This is the constructor method of group
	* 
	* @param name, string with the name of the group
	* @param manager, Type user, manager of a group
	* @param desc, string with the description of a group
	* 
	* @throws IllegalArgumentException, if the name or description are too long
	*/
	public Group (String name, User manager, String desc) throws IllegalArgumentException{

		if (name.length() > MAX_NAME) throw new IllegalArgumentException("Name too long.");
		if (desc.length() > MAX_DESC) throw new IllegalArgumentException("Description too long.");
		
		this.name = name;
		this.manager = manager;
		this.description = desc;
		this.id = this.name.hashCode() + this.manager.hashCode();
		this.groupUsers.add(manager);
		
		// Add it to the system
		MySystem sys = MySystem.getInstance();
		sys.addGroup(this);
		manager.update(this);
	}
	
	/**
	* @return name, string with the name of the group
	*/
	public String getName() {
		return name;
	}
	
	/**
	* @return level, 0 if it is no subgroup, number of the subgroup otherwise
	*/
	public long getLevel() {
		return this.level;
	}
	
	/**
	* @return father, the group above this one if it is a subgroup, null otherwise
	*/
	public Group getFather() {
		return this.father;
	}
	
	/**
	* @return manager, user with the manager of the group
	*/	
	public User getManager() {
		return manager;
	}
	
	/**
	* This method checks if the name of a user passed as parameter
	* is the manager of the group
	* 
	* @return true if it is the manager of the group, false otherwise
	*/	
	public boolean isManager(String manager) {
		return this.manager.getNickname().equals(manager);
	}
	
	/**
	* @return description, string with the description of the group
	*/
	public String getDescription() {

		return description;
	}
	
	/**
	* @return groupUser, ArrayList of type User with the name
	* of the users that are in the group 
	*/
	public ArrayList<User> getArrayUsers() {
		return this.groupUsers;
	}
	
	/**
	* @return groupUsers(i), User that is in the i-th
	* place of the array groupUsers
	*/
	public User getNthUser(int i) {
		return this.groupUsers.get(i);
	}
	
	/**
	* This method returns whether a user is member of the group or not 
	* 
	* @ user that we want to check if it is member
	* @return true if the user is member of the group, false otherwise
	*/
	public boolean isMember(User user) {
		for(User u: groupUsers) {
			if(u.equals(user))
				return true;
		}
		
		return false;
	}
	
	/**
	* @return groupVote, ArrayList of type Project with the name
	* of the projects the group voted 
	*/
	public ArrayList<Project> getGroupVote(){
		return this.groupVote;
	}
	
	/**
	* @return groupVote(i), Project that is in the i-th
	* place of the array groupVote
	*/
	public Project getNthGroupVote(int i) {
		return this.groupVote.get(i);
	}
	
	/**
	* @return groupVote, ArrayList of type Project with the name
	* of the projects the group voted 
	*/
	public ArrayList<Subgroup> getSubgroup(){
		return this.subgroups;
	}
	
	/**
	 * When a user wants to join a group, he calls this
	 * function.
	 * It adds the User to the arrayList "groupUser", 
	 * adds a user to the number of users of the group
	 * and adds a vote to all the projects the group
	 * voted.
	 * 
	 * @param user that joins the group
	 * @return false if the user is already in the group, true otherwise
	 */
	public boolean joinGroup(User user) {

		if(this.groupUsers.contains(user)) return false;
		
		this.groupUsers.add(user);
		user.update(this);

		for(Project p: this.groupVote) {
			if(!p.checkVotes(user, this)) { //if the user is not in any group that had voted for this project
				if(!p.individualVoters.contains(user)) { // if the user has not vote as individual
					p.setPopularityReport(p.getPopularityReport()+1);
				}
			}
		}
		
		return true;
	}
	
	/**
	* When a user wants to leave a group, he calls this
	* function.
	* It removes the User from the arrayList "groupUser", 
	* removes the user from the number of users of the group
	* and removes a vote from all the projects the group voted.
	* 
	* @param user that leaves the group
	* @return false, if the user passed as parameter is the manager of 
	* the group, the this user will not leave the group, true otherwise 
	*/
	public boolean leaveGroup (User user) {
		if(user.equals(this.manager)) {
			return false;
		}
		
		for(Project p: this.groupVote) {
			p.unvoteProject(this, user);
		}
		
		groupUsers.remove(user);
		user.getMembership().remove(this);
		
		return true;
	}
	
	/**
	* When a group votes a project it calls this function
	* and it adds that project to the ArrayList "groupVote"
	* 
	* @param project that the group is going to vote
	*/
	public void voteProjectAsGroup(Project project) {
		this.groupVote.add(project);
		project.voteGroup(this); // Includes update
	}
	
	/**
	* This method returns the affinity report calculated by the following 
	* formula: (N12+N21)/(N1+N2). Where N12 are the number projects created 
	* by group 1 and supported by group 2; N21, the same but the other way around; and N1
	* and N2 the number of projects created by group 1 and 2, respectively.  
	* 
	* @param group which is wanted to do the affinity report with
	*/
	public double affinityReport(Group group) {
		int n12 = 0, n21 = 0;
		MySystem sys = null;

		sys = MySystem.getInstance();
		
		for(Project p: sys.getProjects()) {
			if(p.getProponent().equals(this.name)) {
				if(group.getGroupVote().contains(p)) { //Projects created by this Group and supported by Group "group"
					n12++;
				}
			}
			
			if(p.getProponent().equals(group.getName())) {
				if(this.groupVote.contains(p)) { //Projects created by Group "group" and supported by this Group
					n21++;
				}
			}
		}
		if(this.groupVote.size()+group.getGroupVote().size() == 0) return 0.00; // Just avoid divisions by 0
		else return ((n12+n21)/(this.groupVote.size()+group.getGroupVote().size()));
	}
	
	/**
	* This method checks whether  the information of group g is up to date,
	* depending if we want to add or remove information from group we need to
	* specified it with its parameters
	* 
	* @param u, the user to be checked
	*/
	public void update(User u) {
			
		if(!this.groupUsers.contains(u)) {
			this.groupUsers.add(u);
		}
		return;
	}
	
	/**
	* This method checks whether the information of project p is up to date,
	* depending if we want to add or remove information from group we need to
	* specified it with its parameters
	* 
	* @param p, the project to be checked
	*/
	public void update(Project p) {
		
		if(!this.groupVote.contains(p)) {
			this.groupVote.add(p);
		}
		return;
	}
	
	/**
	* This method returns a string with the group name
	* 
	* @return the group name
	*/
	@Override
	public String toString() {
		return this.name;
	}
}
