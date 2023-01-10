package citicide;

import java.util.*;
import java.io.*;
import java.time.LocalDate;
import es.uam.eps.sadp.grants.*;
import java.time.temporal.ChronoUnit;

/**
* This is the Project class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Project class implements Serializable
*/
public abstract class Project implements Serializable {
	
	private static final long serialVersionUID = 1L;
	private static final int DAYS_TO_EXPIRE = 30;
	private static final int INIT_NUM_VOTES = 0;
	private static final int MAX_DESCRIPTION = 500;
	private static final int MIN_MONEYREQU = 1000;
	private static final int MAX_MONEYREQU = 1000000;
	
	protected String title;
	protected String proponent;
	protected final int iD;
	protected String externalID;
	protected String description;
	protected long minNumVotes;
	protected int popularityReport; // current number of votes
	protected Double moneyRequested;
	protected Double moneyGranted;
	protected LocalDate acceptanceDate;
	protected boolean accepted;
	protected ProjectStatus status;
	protected ArrayList<User> individualVoters = new ArrayList<User>();
	protected ArrayList<Group> groupVoters= new ArrayList<Group>();
	protected boolean checkAdmin;
	
	/**
	* This is the constructor method
	* 
	* @param title the title of the project
	* @param proponent the nickname of the user who has created the project
	* @param description the description of the project with a maximum of 500 characters
	* @param moneyRequested the money that the proponent is asking for to accomplish the project
	*/
	public Project(String title, String proponent, String description, double moneyRequested) {
                
		if (description.length() > MAX_DESCRIPTION) throw new IllegalArgumentException("The description has a maximum of 500 characters\n");
        if (moneyRequested < MIN_MONEYREQU) throw new IllegalArgumentException("Money requested should be more or equal than 1000 euros");
        if (moneyRequested > MAX_MONEYREQU) throw new IllegalArgumentException("Money requested should be less or equal than 1000000 euros");
        
        MySystem sys = MySystem.getInstance(); //MOVER A BAJO
        
		this.title = title;
		this.proponent = proponent;
		this.iD = title.hashCode();
		this.externalID = null; 					// it will be send eventually
		this.description = description;
		this.minNumVotes = 0;  						// it will be send eventually
		this.popularityReport = INIT_NUM_VOTES; 
		this.moneyRequested = Double.valueOf(moneyRequested);
		this.moneyGranted = Double.valueOf(0);	// it will be send eventually
		this.acceptanceDate = null; 				// it will be send eventually       
		this.accepted = false; 						//until the administrator tell other thing
		this.status = ProjectStatus.PENDING;		//until the administrator tell other thing
		this.checkAdmin = false;
		
		// Add it to the system
		User aux = null;
		Group aux2 = null;
		
		sys.addProject(this);
		try {
			aux = sys.getSingleUser(proponent);
			aux.update(this);
			this.voteUser(aux);
		}catch(NullPointerException e1) {
			try {
				aux2 = sys.getSingleGroup(proponent);
				aux2.update(this);
				this.voteGroup(aux2);
			}catch(NullPointerException e2) {
				throw new IllegalArgumentException("The proponent must coincide with your user nickname or group name");
			}
				
		}
		
	}

	/**
	* 
	* @return the title of the project
	*/
	public String getTitle() {
		return title;
	}
	
	/**
	* 
	* @return the proponent of the project
	*/
	public String getProponent() {
		return proponent;
	}
	
	/**
	* 
	* @return the ID of the project
	*/
	public int getiD() {
		return iD;
	}
	
	/**
	* 
	* @return the  external ID of the project
	*/
	public String getExternalID() {
		return externalID;
	}

	/**
	* This method sets the external ID of the project
	*
	* @param externalID the new external ID to be set
	*/
	public void setExternalID(String externalID) {
		this.externalID = externalID;
	}

	/**
	* 
	* @return the description of the project
	*/
	public String getDescription() {
		return description;
	}

	/**
	* 
	* @return the minimum number of votes of the project
	*/
	public long getMinNumVotes() {
		return minNumVotes;
	}

	/**
	* This method sets the minimum number of votes of the project
	*
	* @param minNumVotes the new minimum number of votes to be set
	*/
	public void setMinNumVotes(long minNumVotes) {
		this.minNumVotes = minNumVotes;
	}

	/**
	* 
	* @return the number of votes of the project
	*/
	public int getPopularityReport() {
		return this.popularityReport;
	}
	
	/**
	* 
	* @return whether the project has been checked by the administrator or not
	*/
	public boolean getCheckAdmin() {
		return this.checkAdmin;
	}

	/**
	* This method sets the number of votes of the project
	*
	* @param popularityReport the new number of votes to be set
	*/
	public void setPopularityReport(int popularityReport) {
		this.popularityReport = popularityReport;
	}

	/**
	* 
	* @return the money asked for supporting a project
	*/
	public Double getMoneyRequested() {
		return moneyRequested;
	}
	
	/**
	* 
	* @return the money given for supporting a project
	*/
	public Double getMoneyGranted() {
		return moneyGranted;
	}

