package citicide.mvc.view;

import java.awt.*;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.*;
import citicide.*;

/**
* This is the NewProjectPage class from the citicide.mvc.view package and 
* extends the ProjectPage class
* This view shows all the accepted by the administrator and not expired
* projects. Also is the place where users can vote projects 
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class NewProjectPage extends ProjectPage{
	private static final long serialVersionUID = 1L;
	
	private MySystem system;
	private JComboBox<String> groupsCombo;
	private final String noAvailGroups = "No groups available";
	
	/**
	* This is the constructor method
	*  
	* @param genPage, the general page
	*/
	public NewProjectPage(GeneralPage genPage) {
		super(genPage);
		system = MySystem.getInstance();
		this.setLayout(new BorderLayout());
		
		this.popRep.setText("");
		
		//Use the north and the central part of the father
		this.add(this.n, BorderLayout.NORTH);
		this.cityHallPanel.remove(this.cityHallButton);
		this.sendCityHall.setText("");
		this.add(this.c, BorderLayout.CENTER);
		
		//Generates the south part
		this.down = new JPanel(new GridLayout(2, 1, 5, 5)); //Rows, columns, separation between rows, separation columns
		
		JLabel groupLabel = new JLabel("Select a group you are manager of to vote");
		JPanel groupPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
		groupPanel.setBorder( BorderFactory.createEmptyBorder(1, 20, 1, 1) );
		this.groupsCombo = new JComboBox<String>();
		groupPanel.add(groupLabel);
		
		DefaultComboBoxModel<String> aux = new DefaultComboBoxModel<String>();
		aux.addElement(this.noAvailGroups);
		this.groupsCombo.setModel(aux); // Initialize it with empty groups
	
        
        groupsCombo.addActionListener(comboForMan->{
        	try {

        		String projectTitle = genPage.getSearchProjectList().getSelectedValue(); //Project that is selected at this moment
				Project actualProject = system.getSingleProject(projectTitle);
				String groupName = (String)groupsCombo.getSelectedItem();
				
				try {
					Group group = system.getSingleGroup(groupName);
					
					if(actualProject.voteGroup(group)) {
						JOptionPane.showMessageDialog(this.genPage, "Your vote to the project " + projectTitle + ", has been processed correctly.",
								"Vote project as group manager - Success", JOptionPane.INFORMATION_MESSAGE);
						
						// Updates the list on the screen in run Time
						this.genPage.setProjectListModel(this.genPage.getGui().getController().updateList(GeneralPage.PROJECT));
						
					}else {
						JOptionPane.showMessageDialog(this.genPage, "You have already voted to the project " + projectTitle + ". A project cannot be voted twice.",
								"Vote project as group manager - Error", JOptionPane.ERROR_MESSAGE);
					}
					
				}catch(NullPointerException e) {
					JOptionPane.showMessageDialog(this.genPage, "You must be manager of a group to perform this action",
							"Vote project as group manager - Error", JOptionPane.ERROR_MESSAGE);
		        }
				
			}catch(NullPointerException e) {
				JOptionPane.showMessageDialog(this.genPage, "You must be inside a project to perform this action",
						"Vote project as group manager - Error", JOptionPane.ERROR_MESSAGE);
	        }

        });
        
		groupPanel.add(this.groupsCombo);
		this.down.add(groupPanel, BorderLayout.SOUTH);
        
		JLabel individualLabel = new JLabel("Vote individually");
		JButton individualButton = new JButton("Vote");
		JPanel individualPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 5, 5)); //Alignment, horizontal separation, vertical separation

		individualButton.addActionListener(voteAsInd->{
			
			try {
				String projectTitle = genPage.getSearchProjectList().getSelectedValue(); //Project that is selected at this moment
				Project actualProject = system.getSingleProject(projectTitle);
				String currentUser = this.genPage.getCurrentUser();
				
				if(actualProject.voteUser(system.getSingleUser(currentUser))) {
					JOptionPane.showMessageDialog(this.genPage, "Your vote to the project " + projectTitle + ", has been processed correctly.",
							"Vote project individually - Success", JOptionPane.INFORMATION_MESSAGE);
					
					// Updates the list on the screen in run Time
					this.genPage.setProjectListModel(this.genPage.getGui().getController().updateList(GeneralPage.PROJECT));
					
				}else {
					JOptionPane.showMessageDialog(this.genPage, "You have already voted to the project " + projectTitle + ". A project cannot be voted twice.",
							"Vote project individually - Error", JOptionPane.ERROR_MESSAGE);
				}
				
			}catch(NullPointerException e) {
				JOptionPane.showMessageDialog(this.genPage, "You must be inside a project to perform this action",
						"Vote project individually - Error", JOptionPane.ERROR_MESSAGE);
	        }
		});
		
		individualPanel.setBorder( BorderFactory.createEmptyBorder(1, 20, 1, 1) );
		individualPanel.add(individualLabel);
		individualPanel.add(individualButton);
		this.down.add(individualPanel, BorderLayout.SOUTH);
		this.add(this.down, BorderLayout.AFTER_LAST_LINE);
		this.setVisible(true);
		
		//Updates the Group, when you choose one
        this.genPage.getSearchProjectList().getSelectionModel().addListSelectionListener(listAc->{
        	this.updateVoterBox();
        	String currentproject = genPage.getSearchProjectList().getSelectedValue();
        	this.updateProject(currentproject);  // Updates the newProject page
        });
	}
	
	/**
	* This method updates the combo box where the groups are displayed for
	* voting as a manager
	*/
	protected void updateVoterBox() {
		boolean fl = false;
		MySystem system = MySystem.getInstance();
		DefaultComboBoxModel<String> aux = new DefaultComboBoxModel<String>();
		User currentUser = system.getSingleUser(this.genPage.getCurrentUser());
        
		try {	
            for(Group g: currentUser.getMembership() ) {
            	if(currentUser.equals(g.getManager())) {   // just add it if you are the manager
            		aux.addElement( g.getName() );
            		fl = true;
            	}
            }
        	
            if(!fl)aux.addElement(this.noAvailGroups); //If it is empty, add an error code
        	
		}catch(NullPointerException e) {
			aux.addElement(this.noAvailGroups);
        }
		
		this.groupsCombo.setModel(aux);
		
	}
	
	/**
	 * This method updates the list of voted projects in the application
	 * 
	 * @param pr project you want to interact with
	 * */
	@Override
	protected void updateProject(String pr) {
		MySystem  sys = MySystem.getInstance();
		String projectTitle = pr; 							        //Project that is selected in this moment
		Project actualProject = sys.getSingleProject(projectTitle); //Returns null if this project does not exists, this means that there are no projects in system
		User currentUser = sys.getSingleUser(genPage.getCurrentUser());
		String votedText = "Voted as: ";
		
		try {
			
			int groupVotes = 0;
			
			for(Group g: currentUser.getMembership()) {
				for(Project p: g.getGroupVote()) {

					if(p.getTitle().equals(projectTitle)) {
						if(groupVotes == 0) {
							votedText += g.getName();
						}else {
							votedText += "," + g.getName();
						}
						
						groupVotes++;
					}
				}
			}
			
			for(Project p: currentUser.getIndividualVote()) {
				if(p.getTitle().equals(projectTitle)) {
					if(groupVotes == 0) {
						votedText += "individually";	
					}else {
						votedText += "and individually";
					}
					
					break;
				}
			}
			
			this.result.setText(votedText);
			this.daysLeft.setText(actualProject.getDaysLeft() + " days left");
			this.proponent.setText("Proponent: " +  actualProject.getProponent() );
			this.requested.setText("Amount of money requested: " + actualProject.getMoneyRequested() + '€');
			this.minVotes.setText("Minimum number of votes: You need to vote the project to see this detail");
			this.popRep.setText("Popularity report: You need to vote the project to see this detail");
			this.description.setText("Description: " + actualProject.getDescription());
			
			this.cityHallPanel.remove(this.cityHallButton);
			this.sendCityHall.setText("");
			
			if(actualProject instanceof InfrastructureProject) {//Infrastructure project
				InfrastructureProject infrastructureProject = (InfrastructureProject)actualProject;
				
				this.district.setText("District: " + infrastructureProject.getDistrict());
				this.title.setText("Project title: " + actualProject.getTitle() + " - Infrastructure");
				this.scope.setText(""); //Clean the other field
				this.addr.setText(""); //Clean the other field
				
				try {
					imageIcon = ImageIO.read(new File(infrastructureProject.getPicturePath()));
				} catch (IOException ex) {
					try {
						imageIcon = ImageIO.read(new File("null.png"));
					} catch (IOException e) {System.out.println("Error. File not Found.");}
				}
				
				picLabel.setIcon(new ImageIcon(imageIcon));
				
			}else { //Social project
				SocialProject socialProject = (SocialProject)actualProject;
				
				this.title.setText("Project title: " + actualProject.getTitle() + " - Social");
				
				if(socialProject.getScope().equals(Scope.NATIONAL)) {
					this.scope.setText("National project");
				}else {
					this.scope.setText("International project");
				}
				
				this.addr.setText("Addressed to: " + socialProject.getAddressedTo());
				
				this.district.setText(""); //Clean the other field
				
				try {
					imageIcon = ImageIO.read(new File("null.png"));
				} catch (IOException ex) {System.out.println("Error. File not found.");}
				
				picLabel.setIcon(new ImageIcon(imageIcon));
				
			}
			
		}catch(NullPointerException e) {
			this.result.setText("Select a group to display its information"); 
			this.title.setText("Project name: Error - You must select an existing project");
			this.daysLeft.setText("Select a project");
			this.proponent.setText("Proponent: Select a project" );
			this.requested.setText("Amount of money requested: Select a project" );
			this.minVotes.setText("Minimum number of votes: Select a project" );
			this.popRep.setText("Popularity report: Select a project" );
			this.description.setText("Description: Select a project");	
			this.district.setText("");
			this.scope.setText("");
			this.addr.setText("");
			try {
				imageIcon = ImageIO.read(new File("null.png"));
			} catch (IOException ex) {System.out.println("Error. File not found.");}
			
			picLabel.setIcon(new ImageIcon(imageIcon));
		}
	}
}
