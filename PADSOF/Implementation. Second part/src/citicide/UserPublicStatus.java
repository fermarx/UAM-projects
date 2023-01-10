package citicide;

import java.io.Serializable;

/**
* This is the UserPublicStatus enumeration from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial UserPublicStatus enumeration implements Serializable
*/
public enum UserPublicStatus implements Serializable {
	ONLINE(0), OFFLINE(1), SLEEPING(2), OCCUPIED(3);  
	
	private int value;
	
	/**
	* This is the private constructor method
	* 
	* @param value, the value of the status
	*/
	private UserPublicStatus (int value){
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
