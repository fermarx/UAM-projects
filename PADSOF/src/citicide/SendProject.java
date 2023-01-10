package citicide;

import java.time.LocalDate;
import es.uam.eps.sadp.grants.*;

/**
* This is the SendProject class from the citicide package
* 
* This class implements the GrantRequest interface, in order to correctly match
* with the CCGG 
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial SendProject class implements GrantRequest
*/
public class SendProject implements GrantRequest {
	private LocalDate creationDate;
	private String extraData;
	private String projectDescription;
	private ProjectKind projectKind;
	private String projectTitle;
	private Double requestedAmount; 

	/**
	* This is the SendProject constructor
	*
	* @param p is the project to be send
	* @param kind, is the kind of project of p
	*/
	public SendProject(Project p, ProjectKind kind) {
		
		// gets the system date
		MySystem sys = MySystem.getInstance();
		this.creationDate = sys.getDate();
			
		this.extraData = p.getProponent();
		this.projectDescription = p.getDescription();
		this.projectKind = kind;
		this.projectTitle = p.getTitle();
		this.requestedAmount = Double.valueOf( p.getMoneyRequested() );
		
	}
	
	/**
	* 
	* @return a free String file, for storing extra information
	* about the project, e.g the proponent name
	*/
	@Override
	public String getExtraData() {
		return this.extraData;
	}

	/**
	* 
	* @return the description of the project
	*/
	@Override
	public String getProjectDescription() {
		return this.projectDescription;
	}

	/**
	* 
	* @return the kind of the project
	*/
	@Override
	public ProjectKind getProjectKind() {
		return this.projectKind;
	}

	/**
	* 
	* @return the name of the project
	*/
	@Override
	public String getProjectTitle() {
		return this.projectTitle;
	}

	/**
	* 
	* @return the requested amount of money of the project
	*/
	@Override
	public double getRequestedAmount() {
		return this.requestedAmount;
	}
	
	/**
	* 
	* @return the  creation date of the request of the project
	*/
	public LocalDate getCreationDate() {
		return this.creationDate;
	}

}
