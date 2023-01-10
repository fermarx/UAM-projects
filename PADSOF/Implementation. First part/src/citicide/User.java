package citicide;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.*;
import java.io.Serializable;

/**
* This is the user class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial User class implements Serializable
*/
public class User implements Serializable{
	private static final long serialVersionUID = 1L;
	private static final int MAX_IDNUM = 9;
	
	private String nickname;
	private String iDNumber;
	private String password;
	private boolean turnOnNotification;
	private UserPublicStatus status;
	private UserPrivateStatus situation;
	private ArrayList<Group> membership = new ArrayList<Group>();
	private ArrayList<Project> individualVote = new ArrayList<Project>();
	private ArrayList<Notification> notification = new ArrayList <Notification>();
	
	/**
	* This is the constructor class of user
	* 
	* @param nickname string with the name of the user
	* @param iDNumber string with the id of the user
	* @param password string with the password of the user
	* @param status status of the user
	* 
	* @throws IllegalArgumentException when it has passed an illegal argument
	*/
	public User(String nickname, String iDNumber, String password, UserPublicStatus status) throws IllegalArgumentException {
		
		if (iDNumber.length() != MAX_IDNUM) throw new IllegalArgumentException("The ID number must contain 9 characters\n");
		
		MySystem sys;
		try { // Add it to the system
			sys = MySystem.getInstance();
			for(User u: sys.getUsers()) {
				if(u.nickname.contentEquals(nickname)) {
					System.out.println(u.nickname);
					System.out.println(nickname);
				}
				if(u.nickname.contentEquals(nickname)) throw new IllegalArgumentException("This nickname is already used. Choose another one\n");
			}
			sys.addUser(this);
		}catch(FileNotFoundException sys_1){			
			System.out.println(sys_1);
		}catch(IOException sys_2){
			System.out.println(sys_2);
		}
		
		this.nickname = nickname;
		this.iDNumber = iDNumber;
		this.password = password;
		this.turnOnNotification = true;
		this.status = status;
		this.situation = UserPrivateStatus.PENDING;
		
		
	}
	
	/**
	* This method let the user change his password
	*
	* @param password the new password of the user
	*/
	public void changePassword(String password) {
		this.password = password;
	}
	
	/**
	* This method let the user decide if he wants
	* to receive notifications or not
	*
	* @param turnOnNotification turns on or off the notification
	*/
	public void setTurnOnNotification(boolean turnOnNotification) {
		this.turnOnNotification = turnOnNotification;
	}
	
	/**
	* This method sets the status of the user
	*
	* @param status the new status of the user
	*/
	public void setStatus(UserPublicStatus status){
		this.status = status;
	}
	
	/**
	* This method sets the situation of the user
	*
	* @param situation the new situation to be set
	*/
	
	public void setSitutation(UserPrivateStatus situation){
		this.situation = situation;
	}
	
	/**
	* This method adds a group to the membership arrayList
	*
	* @param group the group that the user is joining to
	*/
	public void addMembership(Group group) {	
		this.membership.add(group);
		group.update(this);
	}
	
	/**
	* This method adds a project to the indivudualVote arrayList
	*
	* @param project the project that the user is voting to
	*/
	public void addIndividualVote(Project project) {
		this.individualVote.add(project);
	}
	
	/**
	* This method adds a notification to the notification arrayList
	*
	* @param notification the notification that the user is receiving
	*/
	public void addNotification(Notification notification) {
		this.notification.add(notification);
	}
	
	/**
	 * @return nickname, string with the nickname of the user 
	 */
	public String getNickname() {
		return this.nickname;
	}
	
	/**
	 * @return iDNumber, string with the iDNumber of the user 
	 */
	public String getiDNumber() {
		return this.iDNumber;
	}
	
	/**
	 * @return password, string with the password of the user 
	 */
	public String getPassword() {
		return this.password;
	}
	
	public boolean getTurnOnNotification() {
		return this.turnOnNotification;
	}
	
	/**
	 * @return status, UserPublicStatus enumeration with
	 * the status of the user 
	 */
	public UserPublicStatus getStatus() {
		return this.status;
	}
	
	/**
	 * @return situation, UserPrivateStatus enumeration with
	 * the situation of the user 
	 */
	public UserPrivateStatus getSituation() {
		return this.situation;
	}
	
	/**
	 * @return value, the value of the public status enumeration 
	 */
	public int getUserPublicStatus() {
		return status.getValue();
	}
	
	/**
	 * @return value, the value of the public situation enumeration 
	 */
	public int getUserPrivateStatus() {
		return situation.getValue();
	}
	
	/**
	* @return membership, ArrayList of type Group with
	* the Groups the user is in
	*/
	public ArrayList<Group> getMembership() {
		return this.membership;
	}
	
	/**
	* @return membership(i), Group that is in the i-th
	* place of the array membership
	*/
	public Group getNthMembership(int i) {
		return this.membership.get(i);
	}
	
	/**
	* @return individualVote, ArrayList of type Project
	* with the projects of the user voted for
	*/
	public ArrayList<Project> getIndividualVote() {
		return this.individualVote;
	}
	
	/**
	* @return individualVote(i), Project that is in the i-th
	* place of the array indivudualVote
	*/
	public Project getNthIndividualVote(int i) {
		return this.individualVote.get(i);
	}
	
	/**
	* @return notification, ArrayList of type notification
	* with the notification the user has received
	*/
	public ArrayList<Notification> getNotification() {
		return this.notification;
	}
	
	/**
	* @return notification(index), notification that is in the i-th
	* place of the array notification
	*/
	public Notification getNthNotification(int index) {
		return this.notification.get(index);
	}
	
	/**
	 * It calls the function joinGroup and adds the 
	 * name of the group to the membership array
	 * 
	 * @param group that the user wants to join
	 */
	public void joinUserGroup(Group group) {
		this.membership.add(group);
		group.joinGroup(this);
	}
	
	/**
	 * It calls the function leaveGroup and deletes the
	 * name of the group from the membership array
	 * 
	 * @param group that the user wants to leave
	 */
	public void leaveUserGroup(Group group) {
		group.leaveGroup(this);
		this.membership.remove(group);
	}
	
	/**
	 * It calls the function voteUser and deletes the
	 * project of from the individualVote array
	 * 
	 * @param project that the user wants to vote
	 */
	public void voteProjectUser(Project project) {
		this.individualVote.add(project);
		project.voteUser(this);
	}
	
	/**
	 * It deletes the notification when the user wants to
	 * 
	 * @param notification that the user wants to remove
	 */
	public void deleteNotification(Notification notification) {
		this.notification.remove(notification);
	}
	
	/**
	 * This method checks whether  the information of group g is up to date,
	 * depending if we want to add or remove information from user we need to
	 * specified it with its parameters
	 * 
	 * @param g, the group to be checked
	 */
	public void update(Group g) {
		
		if(!this.membership.contains(g)) {
			this.membership.add(g);
		}
		return;
	}
	
	/**
	 * This method checks whether the information of project p is up to date,
	 * depending if we want to add or remove information from user we need to
	 * specified it with its parameters
	 * 
	 * @param p, the project to be checked
	 */
	public void update(Project p) {
		
		if(!this.individualVote.contains(p)) {
			this.individualVote.add(p);
		}
		return;
	}
	
}