package citicide;

import java.io.Serializable;

/**
* This is the UserPrivateStatus enumeration from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial UserPrivateStatus enumeration implements Serializable
*/
public enum UserPrivateStatus implements Serializable {
	ACCEPTED(0), REJECTED(1), PENDING(2), BANNED(3);
	
	private int value;
	
	/**
	* This is the private constructor method
	* 
	* @param value, the value of the status
	*/
	private UserPrivateStatus (int value){
		this.value = value;	
	}
	
	/**
	* 
	* @return the integer value of the status
	*/
	public int getValue() {
		return this.value;
	}
}
