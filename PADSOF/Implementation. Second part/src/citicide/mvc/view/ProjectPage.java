package citicide.mvc.view;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import javax.imageio.ImageIO;
import javax.swing.*;
import citicide.*;
import es.uam.eps.sadp.grants.InvalidRequestException;

/**
* This is the ProjectPage class from the citicide.mvc.view package
* This view shows all the accepted by the administrator and not expired
* project. This projects have been voted by the current user or by any group
* he or she is in 
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class ProjectPage extends JPanel{
	private static final long serialVersionUID = 1L;
	
	protected GeneralPage genPage;
	protected JLabel title = new JLabel("");
	protected JLabel daysLeft = new JLabel("");
	protected JLabel proponent = new JLabel("");
	protected JLabel requested = new JLabel("");
	protected JLabel minVotes = new JLabel("");
	protected JLabel description = new JLabel("");
	protected JLabel popRep = new JLabel("");
	protected JLabel district = new JLabel("");
	protected JLabel scope = new JLabel("");
	protected JLabel addr = new JLabel("");
	protected JLabel result = new JLabel("");
	protected BufferedImage imageIcon;
	protected JLabel picLabel = new JLabel("");
	protected JLabel sendCityHall = new JLabel("If you are the proponent and the project has reached the minimum number of votes, you can send it to the City Hall");
	protected JComboBox<String> districtCombo;
	protected JComboBox<String> managerCombo;
	protected JPanel c;
	protected JPanel down;
	protected JPanel n;
	protected JPanel cityHallPanel;
	protected JButton cityHallButton;
	
	/**
	* This is the constructor method
	*  
	* @param genPage, the general page
	*/
	public ProjectPage(GeneralPage genPage) {
		
		MySystem system = MySystem.getInstance(); // We used it in several Action Listeners
		this.genPage = genPage;	
		this.setLayout(new BorderLayout());

		//Generate the north part
		n = new JPanel(new GridLayout(1,2,10,10));
		
		JTextField idSearchBar = new JTextField(10);
		JButton searchButton = new JButton("Check project status by ID");
		
		searchButton.addActionListener(src->{
			boolean flag = false; //To control whether a message has been already displayed
			
			User currentUser = system.getSingleUser(genPage.getCurrentUser());
			ArrayList<Group> managerGroups = new ArrayList<>();
			
			for(Group g: currentUser.getMembership()) {
				
				if(g.getManager().equals(currentUser)) {
					managerGroups.add(g);
				}
				
			}
			
			ArrayList<Project> auxProjects = new ArrayList<>();
			for(Project p: system.getProjects()) {
				
				if(genPage.getCurrentUser().equals(p.getProponent())) {
					auxProjects.add(p);
					
				}else {
					
					for(Group g: managerGroups) {
						if(g.getName().equals(p.getProponent())) {
							auxProjects.add(p);
						}
						
					}
				}
			}
			
			for(Project p: auxProjects) {
				
				if(String.valueOf( p.getiD() ).equals( idSearchBar.getText() )) {
					if(p.isAccepted() && p.getCheckAdmin()) {
						JOptionPane.showMessageDialog(this.genPage, "The project " + p.getTitle() + " has the status: accepted by the administrator" + 
								"\nIt has obtained " + p.getPopularityReport() + " out of " + p.getMinNumVotes() + " votes required", "Project status - Success", 
								JOptionPane.INFORMATION_MESSAGE);
						
					}else if(!p.getCheckAdmin()){
						JOptionPane.showMessageDialog(this.genPage, "The project " + p.getTitle() + " has the status: pending, waiting for the administrator's decision", 
								"Project status - Success", JOptionPane.INFORMATION_MESSAGE);
						
					}else {
						JOptionPane.showMessageDialog(this.genPage, "The project " + p.getTitle() + " has the status: rejected by the administrator",
								"Project status - Success",	JOptionPane.INFORMATION_MESSAGE);
					}
					
					flag = true;
					break;
					
				}else {
					
					try {
						p.getExternalID();

						if(p.getExternalID().equals( idSearchBar.getText() )) {
							
								if(this.genPage.getGui().getController().askForMoneyGrantedController(p)) {
									JOptionPane.showMessageDialog(this.genPage, "The project " + p.getTitle() + " has the status: " + p.getStatus() + 
											"\nIt has obtained " + p.getPopularityReport() + " out of " + p.getMinNumVotes(), "Project status - Success", 
											JOptionPane.INFORMATION_MESSAGE);
									this.notifySearch(p);
								}else {
									JOptionPane.showMessageDialog(this.genPage, "There was an error trying to comunicate with the City Hall servers",
											"Project status - Error", JOptionPane.ERROR_MESSAGE);
								}
								
								flag = true;
								break;
						}

					}catch(NullPointerException e) {

					}
				}
			}
				
			if(!flag) {
				JOptionPane.showMessageDialog(this.genPage, "You must enter a valid project ID",
						"Project status - Error", JOptionPane.ERROR_MESSAGE);
				idSearchBar.setText(""); //Clean the input
			}
			
		});
		
		JPanel n1 = new JPanel(new FlowLayout(FlowLayout.LEFT));
		n1.setBorder(BorderFactory.createEmptyBorder(1, 5, 1, 1) );
		n1.add(idSearchBar);
		n1.add(searchButton);
		n.add(n1);

		JButton createInfProjectButton = new JButton("Create infrastructure project");
		JButton createSocProjectButton = new JButton("Create social project");
		
		createInfProjectButton.addActionListener(crtInf->{
			Boolean flag = false;
			JTextField nameField = new JTextField(25);
			JTextField descriptionField = new JTextField(50);
			JTextField reqMon = new JTextField(20);
			this.districtCombo = new JComboBox<String>();
			this.managerCombo = new JComboBox<String>();
			JTextField picPath = new JTextField(30);
			
			//Load the District into the comboBox
			this.updateDistricBox();
			this.updateManagerBox();

			JPanel myPanel = new JPanel(new GridLayout(6,6,0,5));			// Creates a matrix

			myPanel.add(new JLabel("Do you want to create as a manager? If yes, then select a group"));
			myPanel.add(this.managerCombo);
			myPanel.add(new JLabel("Project Title: "));
			myPanel.add(nameField);
			myPanel.add(new JLabel("Description: "));
			myPanel.add(descriptionField);
			myPanel.add(new JLabel("Money Requested: "));
			myPanel.add(reqMon);
			myPanel.add(new JLabel("Select a Distric:"));
			myPanel.add(this.districtCombo);
			myPanel.add(new JLabel("Path of the photo"));
			myPanel.add(picPath);

			int result = JOptionPane.showConfirmDialog(null, myPanel, 
					"Please enter the data. Think the project name twice, then it cannot be changed eventually", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
			if (result == JOptionPane.OK_OPTION) {
				
				if(nameField.getText().isEmpty() || descriptionField.getText().isEmpty() || 
					reqMon.getText().isEmpty() || picPath.getText().isEmpty() ) {
					
					JOptionPane.showMessageDialog(this.genPage, "You must fill all the fields. Try again",
													"Create Project - Error", JOptionPane.ERROR_MESSAGE);
				}else {
					try {
						String proponent; 
						if( String.valueOf( this.managerCombo.getSelectedItem() ).contentEquals("Create as individual") ) {
							proponent = this.genPage.getCurrentUser();
						}else {
							proponent = String.valueOf( this.managerCombo.getSelectedItem() );
							flag = true;
						}
						
						User user = system.getSingleUser(this.genPage.getCurrentUser());
						
						// The project constructor, already, store the new project in the system
						Project p = this.genPage.getGui().getController().InfrastructureProjectController(nameField.getText(), proponent, descriptionField.getText()
									, Double.valueOf( reqMon.getText() ) , String.valueOf(this.districtCombo.getSelectedItem() )
									, picPath.getText() );
						
						JOptionPane.showMessageDialog(this.genPage, "You have created a new project correctly",
								"Create project - Success", JOptionPane.INFORMATION_MESSAGE);
						
						// Notify to the User
						if(!flag) notifyNewProject(p.getTitle(), p.getDescription(), p.getiD()); 									//You are an individual
						else notifyNewProject(p.getTitle(), p.getDescription(), p.getiD(), system.getSingleGroup(proponent));		//You are a manager
						
						// Updates the list on the screen in run Time
						this.genPage.setNotificationListModel(this.genPage.getGui().getController().updateList(GeneralPage.NOTIFICATION));
						this.genPage.setProjectListModel(this.genPage.getGui().getController().updateList(GeneralPage.PROJECT));
						this.genPage.setSearchProjectListModel(this.genPage.getGui().getController().updateList(GeneralPage.SEARCH_PROJECT));

						
					}catch(NumberFormatException e2) {
						JOptionPane.showMessageDialog(this.genPage, "Introduce a number in the money Requested field",
								"Create Project - Ilegal Argument for Money", JOptionPane.ERROR_MESSAGE);
					}catch(IllegalArgumentException e2) {
						JOptionPane.showMessageDialog(this.genPage, "Name or description is too long\n"
								+ "or the money requested is not in the range 1.000 ~ 1.000.000 . Try again",
								"Create project - Error", JOptionPane.ERROR_MESSAGE);
					}
				}
			}

		});

		createSocProjectButton.addActionListener(crtSoc->{
			Boolean flag = false;
			JTextField nameField = new JTextField(25);
			JTextField descriptionField = new JTextField(50);
			JTextField reqMon = new JTextField(20);
			String[] aux = {Scope.INTERNATIONAL.name(), Scope.NATIONAL.name()};
			JComboBox<String> scopeK= new JComboBox<String>(aux);
			this.managerCombo = new JComboBox<String>();
			JTextField addr = new JTextField(50);
		
			//Load the District into the comboBox
			this.updateManagerBox();

			JPanel myPanel = new JPanel(new GridLayout(6,6,5,5));			// Creates a matrix
			
			myPanel.add(new JLabel("Do you want to create as a manager?   If yes, then select a group"));
			myPanel.add(this.managerCombo);
			myPanel.add(new JLabel("Project Title"));
			myPanel.add(nameField);
			myPanel.add(new JLabel("Description"));
			myPanel.add(descriptionField);	
			myPanel.add(new JLabel("Money Requested"));
			myPanel.add(reqMon);
			myPanel.add(new JLabel("Select a Scope:"));
			myPanel.add(scopeK);
			myPanel.add(new JLabel("To What is this project addressed to: "));
			myPanel.add(addr);
			

			int result = JOptionPane.showConfirmDialog(null, myPanel, 
					"Please enter the data. Think the project name twice, then it cannot be changed eventually", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
			if (result == JOptionPane.OK_OPTION) {
				
				if(nameField.getText().isEmpty() || descriptionField.getText().isEmpty() || 
					reqMon.getText().isEmpty() || addr.getText().isEmpty() ) {
					
					JOptionPane.showMessageDialog(this.genPage, "You must fill all the fields. Try again",
													"Create Project - Error", JOptionPane.ERROR_MESSAGE);
				}else{
					
					Scope ms; // Parse the scope enum
					if (String.valueOf(scopeK.getSelectedItem()).contentEquals(Scope.NATIONAL.name())) 
						ms = Scope.NATIONAL;
					else
						ms = Scope.INTERNATIONAL;
					
					String proponent; 
					if( String.valueOf( this.managerCombo.getSelectedItem() ).contentEquals("Create as individual") ) {
						proponent = this.genPage.getCurrentUser();
					}else {
						proponent = String.valueOf( this.managerCombo.getSelectedItem() );
						flag = true;
					}
					
					try {					
						User user = system.getSingleUser(this.genPage.getCurrentUser());
						
						// The project constructor, already, store the new project in the system
						Project p = this.genPage.getGui().getController().socialProjectController(nameField.getText(), proponent, descriptionField.getText()
									, Double.valueOf( reqMon.getText() ) , addr.getText(), ms );
						
						JOptionPane.showMessageDialog(this.genPage, "You have created a new project correctly",
								"Create project - Success", JOptionPane.INFORMATION_MESSAGE);
						
						// Notify to the User
						if(!flag) notifyNewProject(p.getTitle(), p.getDescription(), p.getiD()); 									//You are an individual
						else notifyNewProject(p.getTitle(), p.getDescription(), p.getiD(), system.getSingleGroup(proponent));		//You are a manager
						
						// Updates the list on the screen in run Time
						this.genPage.setNotificationListModel(this.genPage.getGui().getController().updateList(GeneralPage.NOTIFICATION));
						this.genPage.setProjectListModel(this.genPage.getGui().getController().updateList(GeneralPage.PROJECT));
						this.genPage.setSearchProjectListModel(this.genPage.getGui().getController().updateList(GeneralPage.SEARCH_PROJECT));

						
					}catch(NumberFormatException e2) {
						JOptionPane.showMessageDialog(this.genPage, "Introduce a number in the money Requested field",
								"Create Project - Ilegal Argument for Money", JOptionPane.ERROR_MESSAGE);
					}catch(IllegalArgumentException e2) {
						JOptionPane.showMessageDialog(this.genPage, "Name or description is too long\n"
								+ "or the money requested is not in the range 1.000 ~ 1.000.000 . Try again",
								"Create project - Error", JOptionPane.ERROR_MESSAGE);
					}
				}
			}
		});
		
		
		JPanel n3 = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		n3.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1) );
		n3.add(createInfProjectButton);
		n3.add(createSocProjectButton);
		n.add(n3);
		
		this.add(n, BorderLayout.NORTH);

		//Generate the central part
		c = new JPanel(new GridBagLayout());

		GridBagConstraints constraints = new GridBagConstraints();
		constraints.gridx = 0; 
		constraints.gridy = 0; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
		constraints.fill =  GridBagConstraints.HORIZONTAL;
		constraints.anchor = GridBagConstraints.PAGE_START;
		constraints.weightx = .05;
		constraints.weighty = .05;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		
		
		this.title.setText("Project name: Error - Select an existing project");
		this.daysLeft.setText("Select a project");
		this.proponent.setText("Proponent: Select a project" );
		this.requested.setText("Amount of money requested: Select a project" );
		this.popRep.setText("Popularity report: Select a project" );
		this.description.setText("Description: Select a project");	
		this.district.setText("");
		this.scope.setText("");
		this.addr.setText("");

		constraints.insets = new Insets(8,20,0,0);

		constraints.gridy = 1;
		Font myFont = new Font("Sans Regular", Font.BOLD, 20);
		this.title.setFont(myFont);
		c.add(this.title, constraints);
		
		constraints.gridy = 2; 
		c.add(this.daysLeft, constraints);

		constraints.gridy = 3; 
		c.add(proponent, constraints);

		constraints.gridy = 4;
		c.add(this.requested, constraints);

		constraints.gridy = 5;
		c.add(this.minVotes, constraints);
		
		constraints.gridy = 6;
		c.add(this.popRep, constraints);
		
		constraints.gridy = 7;
		c.add(this.description, constraints);
		
		constraints.gridy = 8; 
		c.add(this.district, constraints);
			
		constraints.gridy = 9;
		c.add(this.scope, constraints);
		
		constraints.gridy = 10; 
		c.add(this.addr, constraints);
		
		constraints.gridy = 11;
		constraints.gridx = 0;
		constraints.weightx = 1.0;
		constraints.weighty = 1.0;
		c.add(this.picLabel, constraints);
	
		this.cityHallPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 3, 3)); //Rows, columns, separation between rows, separation columns
		this.cityHallButton = new JButton("Send to City Hall");
		this.cityHallPanel.add(this.sendCityHall);
		this.cityHallPanel.add(this.cityHallButton);
		
		this.cityHallButton.addActionListener(cHallButton->{
			
			User currentUser = system.getSingleUser(genPage.getCurrentUser());
			ArrayList<Group> managerGroups = new ArrayList<>();
			boolean flag = false;
			
			for(Group g: currentUser.getMembership()) {
				
				if(g.getManager().equals(currentUser)) {
					managerGroups.add(g);
				}
				
			}
			
			try {
				String currentProjectString = genPage.getProjectList().getSelectedValue();
				Project currentProject = system.getSingleProject(currentProjectString);
				
				if(currentProject.checkNumberOfVotes()) {
					
					if(currentUser.getNickname().equals(currentProject.getProponent())) {
						flag = true;
						
					}else {
						
						for(Group g: managerGroups) {
							if(g.getName().equals(currentProject.getProponent())) {
								flag = true;
								break;
							}
							
						}
					}
					
				}
				
				if(flag) {
					try {
						currentProject.sendExternalAssociation();
						
						JOptionPane.showMessageDialog(this.genPage, "The project has been sent to the City Hall succesfully",
								"Send to City Hall - Success", JOptionPane.INFORMATION_MESSAGE);
						this.notifyCityHall(currentProject);
						
					}catch(InvalidRequestException | IOException e) {
						JOptionPane.showMessageDialog(this.genPage, "There was an error trying to communicate with the City Hall servers, try again",
								"Send to City Hall - Error", JOptionPane.ERROR_MESSAGE);
					}
					
				}else {
					JOptionPane.showMessageDialog(this.genPage, "The selected project must have reached the minimum number of votes.\n"
							+ "Or you or a group you are manager of, must be the proponent of the project to perform this action",
							"Send to City Hall - Error", JOptionPane.ERROR_MESSAGE);
				}
				
			}catch(NullPointerException e) {
				JOptionPane.showMessageDialog(this.genPage, "You must select a project to perform this action",
						"Send to City Hall - Error", JOptionPane.ERROR_MESSAGE);
			}

		});
		
		constraints.gridy = 12; 
		c.add(this.cityHallPanel, constraints);
		
		constraints.insets = new Insets(0,0,0,0);
		constraints.gridx = 0; 
		constraints.gridy = 13; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 5; 
        constraints.anchor = GridBagConstraints.PAGE_END;	
        constraints.weightx = 0.10;
		constraints.weighty = 0.10;
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);
		c.setVisible(true);

        this.add(c,BorderLayout.CENTER);
		
		//Generates the south part
		down = new JPanel(new FlowLayout(FlowLayout.LEFT));
		this.result.setText("Select a group to display its information"); 
		down.add(result);
		down.setBorder(BorderFactory.createEmptyBorder(1, 20, 1, 1) );
		this.add(down, BorderLayout.SOUTH);
		this.setVisible(true);
		
        //Updates the Group, when you chose one
        this.genPage.getProjectList().getSelectionModel().addListSelectionListener(listAc->{
        	String currentProject = genPage.getProjectList().getSelectedValue();
        	updateProject(currentProject); // Updates the project page
        });
	}
	
	/**
	* This method updates the list of voted projects in the application
	* 
	* @param pr project you want to interact to
	*/
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
			this.requested.setText("Amount of money requested: " + actualProject.getMoneyRequested() + "€");
			this.minVotes.setText("Minimum number of votes: " + actualProject.getMinNumVotes() );
			this.popRep.setText("Popularity report: " + actualProject.getPopularityReport() );
			this.description.setText("Description: " + actualProject.getDescription());
			
			if(actualProject instanceof InfrastructureProject) {//Infrastructure project
				InfrastructureProject infrastructureProject = (InfrastructureProject)actualProject;
				
				this.district.setText("District: " + infrastructureProject.getDistrict());
				this.title.setText("Project title: " + actualProject.getTitle() + " - Infrastructure");
				this.scope.setText(""); //Clean the other field
				this.addr.setText(""); //Clean the other field
				
				try {
					this.imageIcon = ImageIO.read(new File(infrastructureProject.getPicturePath()));
				} catch (IOException ex) {
					System.out.println("Error. File not found.");
					try {
						this.imageIcon = ImageIO.read(new File("null.png"));
						} catch (IOException ex2) {}
				}
				
				this.picLabel.setIcon(new ImageIcon(this.imageIcon));
				
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
					this.imageIcon = ImageIO.read(new File("null-social.png"));
				} catch (IOException ex) {
					System.out.println("Error. File not found.");
				}
				this.picLabel.setIcon(new ImageIcon(this.imageIcon));	
			}
		}catch(NullPointerException e) {
			this.result.setText("Select a group to display its information"); 
			this.title.setText("Project name: Error - Select an existing project");
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
				this.imageIcon = ImageIO.read(new File("null-social.png"));
			} catch (IOException ex) {System.out.println("Error. File not found.");}
			
			this.picLabel.setIcon(new ImageIcon(this.imageIcon));
		}
	}
	
	/**
	* This method updates the districts combo box that appears when creating an
	* infrastructure project
	*/
	private void updateDistricBox() {
		
		MySystem system = MySystem.getInstance();
		DefaultComboBoxModel<String> aux = new DefaultComboBoxModel<String>();
        
		try {
        	ArrayList<String> allDistrict = new ArrayList<>( system.getDistricts() ); // Work with a copy
  
            for(String s: allDistrict) {
               aux.addElement(s);  	
            }
        	
        	
		}catch(NullPointerException e) {
			aux.addElement("No district");
        }
		
		this.districtCombo.setModel(aux);
		
	}
	
	/**
	* This method updates the combo box of that shows the ways a project can
	* be created. Particularly it adds all the groups the current user is manager of
	*/
	protected void updateManagerBox() {
		
		MySystem system = MySystem.getInstance();
		DefaultComboBoxModel<String> aux = new DefaultComboBoxModel<String>();
		User currentUser = system.getSingleUser(this.genPage.getCurrentUser());
        
		try {
			aux.addElement("Create as individual");
            for(Group g: currentUser.getMembership() ) {
            	if(currentUser.equals(g.getManager()))    // just add it if you are the manager
            		aux.addElement( g.getName() );  	
            }
        	
        	
		}catch(NullPointerException e) {
			aux.addElement("No district");
        }
		
		this.managerCombo.setModel(aux);
		
	}
	
	/**
	* This method sends a notification to the creator when a project is created with its corresponding
	* ID, that can be used to check its state
	* 
	* @param title of the project
	* @param desc of the project
	* @param id of the project
	*/
	protected void notifyNewProject(String title, String desc, int id) {		
		MySystem sys = MySystem.getInstance();
		ArrayList<User> to = new ArrayList<User>();
		to.add(sys.getSingleUser(this.genPage.getCurrentUser()));
		this.genPage.getGui().getController().notify( ("New Project: " + title ) ,
				("You have created a new project: "+title + " with the description: " +
				desc+ "To check the status, you must use this ID: "+ id+
				"<br> When you send your project to the CityHall you will receive a new ID"),
				to );
	}
	
	/**
	* This method sends a notification when a project is created as manager of a group
	* to the creator and all members that has the notifications activated. It also sends
	* its corresponding ID, that can be used to check its state
	* 
	* @param title of the project
	* @param desc of the project
	* @param id of the project
	* @param g group that created the project
	*/
	protected void notifyNewProject(String title, String desc, int id, Group  g) {		
		MySystem sys = MySystem.getInstance();
		
		ArrayList<User> to = new ArrayList<User>(); // Send to all members in the group
		for(User u: g.getArrayUsers()) {
			if(!g.getManager().equals(u))
				to.add(u);
		}
		
		to.add(sys.getSingleUser(this.genPage.getCurrentUser()));
		this.genPage.getGui().getController().notify( ("New Project: " + title ) ,
				("You have voted to a project "+title + "with the description: " +
				desc+ " by belonging to: " +g.getName() 
				 +"We hope you get what you want !!"), to );
		
		ArrayList<User> toM = new ArrayList<User>();
		to.add(g.getManager());
		this.genPage.getGui().getController().notify( ("New Project: " + title ) ,
				("You have created a new project: "+title + "with the description: " +
				desc+ "<br> To check the status, you must use this ID: "+ id+
				"<br> When you send your project to the CityHall you will receive a new ID"),
				toM , true); // Special parameter true, will send this notification to the manager, no matter what
	}
	
	/**
	 * This method allows the user to send the project
	 * he is manager of, to the city hall
	 * 
	 * @param p project the user wants to send to the city hall
	 * */
	private void notifyCityHall(Project p) {
		MySystem sys = MySystem.getInstance();
		
		ArrayList<User> to = new ArrayList<User>();
		to.add(sys.getSingleUser(this.genPage.getCurrentUser()));
		
		this.genPage.getGui().getController().notify( ("City Hall: " + p.getTitle() ) ,
				("You have ask to the CityHall and they will give you an answer <br> in a maximun place  " +
				"of 7 days, be clicking in the same button with this ID: " +p.getExternalID() +
				"<br> you could see the amount of money that you have recieve. Good Luck!!!!"),
				to, true );	
	}
	
	/**
	 * This method returns to the user if his project
	 * has been accepted by the city hall or not. 
	 * 
	 * @param p the project the user wants to check
	 * */
	private void notifySearch(Project p) {
		MySystem sys = MySystem.getInstance();
		
		ArrayList<User> to = new ArrayList<User>();
		to.add(sys.getSingleUser(this.genPage.getCurrentUser()));
		
		try {
			Double aux = p.getMoneyGranted();
			this.genPage.getGui().getController().notify( ("Searh porject: " + p.getTitle() ) ,
					("<br> Congratulations, you have earned " +aux + "<br>"
					+"We hope you can use this money to enhance the world!!!"),
					to, true );
		}catch(NullPointerException e1) {
			this.genPage.getGui().getController().notify( ("Search porject: " + p.getTitle() ) ,
					("<br> We are sorry, but your project is still pending, remenber that they could"
					+ "spend 7 days thinking about your project, so please be pacient and have "
					+ "self confidence, everything is going to be allright. <br> <br> We encourage you to "
					+ "try again in a couple of hours "),
					to, true );
		}
	}
	

	
}
