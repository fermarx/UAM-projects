package citicide.tests;
import static org.junit.Assert.*;
import java.io.*;
import org.junit.*;
import citicide.*;

/**
* This is the SerializationTest class from the citicide.tests package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class SerializationTest{
	
	/**
	* Test method for {@link citicide.Serialization#serializationWrite()}.
	* 
	* @throws FileNotFoundException when the file to work with is not found
	* @throws IOException in case there is an error reading, writing the file 
	*/
	@Test
	public void serializationWriteTest() throws FileNotFoundException, IOException {
		 
		boolean write = false;

		try {
			Serialization.serializationWrite();
		} catch (IndexOutOfBoundsException e) {
		    write = true;
		}

		assertFalse(write); //If it catches a exception, it would be true
	}
	
	/**
	* Test method for {@link citicide.Serialization#serializationRead()}.
	* 
	* @throws FileNotFoundException when the file to work with is not found
	* @throws IOException in case there is an error reading, writing the file 
	*/
	@Test
	public void serializationReadTest() throws FileNotFoundException, IOException {

		boolean read = false;

		try {
			Serialization.serializationRead();
		} catch (IndexOutOfBoundsException e) {
		    read = true;
		}

		assertFalse(read); //If it catches a exception, it would be true
	}
}
