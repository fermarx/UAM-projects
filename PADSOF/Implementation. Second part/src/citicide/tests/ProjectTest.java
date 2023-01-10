package citicide.tests;

import static org.junit.Assert.*;
import modifiableDates.ModifiableDate;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.time.LocalDate;
import org.junit.*;
import citicide.*;
import es.uam.eps.sadp.grants.*;
import es.uam.eps.sadp.grants.GrantRequest.ProjectKind;

/**
* This is the ProjectTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class ProjectTest {
	private MySystem system;
	private Project covid01;
	private Project emergency;
	private Project ad;
	private User u1, u2, u3;
	
	/**
	 * This method creates all the necessary objects to test
	 * the methods of Project
	 * 
	 * @throws java.lang.Exception
	 */
	@Before
	public void setUp() throws Exception{ //Generate some project for all the methods testing proposes

			system = MySystem.getInstance();
		
		u1 = new User("Pedro Sanchez", "111111111", "MrHandsome", UserPublicStatus.OFFLINE);
		u2 = new User("Minister of Inland", "222222222", "inland", UserPublicStatus.OFFLINE);
		u3 = new User("Loly4789", "666666666", "laloly", UserPublicStatus.OFFLINE);
			
		covid01 = new SocialProject("Counter-COVID", "Pedro Sanchez", "Este virus lo paramos todos", (double)100000,
				"España", Scope.NATIONAL);
		
		emergency = new SocialProject("Emergency State", "Minister of Inland", "Este virus lo paramos todos", (double)1000,
				"España", Scope.NATIONAL);
		
		ad = new InfrastructureProject("thoughtfulness", "Loly4789", "You all must wash your hands", (double)2000,
				system.getSingleDistrict("Puente de Vallecas"), "~/Desktop/ChristmasPhoto.png");
	}

	/**
	* Test method for {@link citicide.Project#voteUser(citicide.User)}
	*/
	@Test
	public void testVoteUser() {
		User u1 = new User("LokoColina","12345678R","acb123",UserPublicStatus.ONLINE);
		

		//boolean result = covid01.getIndividualVoters().contains(u1);
		
		assertEquals("PASSED", true, covid01.voteUser(u1));
		
	}

	/**
	* Test method for {@link citicide.Project#voteGroup(citicide.Group)}
	*/
	@Test
	public void testVoteGroup() {
		User user1 = new User("LokoColina","12345678R","acb123",UserPublicStatus.ONLINE);
		Group g1 = null;
		
		covid01.voteUser(user1);
		
		try {
			g1 = new Group("GrupoRdm",user1,"Grupo para quedar");
		}catch (IllegalArgumentException e1) {
			System.out.println(e1);
		}
		
		covid01.voteUser(user1); 	// Vote as individual
		covid01.voteGroup(g1);	// Vote as group member

		
		boolean result = covid01.getGroupVoters().contains(g1);
		
		assertEquals(true, result);
		
		assertEquals(2, covid01.getPopularityReport());
		//PASS iff the vote has only be counted once
		
	}

	/**
	* Test method for {@link citicide.Project#isExpired()}
	*/
	@Test
	public void testIsExpired() {
		
		// Supposed to be run as an administrator
		covid01.setAccepted(true);
		
		boolean result = covid01.isExpired();
		
		assertEquals("PASSED", false, result);
			
	}
	
	/**
	* Test method for {@link citicide.Project#checkNumberOfVotes()}
	*/
	@Test
	public void testCheckNumberOfVotes() {
		
		covid01.setMinNumVotes(200);
		covid01.setPopularityReport(201);
		
		boolean result = covid01.checkNumberOfVotes();
		
		assertEquals("PASSED", true, result);
			
	}
	
	/**
	* @Brief this function sometimes can throws InvalidIDException, just because if
	*  .sendExternalAssociation(); catch an IOException, then after that you are calling to
	*  .askForMoneyGranted(); without a ID, so we assert that in this test
	*  
	* Test method for {@link citicide.Project#askForMoneyGranted()}
	*/
	@Test
	public void testAskForMoneyGranted() {	
		try {
			MySystem sys = MySystem.getInstance();

			CCGG.getGateway().setDate(sys.getDate()); // Start at day one
			ad.sendExternalAssociation();			  // send it
			
			sys.setDate(sys.getDate().plusDays(8));   // Add and offset of days
			CCGG.getGateway().setDate(sys.getDate());
			
			ad.askForMoneyGranted();

			assertNotNull(ad.getMoneyGranted()); // 0 rejected, null pending (All project must be accepted)

		}catch(InvalidRequestException sys_3) {
			System.out.println(sys_3);
		}catch(IOException sys_2) {
			System.out.println(sys_2);
		}catch(InvalidIDException sys_3){
			System.out.println(sys_3);
		}
	}
	
	/**
	 * Test method for {@link citicide.Project#sendExternalAssociation()}
	 */
	@Test
	public void testSendExternalAssociation() { 
		
		try {
			covid01.sendExternalAssociation(); 
			assertNotNull( covid01.getExternalID() );
		}catch(InvalidRequestException sys_3) {
			System.out.println(sys_3);
		}catch(IOException sys_2) {
			System.out.println(sys_2);
		}

	}
	
	/**
	 * Test method for {@link citicide.Project#deleteProject()}
	 */
	@Test
	public void testDeleteProject() {
		User u1 = new User("LokoColina","12345678R","acb123",UserPublicStatus.ONLINE);
		Group g1 = null;
		
		try {
			g1 = new Group("GrupoRdm",u1,"Grupo para quedar");
		}catch (IllegalArgumentException e1) {
			System.out.println(e1);
		}
		
		covid01.voteUser(u1);
		covid01.voteGroup(g1);
		covid01.deleteProject();
		
		boolean result1 = u1.getIndividualVote().contains(covid01);
		boolean result2 = g1.getGroupVote().contains(covid01);
		 
		 assertEquals("PASSED", false, result1 | result2 );
		 
	}

	/**
	 * Test method for {@link citicide.Project#unvoteProject(citicide.Group, citicide.User)}
	 */
	@Test
	public void testUnvoteProject() {
		User user1 = new User("LokoColina","12345678R","acb123", UserPublicStatus.ONLINE);
		Group g1 = null;
		
		try {
			g1 = new Group("GrupoRdm",user1,"Grupo para quedar");
		}catch (IllegalArgumentException e1) {
			System.out.println(e1);
		}
		
		ad.voteGroup(g1);
		g1.joinGroup(user1);
		
		boolean result1 = (ad.getPopularityReport() == 2) ? true : false ;

		assertEquals("PASSED", true, result1 );
		
		g1.leaveGroup(u1); // this function calls internally to UnvoteProject
		// the manager cannot exit
		boolean result2 = (ad.getPopularityReport() == 2) ? true : false ;
		
		assertEquals("PASSED", true, result2 );
		
	}

	/**
	 * Test method to check whether a project synchronization after
	 * one several consecutive actions have been taken
	 */
	@Test
	public void testUpdate() {
		User u1 = new User("LokoColina","12345678R","acb123",UserPublicStatus.ONLINE);
		User u2 = new User("Sr.Eustaquio","87654321R","Eustaquio Ramirez",UserPublicStatus.ONLINE);
		User u3 = new User("Bugs_Bonney","87655487R","Hola amigos",UserPublicStatus.ONLINE);
		Group g1 = null;
		
		try {
			g1 = new Group("GrupoRdm",u1,"Grupo para quedar");
		}catch (IllegalArgumentException e1) {
			System.out.println(e1);
		}
		
		g1.joinGroup(u1); //do not add duplicated members
		g1.joinGroup(u3);
		
		emergency.voteUser(u2); 	//vote + 
		emergency.voteUser(u1); 	//vote +
		emergency.voteGroup(g1); 	//vote +
		
		boolean result1 = (emergency.getPopularityReport() == 4) ? true : false ;
		
		g1.leaveGroup(u3); // vote -, + Update project
		
		boolean result2 = (emergency.getPopularityReport() == 3) ? true : false ;
		
		assertEquals("PASSED", true, result1 | result2 );
		
	}
	
	/**
	 * This method removes all the created objects for testing
	 * the methods of Project
	 * 
	 * @throws java.lang.Exception
	 */
	@After
	public void tearDown() throws Exception{
		
		system = MySystem.getInstance();
		system.getProjects().clear();
		system.getUsers().clear();

		ModifiableDate.setDate(1, 3, 2017); //resets the date 
	}

}
