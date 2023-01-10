package citicide;

import java.io.Serializable;
import java.util.*;
import java.time.*;

/**
* This is the Notification class from the citicide package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Notification class implements Serializable
*/
public class Notification implements Serializable{
	
	private static final long serialVersionUID = 1L;
	
	private String title;
	private final long id;
	private String body;
	private ArrayList<User> recipient = new ArrayList<User>();
	
	private boolean read;
	private LocalDate sentDate;
	
	/**
	* This is the constructor method
	* 
	* @param title the issue of the notification
	* @param body the details, explanation of the notification
	* @param recipient is an array with all the users who are going to receive the notification
	*/
	public Notification(String title, String body, ArrayList<User> recipient) {
		this.title = title;
		this.body = body;
		this.recipient = recipient;
		this.id = this.title.hashCode() + this.body.hashCode();
		
		MySystem sys = MySystem.getInstance();
		sys.addNotification(this);
		
		
	}

	/**
	* This method sets the title of the notification
	*
	* @param title of the notification
	*/
	public void setTitle(String title) {
		this.title = title;
	}

	/**
	* 
	* @return the title of the notification
	*/
	public String getTitle() {
		return title;
	}

	/**
	* 
	* @return the id of the notification
	*/
	public long getId() {
		return id;
	}

	/**
	* This method sets the body of the notification
	*
	* @param body of the notification
	*/
	public void setBody(String body) {
		this.body = body;
	}

	/**
	* 
	* @return the body of the notification
	*/
	public String getBody() {
		return body;
	}

	/**
	* This method sets the recipients of the notification
	*
	* @param recipient array with all the users who are going to receive the notification
	*/
	public void setRecipient(ArrayList<User> recipient) {
		this.recipient = recipient;
	}

	/**
	* 
	* @return array with all the users who are going to receive the notification
	*/
	public ArrayList<User> getRecipient() {
		return recipient;
	}
	
	/**
	* This method sets the read of the notification
	*
	* @param read true if the user has read the notification, false otherwise
	*/
	public void setRead(boolean read) {
		this.read = read;
	}
	
	/**
	* 
	* @return true if the user has read the notification, false otherwise
	*/
	public boolean isRead() {
		return read;
	}

	/**
	* This method sets the sentDate of the notification
	*
	* @param sentDate, date when the notification was sent
	*/
	public void setSentDate(LocalDate sentDate) {
		this.sentDate = sentDate;
	}

	/**
	* 
	* @return the sentDate of the notification
	*/
	public LocalDate getSentDate() {
		return sentDate;
	}

	/**
	* This method sends the notification to all users who are recipients 
	* and want to receive it (they have turned it on)
	*/
	public void sendNotification() {
		
		MySystem sys = MySystem.getInstance();
		this.sentDate = sys.getDate();
		
		for(User u: recipient) {
			if(u.getTurnOnNotification()) {
				u.addNotification(this);
			}
		}
	}
	
	/**
	* This method sends the notification to all users who are recipients
	* 
	*  @param goThrough is used to make the difference with the sendNotification method
	*  without parameters in the Controller class
	*/
	public void sendNotification(Boolean goThrough) {
		
		MySystem sys = MySystem.getInstance();
		this.sentDate = sys.getDate();
		
		for(User u: recipient) {
			u.addNotification(this);
		}
		
	}
}