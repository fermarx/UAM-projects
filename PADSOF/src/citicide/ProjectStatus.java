package citicide;

import java.io.Serializable;
/**
* This is the ProjectStatus enumeration from the citicide package.
* The status of the project will we different form "Unknown" whenever
* it has obtained the minimum number of votes
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial ProjectStatus enumeration implements Serializable
*/
public enum ProjectStatus implements Serializable{
	ADMITTED, REJECTED, PENDING, UNKNOWN
}
