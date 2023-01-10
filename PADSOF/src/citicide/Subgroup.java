package citicide;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.Serializable;

/**
* This is the subgroup class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Subgroup class implements Serializable
*/
public class Subgroup extends Group implements Serializable{

	private static final long serialVersionUID = 1L;
	
	/**
	* This is the subgroup constructor method
	* 
	* @param name, string with the name of the group
	* @param manager, Type user, manager of a group
	* @param description, string with the description of a group
	* @param father, Group root of the tree
	* 
	* @throws IllegalArgumentException when it has passed an illegal argument
	*/
	public Subgroup(String name, User manager, String description, Group father) throws IllegalArgumentException{
		super(name, manager, description);
		super.father = father;
		super.level = father.level + 1;
		this.father.subgroups.add(this);
		
	}
	
	/**
	* Method that lets the user join a subgroup
	* if and only if he is in a group that is
	* sibling of the group he wants to join
	* 
	* @param user that wants to join the subgroup
	*/	
	public void joinSubgroup(User user) {
		if(checkSubgroups(user) == true) {
			this.joinGroup(user);
			user.joinUserGroup(this);
			for(Project p: this.father.groupVote) {
				if(!p.checkVotes(user, this)) { //if the user is not in any group that had voted for this project
					if(!p.individualVoters.contains(user)) { // if the user has not vote as individual
						p.setPopularityReport(p.getPopularityReport()+1);
					}
				}
			}
		}
	}
	
	/**
	* Function that checks if the group a user wants to
	* join is sibling of the group he is in
	* 
	* @param user that needs to be checked
	* 
	* @return true if the if the group a user wants to
	* join is sibling of the group he is in, false otherwise
	*/
	private boolean checkSubgroups(User user) {
		
		for(User u: this.father.groupUsers) {
			if(user.equals(u) == true)
				return false;
		}
		
		for(Subgroup s: this.father.subgroups) {//subgroup array of the father
			if(this.level != s.level ) {
				for(User u: s.groupUsers) {
					if(u.getNickname().equals(user.getNickname())) {
						return false;
					}
				}
			}else {
				return true;
			}
		}
		
		return true;
	}
	
}