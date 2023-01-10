package citicide.mvc.view;

import java.awt.*;

import java.awt.image.BufferedImage;
import java.io.*;
import javax.swing.*;
import javax.imageio.ImageIO;

/**
* This is the InitialPage class from the citicide.mvc.view package
* This view sets the is used as base in LogInPage and LogUpPage
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class InitialPage extends JPanel{

	private static final long serialVersionUID = 1L;
	private BufferedImage centralLogo;
	
	/**
	* This is the constructor method
	* 
	* This method sets the view of the Initial page. It is used in
	* in LogInPage and LogUpPage
	*/
	public InitialPage(){
		
		this.setLayout(new BorderLayout());
		JPanel warp = new JPanel(new GridBagLayout()); 

		try {
			centralLogo = ImageIO.read(new File("logo_transparent.png"));
		} catch (IOException ex) {
			System.out.println("Error");
		}
		
		JLabel picLabel = new JLabel(new ImageIcon(centralLogo));
		GridBagConstraints constraints = new GridBagConstraints();
		constraints.gridx = 0; 
		constraints.gridy = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 2; 
        constraints.weighty = 1.0;
        constraints.anchor = GridBagConstraints.SOUTH;
        warp.add(picLabel,constraints);
		
		
		JLabel text = new JLabel(" To enhance the world ");
		Font myFont = new Font("Serif", Font.ITALIC | Font.BOLD, 12);
	    Font newFont = myFont.deriveFont(50F);
	    text.setFont(newFont);
		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.weighty = 1.0;
        constraints.anchor = GridBagConstraints.NORTH;
	    warp.add(text,constraints);
	    
	    this.add(warp,BorderLayout.CENTER);

	}

}
