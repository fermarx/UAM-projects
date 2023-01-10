package citicide;

import java.io.*;

import es.uam.eps.sadp.grants.CCGG;
import es.uam.eps.sadp.grants.GrantRequest;
import es.uam.eps.sadp.grants.InvalidRequestException;
import es.uam.eps.sadp.grants.GrantRequest.ProjectKind;

/**
* This is the InfrastructureProject class from the citicide package
* It inherits from Project
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial InfrastructureProject class implements Serializable
*/
public class InfrastructureProject extends Project implements Serializable{

	private static final long serialVersionUID = 1L;
	private String district;
	private String picturePath;

	/**
	* This is the constructor method
	* 
	* @param name the name of the project
	* @param proponent the nickname of the user who has created the project
	* @param description the description of the project with a maximum of 500 characters
	* @param moneyRequested the money that the proponent is asking for to accomplish the project
	* @param district the place where the project is going to be performed
	* @param picturePath path where the picture of the project can be found
	*/
	public InfrastructureProject(String name, String proponent, String description, Double moneyRequested, String district, String picturePath) {
		super(name, proponent, description, moneyRequested);
		this.district = district;
		this.picturePath = picturePath;
	}
	
	/**
	* 
	* @return the district of the project
	*/
	public String getDistrict(){
		return this.district;
	}
	
	/**
	* 
	* @return the picture path of the project
	*/
	public String getPicturePath(){
		return this.picturePath;
	}
	
	
	/**
	* This method send a project to the cityCouncil in order
	* to validate it
	*
	* @throws InvalidRequestException if GrantRequest is not correct
	* @throws IOException if there are communication problems
	*/
	@Override
	public void sendExternalAssociation() throws InvalidRequestException, IOException {
		
		try { 
			GrantRequest toSend = new SendProject(this, ProjectKind.Infrastructure);
			this.externalID = CCGG.getGateway().submitRequest(toSend);
	
		}catch(IOException sys_2){
			System.out.println(sys_2);
		}catch(InvalidRequestException sys_3){
			System.out.println(sys_3);
		}
		
	}
}
