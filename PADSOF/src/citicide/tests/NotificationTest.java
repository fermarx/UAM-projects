package citicide.tests;

import static org.junit.Assert.*;
import java.util.ArrayList;
import org.junit.*;
import citicide.*;

/**
* This is the NotificationTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class NotificationTest {
	private Notification notification;
	private User user1;
	private User user2;
	private ArrayList<User> recipients;
	 
	/**
	* This method creates all the necessary objects to test
	* the methods of Notification
	* 
	* @throws java.lang.Exception
	*/
	@Before
	public void setUp() throws Exception {
		user1 = new User("Peter", "123456789", "1234", UserPublicStatus.SLEEPING);
		user2 = new User("Anna", "987654321", "4321", UserPublicStatus.OFFLINE);
		recipients = new ArrayList<User>();
		
		user2.setTurnOnNotification(false);
		
		recipients.add(user1);
		recipients.add(user2);
		
		notification = new Notification("Welcome to Citicide", "Now you are ready to change the world!", recipients);
	}
	
	/**
	* Test method for {@link citicide.Notification#sendNotification()}.
	*/
	@Test
	public void testSendNotification() {
		notification.sendNotification();
		
		assertEquals(true, user1.getNotification().contains(notification));
		assertNotSame(true, user2.getNotification().contains(notification));
	}
}
