package citicide;

import java.io.*;
import es.uam.eps.sadp.grants.CCGG;
import es.uam.eps.sadp.grants.GrantRequest;
import es.uam.eps.sadp.grants.GrantRequest.ProjectKind;
import es.uam.eps.sadp.grants.InvalidRequestException;

/**
* This is the SocialProject class from the citicide package
* It inherits from Project
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial SocialProject class implements Serializable
*/
public class SocialProject extends Project implements Serializable {

	private static final long serialVersionUID = 1L;
	private String asddressedTo;
	private Scope scope;
	
	/**
	* This is the constructor class
	* 
	* @param name the name of the project
	* @param proponent the nickname of the user who has created the project
	* @param description the description of the project with a maximum of 500 characters
	* @param moneyRequested the money that the proponent is asking for to accomplish the project
	* @param addressedTo the social group who is the target of the project
	* @param scope whether it is national or international
	*/
	public SocialProject(String name, String proponent, String description, Double moneyRequested,
			 String addressedTo, Scope scope) {
		
		super(name, proponent, description, moneyRequested);
		this.asddressedTo = addressedTo;
		this.scope = scope;
	}
	
	/**
	* This method send a project to the cityCouncil in order
	* to validate it
	* 
	* @throws InvalidRequestException if GrantRequest is not correct
	* @throws IOException if there are communication problems
	*/
	@Override
	public void sendExternalAssociation() throws InvalidRequestException, IOException  {
		
		try { 
			GrantRequest toSend = new SendProject(this, ProjectKind.Social);
			this.externalID = CCGG.getGateway().submitRequest(toSend);
			
		}catch(IOException sys_2){
			System.out.println(sys_2);
		}catch(InvalidRequestException sys_3){
			System.out.println(sys_3);
		}
		return;
	}
}
