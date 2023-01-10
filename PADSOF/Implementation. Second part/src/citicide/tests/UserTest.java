package citicide.tests;

import static org.junit.Assert.*;
import java.io.*;
import org.junit.*;
import citicide.*;

/**
* This is the UserTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class UserTest {	
	private MySystem system;
	private InfrastructureProject p1; 
	private User u1;
	private User u2; 
	private User u3;
	private Group g;
	
	/**
	* This method creates all the necessary objects to test
	* the methods of User
	* 
	* @throws java.lang.Exception
	*/
	@Before
	public void setUp() throws Exception {
		system = MySystem.getInstance();
		
		u1 = new User("Jack", "123456789", "soydelauam", UserPublicStatus.ONLINE);
		u2 = new User("Tomas", "987654321", "mellamantom", UserPublicStatus.OCCUPIED);
		u3 = new User("Ron", "567891234", "ronuam", UserPublicStatus.ONLINE);
		
		p1 = new InfrastructureProject ("New Bridge", "Jack", 
									"We want to bouild a bridge to connect two diatricts of our city.",
									1300.0, system.getSingleDistrict("San Blas-Canillejas"), "~/Desktop/Projects/Infrastructure/picture1");
		
		g = new Group("New Jersey", u1, "Plans for the future of our city.");
		
		u2.joinUserGroup(g); //in group g we have u1 and u2
		
		u3.addIndividualVote(p1); //u3 votes on p1
	}
	
	/**
	* Test method for {@link citicide.User#joinUserGroup(citicide.Group)}
	* and {@link citicide.User#addIndividualVote(citicide.Project)}
	*/
	@Test
	public void testUser() {
		
		assertEquals(true, u1.getIndividualVote().contains(p1));
		assertEquals(false, u2.getIndividualVote().contains(p1));
		assertEquals(true, u3.getIndividualVote().contains(p1));
		
		assertEquals(true, u1.getMembership().contains(g));
		assertEquals(true, u2.getMembership().contains(g));
		assertEquals(false, u3.getMembership().contains(g));
	
	}
}
