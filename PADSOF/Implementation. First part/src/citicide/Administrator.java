package citicide;

import java.util.*;
import java.io.Serializable;

/**
* This is the Administrator class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Administrator class implements Serializable
*/
public class Administrator implements Serializable {
	private static final long serialVersionUID = 1L;
	private String nickname;
	private String password;
	private ArrayList<Notification> notifications = new ArrayList<Notification>();
	
	/**
	* This is the constructor class
	* 
	* @param nickname is the user-like name of the administrator
	* @param password the password to can log as administrator
	*/
	public Administrator(String nickname, String password) {
		this.nickname = nickname;
		this.password = password;
	}

	/**
	* This method sets the notifications ArrayList of Administrator
	*
	* @param notifications the new notifications to be set
	*/
	public void setNotifications(ArrayList<Notification> notifications) {
		this.notifications = notifications;
	}

	/**
	* This method sets the password of the Administrator
	*
	* @param password the new password to be set
	*/
	public void changePassword(String password) {
		this.password = password;
	}
	
	/**
	*
	* @return the nickname string of Administrator
	*/
	@Override
	public String toString() {
		return this.nickname;
		
	}
}