	/**
	* This method sets the external money given for a project
	*
	* @throws InvalidRequestException if GrantRequest is not correct
	* @throws IOException if there are communication problems
	* @throws InvalidIDException if id is not correct
	*/
	public void askForMoneyGranted() throws InvalidRequestException, IOException, InvalidIDException  {

		try { 
			this.moneyGranted = CCGG.getGateway().getAmountGranted(this.externalID);
			
			if(this.moneyGranted == 0) {
				this.status = ProjectStatus.REJECTED;
			}else{
				this.status = ProjectStatus.ADMITTED;
			}
			
		}catch(NullPointerException sys_4) {
			this.status = ProjectStatus.PENDING;
		}
	}

	/**
	* 
	* @return the acceptance date of the project
	*/
	public LocalDate getacceptanceDate() {
		return acceptanceDate;
	}

	/**
	* 
	* @return the acceptance status of the project
	*/
	public boolean isAccepted() {
		return accepted;
	}

	/**
	* This method sets the acceptance status of a project
	*
	* @param accepted the new amount of money to be set
	*/
	public void setAccepted(boolean accepted) {
		MySystem sys;
		
		this.checkAdmin = true;
		if (accepted == true) {
			sys = MySystem.getInstance();
			this.accepted = accepted;
			this.acceptanceDate = sys.getDate();
			
		}else if(accepted == false)
			this.status = ProjectStatus.REJECTED;
			
	}

	/**
	* 
	* @return the  status of the project
	*/
	public ProjectStatus getStatus() {
		return status;
	}

	/**
	* This method sets the status of a project
	*
	* @param status the new status to be set
	*/
	public void setStatus(ProjectStatus status) {
		this.status = status;
	}
	
	/**
	* 
	* @return the collection of 
	* of user that have voted for the project
	*/
	public ArrayList<User> getIndividualVoters() {
		return individualVoters;
	}

	/**
	* This method sets the collection of user that support a project
	*
	* @param individualVoters the new collection of voters to be set
	*/
	public void setIndividualVoters(ArrayList<User> individualVoters) {
		this.individualVoters = individualVoters;
	}

	/**
	* 
	* @return the collection of 
	* of groups that have voted for the project
	*/
	public ArrayList<Group> getGroupVoters() {
		return groupVoters;
	}
	
	/**
	* This method adds to the arrayList of indivudualVoters
	* a new user
	* 
	* @param user 
	*/
	public void addIndividualVoters(User user) {
		this.individualVoters.add(user);
	}
	
	/**
	* This method adds a arrayList of groupVoters
	* a new group
	* 
	* @param group
	*/
	public void addGroupVoters(Group group) {
		this.groupVoters.add(group);
	}

	/**
	* This method sets the collection of groups that support a project
	*
	* @param groupVoters the new collection of voters to be set
	*/
	public void setGroupVoters(ArrayList<Group> groupVoters) {
		this.groupVoters = groupVoters;
	}
	
	/**
	* This method evaluates whether a given user can vote for the instance
	* of the project or not, if he can support it, then the project will
	* increment by one their number of votes.
	*
	* @param voter is the user that wants to vote for the project.
	* @return true if the vote could be processed, false if the user has already voted
	*/
	public boolean voteUser(User voter) {
		
		if(this.individualVoters.contains(voter)) {	// if this user has voted previously as individual
			return false;
		}
		
		this.individualVoters.add(voter); // add the user in the array
		
		if(!this.checkVotes(voter)) { //if the user is not in any group that had voted for this project
			this.popularityReport++;  //add a vote
		}
		
		voter.update(this);
		
		return true;
	}
	
	/**
	* This method evaluates whether a given group vote for the instance
	* of the project or not, if it can support the project, then the
	* project will increment by the number of members of the group,
	* their number of votes.
	*
	* @param voter is the group that wants to vote for the project.
	* @return true if the vote could be processed, false if the group has already voted
	*/
	public boolean voteGroup(Group voter) {
		
		if(this.groupVoters.contains(voter)) {	// if this group has voted previously
			return false;
		}
		
		for(User u : voter.getArrayUsers()) {
			if(!this.checkVotes(u)) { //if the user is not in any group that had voted for this project
				if(!this.individualVoters.contains(u)) { // if the user has not vote as individual
					this.popularityReport++;
				}
			}
		}
	
		for(Subgroup sg: voter.getSubgroup()) { // add the votes of the members of the subgroups
			for(User u : sg.getArrayUsers()) {
				if(!this.checkVotes(u)) { //if the user is not in any group that had voted for this project
					if(!this.individualVoters.contains(u)) { // if the user has not vote as individual
						this.popularityReport++;
					}
				}
			}
		}
		
		this.groupVoters.add(voter);
		voter.update(this);
		return true;
	}
	
	/**
	* This method evaluates whether the time life of a project has been
	* expired or not.
	*
	* @return true if the project is expired (if it runs out of time) or
	* false otherwise.
	*/
	public boolean isExpired() {
		
		if(this.getDaysLeft() < 0)
			return true;
		
		
		return false; //Otherwise
	}
	
