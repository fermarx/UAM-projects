package citicide.tests;

import static org.junit.Assert.*;
import java.io.*;
import java.util.ArrayList;
import org.junit.*;
import citicide.*;

/**
* This is the MySystemTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class MySystemTest {
	private MySystem system;
	private User user1;
	private User user2;
	private User user3;
	private User user4;
	
	/**
	* This method creates all the necessary objects to test
	* the methods of MySystem
	* 
	* @throws java.lang.Exception
	*/
	@Before
	public void setUp() throws Exception {
		
		system = MySystem.getInstance();
		
		user1 = new User("Peter", "123456789", "1234", UserPublicStatus.SLEEPING);
		user2 = new User("Anna", "987654321", "4321", UserPublicStatus.OFFLINE);
		user3 = new User("Jones", "987654352", "5555", UserPublicStatus.ONLINE);
		user4 = new User("Alex", "015187166", "jfls5", UserPublicStatus.OCCUPIED);
		
		user1.setSitutation(UserPrivateStatus.ACCEPTED);
		user2.setSitutation(UserPrivateStatus.BANNED);
		user3.setSitutation(UserPrivateStatus.REJECTED);
		
		system.addUser(user1);
		system.addUser(user2);
		system.addUser(user3);
		system.addUser(user4);
	}
	
	/**
	* Test method to check whether districts read from the text file
	* are saved correctly in the ArrayList
	*/
	@Test
	public void testDistricts() {
		final ArrayList<String> districts = system.getDistricts();
		
		assertEquals(true, districts.contains("Fuencarral-El Pardo"));
		assertNotSame(true, districts.contains("Baraja"));
	}
	
	/**
	* Test method for {@link citicide.MySystem#logIn(java.lang.String, java.lang.String)}.
	*/
	@Test
	public void testLogIn() {
		assertEquals(true, system.logIn("Peter", "1234"));
		assertNotSame(true, system.logIn("Petr", "1234"));
		assertNotSame(true, system.logIn("Peter", "134"));
		
		assertNotSame(true, system.logIn("Anna", "1234"));
		assertNotSame(true, system.logIn("Anna", "4321"));
		
		
		assertNotSame(true, system.logIn("Jones", "5555"));
		assertNotSame(true, system.logIn("Jones", "5565"));
		
		assertNotSame(true, system.logIn("Alex", "jfls6"));
		assertNotSame(true, system.logIn("Alex", "jfls5"));
	}
	
	/**
	* This method cleans all the created objects when testing
	* the methods of MySystem
	* 
	* @throws java.lang.Exception
	*/
	@After
	public void tearDown() throws Exception {
		system.getUsers().clear();
	}
	
}