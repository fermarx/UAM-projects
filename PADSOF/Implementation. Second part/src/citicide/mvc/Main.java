package citicide.mvc;

import java.io.FileNotFoundException;
import java.io.IOException;

import javax.swing.SwingUtilities;

import citicide.MySystem;
import citicide.mvc.controller.Controller;
import citicide.mvc.view.Gui;

/**
* This is the Main class from the citicide.mvc package
* This class runs the application
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class Main {
	
	/**
	* This is the main method that runs the application
	*  
	* @param args
	*/
	public static void main(String[] args) throws FileNotFoundException, IOException  {
		SwingUtilities.invokeLater(new Runnable() {
		    public void run() {
				MySystem app = null;
				
				app = MySystem.getInstance();
				
				Gui gui = new Gui("Citicide");

				Controller  controlador = new Controller(gui, app);
				gui.setController( controlador );
		    }
		});

	}

}
