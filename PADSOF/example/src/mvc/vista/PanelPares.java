package mvc.vista;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class PanelPares extends JPanel implements ActionListener {
	private JLabel etiqueta;
	private JTextField campo;
	private JButton boton;
	private GuiSimple gui;
	
	public PanelPares(GuiSimple gui, String etiquetaCampo, String etiquetaBoton) {
		this.gui = gui;	
		this.etiqueta = new JLabel(etiquetaCampo);
		this.campo = new JTextField(10);
		this.boton = new JButton(etiquetaBoton);
		
		// asociar acciones a componentes
		boton.addActionListener( this );
		
		// asignar Layout Manager y añadir componentes 
		this.setLayout(new BorderLayout() );
		this.add(etiqueta, BorderLayout.WEST);
		this.add(campo, BorderLayout.CENTER);
		this.add(boton, BorderLayout.SOUTH);	
	}

	@Override
	public void actionPerformed(ActionEvent ev) {
	  String mensaje = "";
	  try {
		if ( this.gui.getControlador().checkPar( Integer.parseInt( this.campo.getText() )) )
		  mensaje = "Sí, " + this.campo.getText() + " es par";
	    else mensaje = "El número " + this.campo.getText() + " no es par";
	  } catch (NumberFormatException e) {
		  mensaje = this.campo.getText() + "No se ha introducido un número entero válido";
	  } finally {
		  JOptionPane.showMessageDialog(null, mensaje);
	  }
	}
	
	public void limpiaCampo() { this.campo.setText(""); }

}
