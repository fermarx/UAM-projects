package citicide.mvc.view;

import java.awt.*;
import javax.swing.*;
import citicide.*;

/**
* This is the NewGroupPage class from the citicide.mvc.view package
*
* @author Jorge Blanco Rey jorge.blancor@estudiante.uam.es
* @author Angel Casanova Bienzobas angel.casanova@estudiante.uam.es
* @author S. Xiao Fernández Marín sofiax.fernandez@estudiante.uam.es
*
*/
public class NewGroupPage extends GroupPage {
	private static final long serialVersionUID = 1L;
	
	/**
	* This is the constructor method of the NewGroupPage
	*  
	* @param genPage, the general page
	*/
	public NewGroupPage(GeneralPage genPage) {
		super(genPage); // generates a default project page to share the buttons and the north
		MySystem system = MySystem.getInstance();
		this.setLayout(new BorderLayout());
	
		//Generate the north part
		JPanel n = new JPanel(new GridLayout(0,3,10,10));
		
		// Initializes the north part
		JPanel n1 = new JPanel(new FlowLayout(FlowLayout.LEFT));
		n1.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 1) );
		n1.add(this.create);
		n.add(n1);
		
		JPanel n2 = new JPanel(new FlowLayout(FlowLayout.CENTER));
		n2.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 10) );
		n.add(n2);
		
		JButton joinButton = new JButton("Join group");
		
		joinButton.addActionListener(join->{
			
			try {
				String groupName = genPage.getSearchGroupList().getSelectedValue(); //Group that is selected in this moment
				Group group = system.getSingleGroup(groupName);
				User user = system.getSingleUser(this.genPage.getCurrentUser());
				
				if(group.joinGroup(user)) {
					JOptionPane.showMessageDialog(this.genPage, "You have joined the group correctly",
							"Join group - Success", JOptionPane.INFORMATION_MESSAGE);

					// Updates the list on the screen in run Time
					this.genPage.setGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.GROUP));
					this.genPage.setSearchGroupListModel(this.genPage.getGui().getController().updateList(GeneralPage.SEARCH_GROUP));
					
				}else {
					JOptionPane.showMessageDialog(this.genPage, "You are already a member of the selected group",
							"Join group - Error", JOptionPane.ERROR_MESSAGE);
				}
				
			}catch(NullPointerException e1) {
				JOptionPane.showMessageDialog(this.genPage, "You must select a group to perform this action.\nYou can be the first one to create a group!",
												"Join group - Error", JOptionPane.ERROR_MESSAGE);
			}
		});
		
		JPanel n3 = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		n3.setBorder(BorderFactory.createEmptyBorder(1, 10, 1, 1) );
		n3.add(joinButton);
		n.add(n3);
		n.setVisible(true);
		this.add(n, BorderLayout.NORTH);

        this.c.setVisible(true);
        this.add(this.c,BorderLayout.CENTER);
        this.setVisible(true);

        //Updates the Group, when you choose one
        this.genPage.getSearchGroupList().getSelectionModel().addListSelectionListener(listAc->{
        	String currentgroup = genPage.getSearchGroupList().getSelectedValue();
        	this.updateGroup(currentgroup); // Updates the  newGroup page
        });
	}
	
}