	/**
	* This method returns the days left that this project has to be voted
	*
	* @return days left that this project has to be voted, it is important to recall
	* that if the return value is negative this project has already expired
	*/
	public long getDaysLeft() {
		
		MySystem sys = MySystem.getInstance();

		LocalDate sysDays = sys.getDate();
		long daysInBetween = ChronoUnit.DAYS.between(sysDays, this.acceptanceDate);
		
		long daysLeft = DAYS_TO_EXPIRE - daysInBetween;
		
		return daysLeft;
	}
	
	/**
	* This method evaluates whether a given project has get the
	* minimum number of votes that it needs to be send to the external
	* association.
	*
	* @return true if the project is has get the minimum number of votes
	* or false otherwise.
	*/
	public boolean checkNumberOfVotes() {
		
		if(this.minNumVotes <= this.popularityReport) return true;
		
		return false;

	}
	
	/**
	* This method send a project to the cityCouncil in order
	* to validate it
	* 
	* @throws InvalidRequestException if GrantRequest is not correct
	* @throws IOException if there are communication problems
	*/
	public abstract void sendExternalAssociation() throws InvalidRequestException, IOException;
	
	/**
	* This method manages all the synchronization needed in order to
	* remove a project from the system. 
	*/
	public void deleteProject() {
		
		for (User u : this.individualVoters) u.getIndividualVote().remove(this);  
		for(Group g : this.groupVoters)g.getGroupVote().remove(this);
		
		
		this.individualVoters.clear(); // empty the lists of the project
		this.groupVoters.clear();
		
		// Remove it from the the system
		MySystem sys = MySystem.getInstance();
		
		sys.getProjects().remove(this);
		
		return;
	}
	
	/**
	* This method evaluates whether a given user, is already supporting
	* a project by belonging to any groups that support it or not.
	*
	* @param voter is the user to be check
	* 
	* @return true if the voter is in any group that supports 
	* the project or false otherwise.
	*/
	protected boolean checkVotes(User voter) {
		for(Group p : this.groupVoters) {
			for(User u : p.groupUsers) {
				if(u.equals(voter)) {
					return true;
				}
			}

		}
		
		return false;
	}
	
	/**
	* This method evaluates whether a given user, is already supporting
	* a project by belonging to any groups that support it or not.
	*
	* @param voter is the user to be check
	* @param group the group you want to exclude
	* 
	* @return true if the voter is in any group that supports 
	* the project or false otherwise.
	*/
	protected boolean checkVotes(User voter, Group group) {
		for(Group p : this.groupVoters) {
			if(!p.equals(group)) {
				for(User u : p.groupUsers) {
					if(u.equals(voter)) {
						return true;
					}
				}
			}
				
		}
		return false;
	}
	
	/**
	* This method handled the possible situation when an user leaves
	* a group, and that groups were supporting this project, so this 
	* subroutine will decrement by one the number of votes of the project
	* only if the user is not supporting the project individually 
	* (or by belonging to other group that also supports the 
	* project) too.
	*
	* @param group is the group which the user leaves
	* @param user is the user who leaves the group
	*/
	public void unvoteProject(Group group, User user){
		
		// You can only remove your vote for a project if you leave a group that is supporting that 
		// project, so this function only contemplates that possibility   

		if(this.groupVoters.contains(group)) {				// if this group has voted to this project
			if(group.getArrayUsers().contains(user)) {		// if this user is in the group
				if(!this.individualVoters.contains(user)) { // id he has not vote as individual
					if(this.checkVotes(user, group)) {				// if this user has also voted to the project
						// by belonging to other group
						return;
					}else {
						this.popularityReport--; // remove one vote
					}
				}
			}
		}
		return;
	}
	
	/**
	* By general the user cannot unvote, but if he gets ban
	* the project must loose one vote, all this is managed by the 
	* system, we just have implement this method, to make the program
	* more handable.
	*/
	public void unvote() {
		this.popularityReport--;
	}
	
	/**
	* This function does not syschronize nothing, it is not a well practice
	* to use it, but sometimes is necessary
	*/
	public void vote() {
		this.popularityReport++;
	}

	/**
	* This method checks whether  the information of group g is up to date,
	* depending if we want to add or remove information from project we need to
	* specified it with its parameters
	* 
	* @param u, the user to be checked
	*/
	public void update(User u) {
			
		if(!this.individualVoters.contains(u)) {
			this.individualVoters.add(u);
		}
		return;
	}
	
	/**
	* This method checks whether  the information of group g is up to date,
	* depending if we want to add or remove information from project we need to
	* specified it with its parameters
	* 
	* @param g, the group to be checked
	*/
	public void update(Group g) {
		
		if(!this.groupVoters.contains(g)) {
			this.groupVoters.add(g);
		}
		return;
	}
	
	/**
	* This method return the title of the project to be printed
	* 
	* @return project title
	*/
	@Override
	public String toString() {
		return this.title;
	}
	
}