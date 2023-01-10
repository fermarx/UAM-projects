package citicide.tests;

import static org.junit.Assert.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import org.junit.*;
import citicide.*;

/**
* This is the SubgroupTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class SubgroupTest {
	private MySystem system;
	private Group g;
	private Subgroup sg1;
	private Subgroup sg2;
	private Subgroup sg3;
	private User u1;
	private User u2;
	private User u3;
	private User u4;
	private Project p;
	
	/**
	* This method creates all the objects for testing
	* the methods of Subgroup
	* 
	* @throws java.lang.Exception
	*/
	@Before
	public void setUp() throws Exception {

		system = MySystem.getInstance();
		
		u1 = new User ("Mario", "123456789", "abcde", UserPublicStatus.OCCUPIED);
		u2 = new User ("Luigi", "987654321", "xyz", UserPublicStatus.ONLINE);
		u3 = new User ("Bowser", "567891234", "nanana", UserPublicStatus.SLEEPING);
		u4 = new User ("Peach", "543219876", "letsgo", UserPublicStatus.OFFLINE);
		
		p = new InfrastructureProject ("Tower", "Mario",
				"We want to bouild a bridge to connect two diatricts of our city.",
				1300.0, system.getSingleDistrict("Hortaleza"), "~/Desktop/Projects/Infrastructure/picture1");

		g = new Group("Sports", u1, "For sport lovers");
		
		sg1 = new Subgroup("Tenis", u3, "Tennis lovers", g);
		sg2 = new Subgroup("Roland Garros", u4, "Rafa Nadal", sg1);
		sg3 = new Subgroup("Footbal", u3, "Football lovers", g);
		
		sg1.joinSubgroup(u2);
		sg1.voteProjectAsGroup(p);
		System.out.println("\n");
		sg2.joinSubgroup(u2);/*he can't*/
		sg2.voteProjectAsGroup(p);
	}
	
	/**
	* Test method for {@link citicide.Subgroup#joinSubgroup(citicide.User)},
	* {@link citicide.Subgroup#checkSubgroups(citicide.User)}
	* and {@link citicide.Subgroup#voteProjectAsGroup(citicide.Project)}
	*/
	@Test
	public void testSubgroup() {
		
		assertEquals(false, g.getGroupVote().contains(p));
		
		assertEquals(false, sg1.getArrayUsers().contains(u1));
		assertEquals(true, sg1.getArrayUsers().contains(u2));
		assertEquals(true, sg1.getArrayUsers().contains(u3));
		assertEquals(false, sg1.getArrayUsers().contains(u4));
		
		assertEquals(false, sg2.getArrayUsers().contains(u1));
		assertEquals(false, sg2.getArrayUsers().contains(u2));
		assertEquals(false, sg2.getArrayUsers().contains(u3));
		assertEquals(true, sg2.getArrayUsers().contains(u4));
		
		assertEquals(false, sg3.getArrayUsers().contains(u1));
		assertEquals(false, sg3.getArrayUsers().contains(u2));
		assertEquals(true, sg3.getArrayUsers().contains(u3));
		assertEquals(false, sg3.getArrayUsers().contains(u4));
		
		assertEquals(true, sg1.getGroupVote().contains(p));
		assertEquals(true, sg2.getGroupVote().contains(p));
		assertEquals(false, sg3.getGroupVote().contains(p));
	}
}