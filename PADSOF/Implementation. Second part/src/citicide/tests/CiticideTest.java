package citicide.tests;

import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;
import citicide.*;

/**
* CiticideTest it test the citicide application.
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Subgroup class implements Serializable
*/
public class CiticideTest {

	private static Group g1, g2;
	
	private static InfrastructureProject p1;
	private static SocialProject p2;
	
	private static Notification n1;
	
	private static Subgroup sg1, sg2, sg3, sg4, sg5;
	
	private static User u1, u2, u3, u4, u5, u6, u7, u8, u9, u10;
	
	public static void main(String[] args) throws Exception {
		
		System.out.println("Loading all previous saved data...");
		Thread.sleep(1000);
		MySystem sys = MySystem.getInstance();
		Thread.sleep(1500);
		
		System.out.println("\nCreating users...");
		u1 = new User("Pablo", "123456789", "asdfghjkl", UserPublicStatus.OCCUPIED);
		System.out.println("User created: " + u1.getNickname());
		Thread.sleep(500);
		u2 = new User("Pedro", "928402847", "passsssword", UserPublicStatus.OFFLINE);
		System.out.println("User created: " + u2.getNickname());
		Thread.sleep(500);
		u3 = new User("Amanda", "039485002", "contrasena", UserPublicStatus.ONLINE);
		System.out.println("User created: " + u3.getNickname());
		Thread.sleep(500);
		u4 = new User("Alvaro", "111199983", "nanana", UserPublicStatus.ONLINE);
		System.out.println("User created: " + u4.getNickname());
		Thread.sleep(500);
		u5 = new User("Sara", "098765432", "qwerty", UserPublicStatus.ONLINE);
		System.out.println("User created: " + u5.getNickname());
		Thread.sleep(500);
		u6 = new User("Rosalia", "987654321", "mas", UserPublicStatus.SLEEPING);
		System.out.println("User created: " + u6.getNickname());
		Thread.sleep(500);
		u7 = new User("Becky", "678901234", "nose", UserPublicStatus.SLEEPING);
		System.out.println("User created: " + u7.getNickname());
		Thread.sleep(500);
		u8 = new User("Benito", "123454321", "zxcvbnm", UserPublicStatus.OCCUPIED);
		System.out.println("User created: " + u8.getNickname());
		Thread.sleep(500);
		u9 = new User("Jaime", "098767890", "alergj", UserPublicStatus.OCCUPIED);
		System.out.println("User created: " + u9.getNickname());
		Thread.sleep(500);
		u10 = new User("Luis", "444444444", "987tyui", UserPublicStatus.ONLINE);
		System.out.println("User created: " + u10.getNickname() + "\n\n");
		Thread.sleep(1000);
		
		System.out.println("User " + u1.getNickname() + " is creating a group...");
		Thread.sleep(700);
		g1 = new Group("Musicals <3", u1, "For musicals lovers. We have a discussion every Friday to"
				+ " find which is the best musical of all times.");
		System.out.println("Group created: " + g1.getName() + "\tManager: " + g1.getManager().getNickname());
		System.out.println("Description: " + g1.getDescription() + "\n");
		Thread.sleep(1500);
		System.out.println("User " + u2.getNickname() + " is creating a group...");
		Thread.sleep(700);
		g2 = new Group("Videogames", u2, "Play or die. This is our description.");
		System.out.println("Group created: " + g2.getName() + "\tManager: " + g2.getManager().getNickname());
		System.out.println("Description: " + g2.getDescription() + "\n\n");
		Thread.sleep(1500);
		
		System.out.println("User " + u8.getNickname() + " is creating a subgroup...");
		Thread.sleep(700);
		sg1 = new Subgroup("Cats", u8, "Cats is the best musical of all times", g1);
		System.out.println("Subgroup created: " + sg1.getName() + "\t\t\t\tManager: " + sg1.getManager().getNickname());
		System.out.println("Description: " + sg1.getDescription() + "\n");
		Thread.sleep(1500);
		System.out.println("User " + u7.getNickname() + " is creating a subgroup...");
		Thread.sleep(700);
		sg2 = new Subgroup("Billy Elliot", u7, "We love dance", g1);
		System.out.println("Subgroup created: " + sg2.getName() + "\t\t\tManager: " + sg2.getManager().getNickname());
		System.out.println("Description: " + sg2.getDescription() + "\n");
		Thread.sleep(1500);
		System.out.println("User " + u10.getNickname() + " is creating a subgroup...");
		Thread.sleep(700);
		sg3 = new Subgroup("Cats: The movie", u10, "Cats the movie was a complete failure", sg1);
		System.out.println("Subgroup created: " + sg3.getName() + "\t\tManager: " + sg3.getManager().getNickname());
		System.out.println("Description: " + sg3.getDescription() + "\n");
		Thread.sleep(1500);
		System.out.println("User " + u6.getNickname() + " is creating a subgroup...");
		Thread.sleep(700);
		sg4 = new Subgroup("Zelda", u6, "The best videogame saga of all times", g2);
		System.out.println("Subgroup created: " + sg4.getName() + "\t\t\t\tManager: " + sg4.getManager().getNickname());
		System.out.println("Description: " + sg4.getDescription() + "\n");
		Thread.sleep(1500);
		System.out.println("User " + u9.getNickname() + " is creating a subgroup...");
		Thread.sleep(700);
		sg5 = new Subgroup("Zelda: breath of the wild", u9, "A masterpiece", sg4);
		System.out.println("Subgroup created: " + sg5.getName() + "\tManager: " + sg5.getManager().getNickname());
		System.out.println("Description: " + sg5.getDescription() + "\n\n");
		Thread.sleep(1500);
		
		System.out.println("Group " + g2.getName() + " is creating a project...");
		Thread.sleep(700);
		p1 = new InfrastructureProject("New Arcade", "Videogames", 
				"We want tu build a new arcade for playing.", 1100.0,
				"Moncloa", "C://Desktop/Infrastructure/Arcade");
		p1.voteGroup(g2);
		//Should print 3 because it counts also the subgroups members
		System.out.println("Creating project: " + p1.getTitle() + "\tProponent: " + p1.getProponent() + "\tNumber of votes: " + p1.getPopularityReport());
		System.out.println("Description: " + p1.getDescription() + "\n");
		Thread.sleep(1500);
		System.out.println("User " + u3.getNickname() + " is creating a project...");
		Thread.sleep(700);
		p2 = new SocialProject("Dog pound", "Amanda", "Dog pound for keep dogs safe",
				2000.0, "Dogs", Scope.NATIONAL);
		u3.voteProjectUser(p2);
		//Should print 1
		System.out.println("Creating project: " + p2.getTitle() + "\tProponent: " + p2.getProponent() + "\tNumber of votes: " + p2.getPopularityReport());
		System.out.println("Description: " + p2.getDescription() + "\n\n");
		Thread.sleep(1500);

		System.out.println("Adding users to \""+ g1.getName() +"\"...");
		Thread.sleep(700);
		u4.joinUserGroup(g1);
		u5.joinUserGroup(g1);
		System.out.println("User(s) joined: ");
		for(User u: g1.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("");
		Thread.sleep(1000);
		
		System.out.println("Adding users to \""+ g2.getName() +"\"...");
		Thread.sleep(700);
		u1.joinUserGroup(g2);
		System.out.println("User(s) joined: ");
		for(User u: g2.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("");
		Thread.sleep(1000);
		
		System.out.println("Adding users to \""+ sg1.getName() +"\"...");
		Thread.sleep(700);
		sg1.joinSubgroup(u9);
		u9.joinUserGroup(sg1);
		System.out.println("User(s) joined: ");
		for(User u: sg1.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("");
		Thread.sleep(1000);
		
		System.out.println("Adding users to \""+ sg2.getName() +"\"...");
		Thread.sleep(700);
		sg2.joinSubgroup(u8);
		sg2.joinSubgroup(u9);
		System.out.println("User(s) joined: ");
		for(User u: sg2.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("");
		Thread.sleep(1000);

		System.out.println("Adding users to \""+ sg3.getName() +"\"...");
		Thread.sleep(700);
		sg3.joinSubgroup(u6);
		System.out.println("User(s) joined: ");
		for(User u: sg3.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("\n");
		Thread.sleep(1000);

		System.out.println("Adding users to \""+ sg4.getName() +"\"...");
		Thread.sleep(700);
		sg4.joinSubgroup(u8);
		sg4.joinSubgroup(u4);
		System.out.println("User(s) joined: ");
		for(User u: sg4.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("\n");
		Thread.sleep(1000);
		
		System.out.println("Adding users to \""+ sg5.getName() +"\"...");
		Thread.sleep(700);
		sg5.joinSubgroup(u5);
		sg5.joinSubgroup(u8);
		System.out.println("THERE WAS AN ERROR JOINING " + u8.getNickname() + 
							".\nHe is already member of the group \"" + sg4.getName() + "\" so he can not join to a children subgroup.");
		Thread.sleep(1500);
		System.out.println("User(s) joined: ");
		for(User u: sg5.getArrayUsers())
			System.out.println(u.getNickname());
		System.out.println("\n");
		Thread.sleep(1500);
		
		
		//should print 7
		System.out.println("Number of votes of \"" + p1.getTitle() + "\" Now that we have added users to the group:\t" + p1.getPopularityReport());
		Thread.sleep(1000);
		
		p1.voteUser(u6);
		p1.voteUser(u7);
		p1.voteUser(u10);
		//should print 10
		System.out.println("Number of votes of \"" + p1.getTitle() + "\" Now that we have voted with some users:\t\t" + p1.getPopularityReport());
		Thread.sleep(1000);
		
		sg2.voteProjectAsGroup(p2);
		//should print 4
		System.out.println("Number of votes of \"" + p2.getTitle() + "\" Now that we have voted with a group:\t\t" + p2.getPopularityReport());		
		Thread.sleep(1000);
		
		p2.voteUser(u10);
		//should print 5
		System.out.println("Number of votes of \"" + p2.getTitle() + "\" Now that we have voted with some users:\t\t" + p2.getPopularityReport() + "\n\n");
		Thread.sleep(1500);
		
		System.out.println("Creating a new notification to send it to \"" + sg4.getName() + "\"...");
		Thread.sleep(700);
		n1 = new Notification("New update available", 
				"\tWe just realized that a new update for \"the legend of zelda: ocarina of time\"\n"
				+ "\tis coming this weekend. A nice sourprise for all the lovers.", sg4.getArrayUsers());
		System.out.println("Sending notification ...");
		Thread.sleep(700);
		n1.sendNotification();
		for(User u: sg4.getArrayUsers()) {
			System.out.println(u.getNickname() + "\thas a new notification: " + u.getNthNotification(0).getTitle());
			Thread.sleep(500);
		}
		Thread.sleep(500);
		System.out.println("\nBody of the notification:\n" + n1.getBody());
		Thread.sleep(2000);
		
		u8.leaveUserGroup(sg2);
		System.out.println("\nUser " + u8.getNickname() + " if lefting the group \"" + sg2.getName() + "\"...");
		Thread.sleep(1000);
		System.out.println("\nRemaining users in group \"" + sg2.getName() + "\":");
		for(User u: sg2.getArrayUsers())
			System.out.println(u.getNickname());
		
		Thread.sleep(1000);
		//should print 5
		System.out.println("\nNumber of votes of \"" + p2.getTitle() + "\" Now that someone left a group: " + p2.getPopularityReport());
		
		System.out.println("Sending " + p1.getTitle() + " to an external association...");
		Thread.sleep(1000);
		p1.sendExternalAssociation();		
		System.out.println("The project has an external ID: " + p1.getExternalID() +"\n");
		System.out.println("COME IN 1-7 DAYS TO CHECK IF YOUR PROJECT HAS BEEN ACCEPTED! :D\n");
		Thread.sleep(2000);
		p1.askForMoneyGranted();
		
		Serialization.serializationWrite();
		
	}
}
