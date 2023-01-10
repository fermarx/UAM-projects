package mvc.vista;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class PanelPalindromos extends JPanel implements ActionListener {
	private JLabel etiqueta;
	private JTextField campo;
	private JButton boton;
	private GuiSimple gui;
	
	public PanelPalindromos(GuiSimple gui, String etiquetaCampo, String etiquetaBoton) {
		this.gui = gui;
		this.setLayout(new FlowLayout());
		
		this.etiqueta = new JLabel(etiquetaCampo);
		this.campo = new JTextField(10);
		this.boton = new JButton(etiquetaBoton);
		
		// this.setPreferredSize(new Dimension(400, 200));
		
		// asociar acciones a componentes
		boton.addActionListener( this );
		
		// a�adir componentes al panel
		this.add(etiqueta);
		this.add(campo);
		this.add(boton);
	}

	public void actionPerformed(ActionEvent ev) {
      String mensaje;
	  if ( this.gui.getControlador().checkPal( this.campo.getText() ) )
           mensaje = "Yes, " + this.campo.getText() + " es pal�ndromo";
      else mensaje = "No es pal�dromo. Try again ...";
	  JOptionPane.showMessageDialog(null, mensaje);
	}
    
	public void limpiaCampo() { this.campo.setText(""); };
}
