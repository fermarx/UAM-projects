package mvc.vista;
import mvc.controlador.Controlador;
import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;


public class GuiSimple extends JFrame implements ChangeListener {
	private PanelLogin panelLogin;	
	private PanelPares panelPares;		
	private PanelPalindromos panelPalindromos;
	private JTabbedPane pestanas = new JTabbedPane();
	private Controlador controlador;
	
	public GuiSimple(String titulo) {
		super(titulo); // antes: JFrame ventana = new JFrame("Mi GUI");
		
		// obtener contenedor, asignar layout
		Container contenedor = this.getContentPane(); // antes: ventana.getContentPane();
		contenedor.setLayout(new FlowLayout());
		
		// crear componentes
		panelLogin = new PanelLogin(this);
		panelPares = new PanelPares(this, "Numero", "Check par");		
		panelPalindromos= new PanelPalindromos(this, "Cadena", "Pal�ndromo?");
		pestanas.addTab("Pares",  panelPares);
		pestanas.addTab("Pal�ndromos",  panelPalindromos);
		pestanas.setSelectedIndex(1); // 0 means first
		
		// a�adir componentes al contenedor
		contenedor.add(panelLogin);
		contenedor.add(pestanas);
		// this.pack();
		
		// visibilidad inicial
		panelLogin.setVisible( true );
		pestanas.setVisible( false );
		
		// Propuesta: PERMITIR REGRESAR A PANEL LOGIN DESDE CUALQUIER PESTA�A
		// Proposed work: ALLOW RETURN TO PANEL LOGIN FROM ANY TAB
		
		// Para realizar acciones al cambiar de pesta�as
		pestanas.addChangeListener( this );

		// mostrar this, en otros ejemplos era ventana, ahora this
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setSize(500,400); // remove and uncomment this.pack above
		this.setVisible(true);	
	}

	public void setControlador(Controlador c) {
		this.controlador = c;
	}
	public Controlador getControlador() {
		return this.controlador;
	}
	
	
	@Override
	public void stateChanged(ChangeEvent ev) {
     // solamente a efectos de seguimiento del programa
   	 System.out.println( pestanas.getSelectedIndex() );
   	 System.out.println( pestanas.getSelectedComponent() );
   	 this.panelPares.limpiaCampo();
   	 this.panelPalindromos.limpiaCampo();
	}

	public void loginResult(boolean loginOK) {
		if (loginOK) { 
			panelLogin.setVisible( false );
			pestanas.setVisible( true );
		} else {
			this.panelLogin.setError("login incorrecto");
		}
	}
}
