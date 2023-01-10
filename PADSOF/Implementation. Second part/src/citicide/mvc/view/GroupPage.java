package citicide.mvc.view;

import java.awt.*;
import java.util.*;
import javax.swing.*;
import citicide.*;

/**
* This is the GroupPage class from the citicide.mvc.view package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/

public class GroupPage extends JPanel{
	
	private static final long serialVersionUID = 1L;
	private final String managerText = "Manager of the group: None group selected";
	private final String descriptionText = "Description: None group selected";
	private final String projectsText = "Voted projects: None group selected";
	private final String noAvailable = "No groups available";
	private final String noTitle = "Group name: Error - You must select an existing group";
	
	protected GeneralPage genPage;
	private JComboBox<String> affCom;
	protected JLabel manager = new JLabel("");
	protected JLabel description = new JLabel("");
	protected JLabel projects = new JLabel("");
	protected JLabel title = new JLabel("");
	protected JButton create;
	protected JPanel c;
	
	/**
	* This is the constructor class of GroupPage
	* 
	* @param genPage general page the class GroupPage is using
	*/
	public GroupPage(GeneralPage genPage) {
		MySystem system = MySystem.getInstance();
		
		this.genPage = genPage;	
		
		this.setLayout(new BorderLayout());
	
		//Generate the north part
		JPanel n = new JPanel(new GridLayout(0,3,10,10));
		create = new JButton("Create group");

		create.addActionListener(crt->{
			
			JTextField nameField = new JTextField(20);
			JTextField descriptionField = new JTextField(50);

			JPanel myPanel = new JPanel();
			myPanel.add(new JLabel("Group name"));
			myPanel.add(nameField);
			myPanel.add(Box.createHorizontalStrut(15)); // a spacer
			myPanel.add(new JLabel("Description"));
			myPanel.add(descriptionField);

			int result = JOptionPane.showConfirmDialog(null, myPanel, 
					"Please enter the data. Think the group name twice, then it cannot be changed", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);
			if (result == JOptionPane.OK_OPTION) {
				
				if(nameField.getText().isEmpty() || descriptionField.getText().isEmpty()) {
					JOptionPane.showMessageDialog(this.genPage, "You must fill all the fields. Try again",
													"Create group - Error", JOptionPane.ERROR_MESSAGE);
				}else {
					try {					
						User user = system.getSingleUser(this.genPage.getCurrentUser());
						
						Group group = this.genPage.getGui().getController().groupController(nameField.getText(), user, descriptionField.getText());
						JOptionPane.showMessageDialog(this.genPage, "You have created the group correctly",
								"Create group - Success", JOptionPane.INFORMATION_MESSAGE);
						
						//Notify the user
						this.notifyNewGroup(group.getName(), group.getDescription());
						
						// Updates the list on the screen in run Time
						this.genPage.setGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.GROUP));
						this.genPage.setNotificationListModel(this.genPage.getGui().getController().updateList(GeneralPage.NOTIFICATION));
						this.genPage.setSearchGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.SEARCH_GROUP));
						this.updateAff();
						
					}catch(IllegalArgumentException e) {
						JOptionPane.showMessageDialog(this.genPage, "Name or description is too long. Try again",
														"Create group - Error", JOptionPane.ERROR_MESSAGE);
					}
				}
			}
			
		});
		
		// Initializes the north part
		JPanel n1 = new JPanel(new FlowLayout(FlowLayout.LEFT));
		n1.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 1) );
		n1.add(create);
		n1.add(new JLabel(""));
		n.add(n1);
		
		JPanel n2 = new JPanel(new FlowLayout(FlowLayout.CENTER));
					
		n2.add(new JLabel(" "));
		n.add(n2);
		
		JButton uns = new JButton("Leave group");
		JButton createSub = new JButton("Create subgroup");
		
		uns.addActionListener(unsubcribe->{
			
			try {
				String groupName = genPage.getGroupList().getSelectedValue(); //Group that is selected in this moment
				Group group = system.getSingleGroup(groupName);
				User user = system.getSingleUser(this.genPage.getCurrentUser());
				
				if(group.isMember(user)) {
					if(group.leaveGroup(user)) {
						JOptionPane.showMessageDialog(this.genPage, "You have unsubscribe correctly from the group",
								"Unsubscribe from group - Success", JOptionPane.INFORMATION_MESSAGE);
						
						// Updates the list on the screen in run Time
						this.genPage.setGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.GROUP));
						this.genPage.setSearchGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.SEARCH_GROUP));
						this.updateAff();
					}else {
						JOptionPane.showMessageDialog(this.genPage, "You cannot leave a group you are manager of",
								"Unsubscribe from group - Error", JOptionPane.ERROR_MESSAGE);
					}
					
				}else {
					JOptionPane.showMessageDialog(this.genPage, "You must be member of the group to perform this action",
							"Unsubscribe from group - Error", JOptionPane.ERROR_MESSAGE);
				}
				
			}catch(NullPointerException e1) {
				JOptionPane.showMessageDialog(this.genPage, "You do not have any group, you must be member of one to perform this action",
												"Unsubscribe from group - Error", JOptionPane.ERROR_MESSAGE);
			}
		});
		
		createSub.addActionListener(sub->{
			
			try {
				String groupName = genPage.getGroupList().getSelectedValue(); //Group that is selected in this moment
				Group group = system.getSingleGroup(groupName);	

				if(group.isManager(this.genPage.getCurrentUser())) {
					
					JTextField nameField = new JTextField(20);
					JTextField descriptionField = new JTextField(50);
	
					JPanel myPanel = new JPanel();
					myPanel.add(new JLabel("Group name"));
					myPanel.add(nameField);
					myPanel.add(Box.createHorizontalStrut(15)); // a spacer
					myPanel.add(new JLabel("Description"));
					myPanel.add(descriptionField);
	
					int result = JOptionPane.showConfirmDialog(null, myPanel, 
							"Please enter the data. Think the subgroup name twice, then it cannot be changed", JOptionPane.OK_CANCEL_OPTION);
					if (result == JOptionPane.OK_OPTION) {
						if(nameField.getText().isEmpty() || descriptionField.getText().isEmpty()) {
							JOptionPane.showMessageDialog(this.genPage, "You must fill all the fields. Try again",
															"Create subgroup - Error", JOptionPane.ERROR_MESSAGE);
						}else {
							try {
								Subgroup subgroup = this.genPage.getGui().getController().subgroupController(nameField.getText(), group.getManager(), descriptionField.getText(), group);
								JOptionPane.showMessageDialog(this.genPage, "You have created the subgroup correctly",
										"Create subgroup - Success", JOptionPane.INFORMATION_MESSAGE);
								
								// Updates the list on the screen in run Time
								this.genPage.setGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.GROUP));
								this.genPage.setSearchGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.SEARCH_GROUP));
								this.updateAff();
								
							}catch(IllegalArgumentException e) {
								JOptionPane.showMessageDialog(this.genPage, "Name or description is too long. Try again",
																"Create subgroup - Error", JOptionPane.ERROR_MESSAGE);
							}
						}
					}
					
				}else {
					JOptionPane.showMessageDialog(this.genPage, "You must be the manager of the group\n to be able to create a subgroup",
													"Create subgroup - Error", JOptionPane.ERROR_MESSAGE);
				}
			
			}catch(NullPointerException e1) {
				JOptionPane.showMessageDialog(this.genPage, "You do not have any group, so you cannot create subgroups",
												"Create subgroup - Error", JOptionPane.ERROR_MESSAGE);
			}
			
		});
		
		JPanel n3 = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		n3.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 1) );
		n3.add(uns);
		n3.add(createSub);
		n.add(n3);
		n.setVisible(true);
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
		
		constraints.insets = new Insets(0,20,0,0);
		constraints.gridx = 0; 
		constraints.gridy = 1; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;
		this.title.setText(this.noTitle);
		Font myFont = new Font("Sans Regular", Font.BOLD, 20);
		this.title.setFont(myFont);
		c.add(this.title, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 2; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1;  
		this.manager.setText(this.managerText);
		c.add(this.manager, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 3; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 5; 
	    constraints.weightx = .35;
	    constraints.weighty = .35;
	    this.description.setText(this.descriptionText);
		c.add(this.description, constraints);
		
		constraints.gridx = 0; 
		constraints.gridy = 11; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.weightx = 1.0;
        constraints.weighty = 1.0;
        this.projects.setText(this.projectsText);
		c.add(this.projects, constraints);
		
		constraints.insets = new Insets(0,0,0,0);
		constraints.gridx = 0; 
		constraints.gridy = 12; 
		constraints.gridwidth = 1; 
		constraints.gridheight = 1; 
        constraints.anchor = GridBagConstraints.PAGE_END;	
		c.add(new JSeparator(SwingConstants.HORIZONTAL), constraints);

		//Generates the south part
		JPanel down = new JPanel(new FlowLayout(FlowLayout.LEFT));
        
        JLabel aff = new JLabel("Affinity report with group: ");
        aff.setForeground(Color.MAGENTA.darker());
        down.add(aff);
        
        String[] affNames;
        try {
        	String groupName = genPage.getGroupList().getSelectedValue(); //Group that is selected in this moment
        	ArrayList<Group> groups = new ArrayList<Group>( system.getGroups() ); // Work with a copy
        	Group actualGroup = system.getSingleGroup(groupName);
        	
        	groups.remove(actualGroup); //Remove the group where the user is from the list
        	
        	if(groups.size() == 0) {
        		affNames = new String[] {this.noAvailable};
        	}else {
        		affNames = new String[groups.size()];
        		
        		int i = 0;
                for(Group g: groups) {
                	affNames[i] = g.getName();
                	i++;
                }
        	}
        	
        }catch(NullPointerException e) {
        	affNames = new String[] {this.noAvailable};
        }
        
        affCom = new JComboBox<String>(affNames);
        JLabel result= new JLabel("");
        
        affCom.addActionListener(allCombos->{
        	String groupN1Name = genPage.getGroupList().getSelectedValue();
			
			try {
				Group groupN1 = system.getSingleGroup(groupN1Name);
				String groupN2Name = (String)affCom.getSelectedItem();
				
				try {
					
					Group groupN2 = system.getSingleGroup(groupN2Name);
					double affinityReport = groupN1.affinityReport(groupN2);
					result.setText(Double.toString(affinityReport));
	
				}catch(NullPointerException e1) {
					JOptionPane.showMessageDialog(this.genPage, "There must be more than one group in the application to perform this action.",
							"Affinity report - Error", JOptionPane.ERROR_MESSAGE);
				}
				
			}catch(NullPointerException e1) {
				JOptionPane.showMessageDialog(this.genPage, "You must be inside a group to perform this action",
						"Affinity report - Error", JOptionPane.ERROR_MESSAGE);
			}

        });
        
        down.setBorder(BorderFactory.createEmptyBorder(1, 20, 1, 1) );
        down.add(affCom);
        down.add(result);
        
        this.add(down, BorderLayout.SOUTH);
        c.setVisible(true);
        this.add(c,BorderLayout.CENTER);
        this.setVisible(true);
        
        //Updates the Group, when you chose one
        this.genPage.getGroupList().getSelectionModel().addListSelectionListener(listAc->{
        	String currentgroup = genPage.getGroupList().getSelectedValue();
        	this.updateAff();				// Updates the combo box(of the south part)
        	updateGroup(currentgroup);		// Updates the group page
        });
	}
	
	/**
	 * @return affCom affinity report combo box
	 * */
	public JComboBox<String> getAffCom() {
		return affCom;
	}
	
	/**
	 * This method allows the group page to update the 
	 * page when you select a user
	 * 
	 * @param gr group you want to interact with
	 * */
	protected void updateGroup(String gr) { 
		
		try {
			MySystem  sys = MySystem.getInstance();
			String groupName = gr; 							   //Group that is selected in this moment
			Group actualGroup = sys.getSingleGroup(groupName); //Returns null if this group does not exists, this means that there are no groups in system
			
			this.manager.setText( "Manager of the group: " + actualGroup.getManager().getNickname());
			this.description.setText( "<html><p style=\"width:750px\">"+ "Descripcion: <br> <br>" + actualGroup.getDescription() +"</p></html>");
			
			if(actualGroup.getGroupVote().size() == 0 ) {
				this.projects.setText("Voted projects: This group does not voted projects yet" );
			}else {
				this.projects.setText("Voted projects: " + actualGroup.getGroupVote() );				
			}
			if(actualGroup.getLevel() == 0) { //It is a father group (Not a subgroup)
				this.title.setText("Group name: " + actualGroup.getName());
				
			}else { //It is a subgroup of any other group
				this.title.setText("Group name: " + actualGroup.getName() + " - Subgroup of \"" + actualGroup.getFather().getName() + "\"");
			}
			
		}catch(NullPointerException e) {
			this.manager.setText(this.managerText);
			this.description.setText(this.descriptionText);
			this.projects.setText(this.projectsText);
			Font myFont = new Font("Sans Regular", Font.BOLD, 20);
			this.title.setFont(myFont);
			this.title.setText("Group name: Error - You must select an existing group");
		}
		
	}
	
	/**
	 * This method allows the group page to update the 
	 * affinity report
	 * */
	protected void updateAff() {
		MySystem system = MySystem.getInstance();
		DefaultComboBoxModel<String> aux = new DefaultComboBoxModel<String>();
		String currentgroup = genPage.getGroupList().getSelectedValue();
        
		try {
        	ArrayList<Group> groups = new ArrayList<>( system.getGroups() ); // Work with a copy
        	Group actualGroup = system.getSingleGroup(currentgroup);
        	
        	groups.remove(actualGroup); //Remove the group where the user is from the list
        	
        	if(groups.size() == 0) {
        		aux.addElement(this.noAvailable);
        	}else {
	            for(Group g: groups) {
	               aux.addElement(g.getName() );  	
	            }
        	}
        	
		}catch(NullPointerException e) {
			aux.addElement(this.noAvailable);
        }
		
		this.affCom.setModel(aux);
		
	}
	
	/**
	 * Sends a notification to the manager of the group if he
	 * created a new group
	 * 
	 * @param name name of the group the user has created
	 * @param desc description of the group the user has created
	 * */
	protected void notifyNewGroup(String name, String desc) {
		MySystem sys = MySystem.getInstance();
		ArrayList<User> to = new ArrayList<User>();
		to.add(sys.getSingleUser(this.genPage.getCurrentUser()));
		this.genPage.getGui().getController().notify( ("New Group: " + name ) ,
				("You have created a new group: " +name +"with the description: " +
				desc+ "<br> We hope that you will get a lot of user in it"),
				to );
	}

}
