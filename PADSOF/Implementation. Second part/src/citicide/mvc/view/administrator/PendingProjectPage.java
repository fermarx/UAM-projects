package citicide.mvc.view.administrator;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

import citicide.InfrastructureProject;
import citicide.MySystem;
import citicide.Project;
import citicide.SocialProject;

/**
* This is the PendingProjectPage class from the citicide.mvc.view.administrator package
*
* It allows the administrator to see the pending projects and to accept
* or reject the ones he/she wants.
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class PendingProjectPage extends JPanel implements ActionListener{

	private static final long serialVersionUID = 1L;
	private AdministratorPage administratorPage;
	private JLabel title = new JLabel("Title");	
	private JLabel district= new JLabel("District: ");
	private BufferedImage imageIcon;
	private JLabel picLabel = new JLabel("");
	
	private JLabel addressed = new JLabel("");
	private JLabel scope = new JLabel("Scope: ");
	
	private JLabel proponent= new JLabel("Proponent: ");
	private JLabel amountMoney= new JLabel("Amount of money: ");
	private JLabel description = new JLabel("Description: ");
	
	private JTextField minNumVotesField = new JTextField(4);
	private JButton finishButton = new JButton("Finish");
	private ButtonGroup bg = new ButtonGroup();
	private JRadioButton admit = new JRadioButton("Admit");
	private JRadioButton reject = new JRadioButton("Reject");
	
	/**
	* This is the constructor class of PendingProjectPage
	* 
	* @param administratorPage administrator page the class pending project is using
	*/
	public PendingProjectPage(AdministratorPage administratorPage) {
		
		this.administratorPage = administratorPage;
		
		this.setLayout(new BorderLayout());

		String projectTitle = administratorPage.getListproject().getSelectedValue();
		
		for(Project p: administratorPage.getprojects()) {
			if(p.getTitle().equals(projectTitle)) {
				break;
			}
		}
		
		JPanel pn = new JPanel(new GridLayout(2, 8, 10, 5)); //Rows, columns, separation between rows, separation columns
		
		//North panel
		Font myFont = new Font("Sans Regular", Font.BOLD, 20);
		
	    title.setFont(myFont);
	    
	    JPanel n1 = new JPanel(new FlowLayout(FlowLayout.CENTER));
		n1.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 1) );
		n1.add(title);
		pn.add(n1);
		
		
		this.add(pn, BorderLayout.NORTH);
		
		//Central part
		JPanel c = new JPanel(new GridBagLayout());
		GridBagConstraints constraints = new GridBagConstraints();
		
		constraints.gridx = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
	    constraints.fill =  GridBagConstraints.HORIZONTAL;
	    constraints.anchor = GridBagConstraints.PAGE_START;
	    constraints.weightx = .05;
	    constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 1; 
		constraints.gridwidth = 0; 
		constraints.gridheight = 1; 
	    constraints.fill =  GridBagConstraints.HORIZONTAL;
	    constraints.anchor = GridBagConstraints.PAGE_START;
	    constraints.weightx = .05;
	    constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
	
		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		c.add(district, constraints);

		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		c.add(addressed, constraints);

		constraints.gridx = 0; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		c.add(proponent, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		c.add(amountMoney, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 5; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		c.add(description, constraints);
			
		constraints.gridx = 0; 
		constraints.gridy = 6; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		c.add(scope, constraints);

		constraints.gridx = 1; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		constraints.anchor = GridBagConstraints.SOUTH;
		c.add(picLabel,constraints);

		this.add(c,BorderLayout.CENTER);
		
		//South part
		JPanel s = new JPanel(new GridBagLayout());
		
		constraints.gridx = 0;
		constraints.gridy = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
		s.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		constraints.gridx = 1;
		constraints.gridy = 0;
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.fill =  GridBagConstraints.HORIZONTAL;
        constraints.anchor = GridBagConstraints.PAGE_START;
        constraints.weightx = .05;
        constraints.weighty = .05;
        s.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);

        bg.add(admit);
        bg.add(reject);
        
        constraints.gridx = 0; 
		constraints.gridy = 1; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;  
		s.add(admit, constraints);
        
		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;  
		s.add(reject, constraints);
        
        constraints.gridx = 0; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		s.add(new JLabel("Choose the minimum number of votes: "), constraints);
        	
		constraints.gridx = 0; 
		constraints.gridy = 4; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;  
		s.add(minNumVotesField, constraints);
		
		constraints.gridx = 1; 
		constraints.gridy = 2;
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		finishButton.setBackground(Color.yellow);
		s.add(finishButton, constraints);
		
		this.add(s, BorderLayout.SOUTH);
        
        //Updates the pending projects, when you chose one
        this.administratorPage.getListproject().getSelectionModel().addListSelectionListener(listAc->{
        	String project2 = administratorPage.getListproject().getSelectedValue();
        	updateProject(project2);
        });
		
        /* finishButton action listener. When you push the finish
         * button it calls the methods of the code of pending
         * it needs to use to set the project as accepted or rejected.
         * */
		finishButton.addActionListener(e->{
			MySystem  sys = MySystem.getInstance();
			String title2 = administratorPage.getListproject().getSelectedValue(); 						
			Project project= sys.getSingleProject(title2); 
			
			if(admit.isSelected()) {
				long votes = 0;
				try {
					votes = Long.valueOf(minNumVotesField.getText());
				}catch(Exception exc) {
					JOptionPane.showMessageDialog(administratorPage, "Enter only numbers",
							"Information message", JOptionPane.ERROR_MESSAGE);
					this.bg.clearSelection();
					this.minNumVotesField.setText("");
					return;
				}
				
				project.setAccepted(true);
				project.setMinNumVotes(votes);
				JOptionPane.showMessageDialog(administratorPage, "Project accepted",
						"Information message", JOptionPane.INFORMATION_MESSAGE);
			}else if(reject.isSelected()) {
				project.setAccepted(false);
				JOptionPane.showMessageDialog(administratorPage, "Project rejected",
						"Information message", JOptionPane.INFORMATION_MESSAGE);
			}else {
				JOptionPane.showMessageDialog(administratorPage, "Please select an option",
						"Error", JOptionPane.ERROR_MESSAGE);
			}
			this.bg.clearSelection();
			this.minNumVotesField.setText("");
			updateProject(title2);
			this.administratorPage.setListProjectListModel(this.administratorPage.getGui().getController().updateList(AdministratorPage.PENDINGPROJECTS));
		});
		
	}
	
	/**
	 * This method allows the administrator to update the 
	 * page any time he makes a change
	 * 
	 * @param p project you want to interact with
	 * */
	public void updateProject(String p) {
		try {
			MySystem  sys = MySystem.getInstance();
			String title = p; 							
			Project project= sys.getSingleProject(title); 
			
						
			this.proponent.setText( "Proponent: " + project.getProponent());
			this.amountMoney.setText( "Amount of money: " + project.getMoneyRequested());
			this.description.setText( "Description: " + project.getDescription());
			this.minNumVotesField.setText("");
			
			if(project instanceof InfrastructureProject) {
				InfrastructureProject infrastructureProject = (InfrastructureProject)project;
				this.title.setText( project.getTitle() + " - Infrastructure");
				this.district.setText("District: " + infrastructureProject.getDistrict());
				
				try {
					imageIcon = ImageIO.read(new File(infrastructureProject.getPicturePath()));
				} catch (IOException ex) {System.out.println("Error. File not found.");
				try {
					this.imageIcon = ImageIO.read(new File("null.png"));
					} catch (IOException ex2) {}}
				
				picLabel.setIcon(new ImageIcon(imageIcon));
				
				this.addressed.setText("");
				this.scope.setText("");
			}
			
			if(project instanceof SocialProject) {
				SocialProject socialProject = (SocialProject)project;
				this.title.setText( project.getTitle() + " - Social");
				this.district.setText("");
				this.addressed.setText("Addressed to: " + socialProject.getAddressedTo());
				this.scope.setText("Scope: " + socialProject.getScope());
				this.minNumVotesField.setText("");

				try {
					imageIcon = ImageIO.read(new File("null-social.png"));
				} catch (IOException ex) {}
				
				picLabel.setIcon(new ImageIcon(imageIcon));
			}
			
		}catch(NullPointerException e) {
			MySystem  sys = MySystem.getInstance();
			String title = p; 							
			Project project= sys.getSingleProject(title); 
			this.title.setText( "Title: Select a project");
			this.proponent.setText( "Proponent: Select a project");
			this.amountMoney.setText( "Amount of money: Select a project");
			this.description.setText( "Description: Select a project");
			this.minNumVotesField.setText("");

			try {
				imageIcon = ImageIO.read(new File("null-social.png"));
			} catch (IOException ex) {}
			
			picLabel.setIcon(new ImageIcon(imageIcon));
			
			if(project instanceof InfrastructureProject) {
				this.district.setText("District: Select a project");
				this.addressed.setText("");
				this.scope.setText("");
			}
			
			if(project instanceof SocialProject) {
				this.district.setText("");
				this.addressed.setText("Addressed To: Select a project");
				this.scope.setText("Scope: Select a project");
			}	
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
	}

}
