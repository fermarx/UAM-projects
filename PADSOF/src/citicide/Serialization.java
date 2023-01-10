package citicide;

import java.io.*;

/**
* This is the serialization class from the citicide package for saving 
* and loading all the information of the application in a binary file.
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
* @serial Serialization class implements Serializable
*/
public class Serialization implements Serializable{
	
	private static final long serialVersionUID = 1L;
	
	/**This method writes in the binary file "system.ser" the information of the application
	* 
	* @throws FileNotFoundException when the file to work with is not found
	* @throws IOException in case there is an error reading, writing the file
	*/
	public static void serializationWrite() throws FileNotFoundException, IOException {

		MySystem system = null;
		FileOutputStream oFile = null;
		ObjectOutputStream out = null;
		
	    try {
	    	/*We create a new file to store the information
	    	  if file already exists will do nothing*/
	    	File yourFile = new File("system.ser");
	    	
	        if (!yourFile.exists()){ 
	        	yourFile.createNewFile();
	        	System.out.println("File is created!");
	        }else {
	            System.out.println("File already exists.");
	        }
	        
	        oFile = new FileOutputStream(yourFile); 
	        out = new ObjectOutputStream(oFile);
	        
	        //We initialize the application we want to store in the file
 			system = MySystem.getInstance();

	        //We write all the information of the system in the file
	        out.writeObject(system);
	        
	        out.close();
	        oFile.close();
	        System.out.println("Serialize complete.");
	         
	    }catch(IOException i) {
	        //Exception for if the serialization fails
	    	out.close();
	        oFile.close();
	    	i.printStackTrace();
	    }
	}

	/**This method reads from the binary file "system.ser" the information of the application
	* 
	* @throws FileNotFoundException when the file to work with is not found
	* @throws IOException in case there is an error reading, writing the file
	*/
	public static MySystem serializationRead() throws FileNotFoundException, IOException {
		
		MySystem system = null;
		ObjectInputStream in = null;
		
		try{  
			//Creating stream to read the object  
			in = new ObjectInputStream(new FileInputStream("system.ser"));
			
			//We initialize the application we want to store in the file
	        system = (MySystem)in.readObject();
			
	        if(system != null) {
			//printing the data of the serialized object
			System.out.println("Deserialization complete.\n");
			System.out.println("System loaded\nAdministrator: " + system.getAdmin().toString() +
								"\n"+ system.getUsers().size() + " users\n" 
								+ system.getGroups().size() + " groups\n"
								+ system.getProjects().size()+ " Projects\n"
								+ system.getNotification().size()+ " Notifications");
	        }
			
		} catch(ClassNotFoundException e){ //If there is not a MySystem class in the file or the is no file
			throw new FileNotFoundException();
			
		} finally {
			//closing the stream 
			if(in != null)
				in.close();
		}
		
		return system;
	}  
}
