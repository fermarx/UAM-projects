package citicide.tests;

import static org.junit.Assert.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.junit.*;
import citicide.*;

/**
* This is the GroupTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class GroupTest {
	private MySystem system;
	private Group g;
	private User u1;
	private User u2;
	private User u3;
	private Project p;
	
	/**
	* This method creates all the objects for testing
	* the methods of Group
	* 
	* @throws java.lang.Exception
	*/
	@Before
	public void setUp() throws Exception {
		try {
			system = MySystem.getInstance();
		} catch (FileNotFoundException e1) {
			System.out.println(e1);
		} catch (IOException e2) {
			System.out.println(e2);
		}
		
		u1 = new User ("Mario", "123456789", "abcde", UserPublicStatus.OCCUPIED);
		u2 = new User ("Luigi", "987654321", "xyz", UserPublicStatus.ONLINE);
		u3 = new User ("Bowser", "567891234", "nanana", UserPublicStatus.SLEEPING);

		
		p = new InfrastructureProject ("Tower", "Mario",
				"We want to bouild a bridge to connect two diatricts of our city.",
				300.0, system.getSingleDistrict("Tetuán"), "~/Desktop/Projects/Infrastructure/picture1");

		g = new Group("Sports", u1, "For sport lovers");
		
		g.joinGroup(u2);
		
		g.voteProjectAsGroup(p);
	}
	
	/**
	* Test method for {@link citicide.Group#joinGroup(citicide.User)}
	* and {@link citicide.Group#voteProjectAsGroup(citicide.Project)}
	*/
	@Test
	public void testGroup() {
		
		assertEquals(true, g.getGroupVote().contains(p));
		
		assertEquals(true, g.getArrayUsers().contains(u1));
		assertEquals(true, g.getArrayUsers().contains(u2));
		assertEquals(false, g.getArrayUsers().contains(u3));

	}
	
}
