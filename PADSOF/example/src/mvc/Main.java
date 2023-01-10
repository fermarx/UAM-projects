package mvc;
import mvc.controlador.Controlador;
import mvc.modelo.Aplicacion;
import mvc.vista.GuiSimple;

public class Main {
	public static void main(String[] args) {
		GuiSimple gui = new GuiSimple("Una GUI muy simple");
		Aplicacion app = new Aplicacion();
		Controlador  controlador = new Controlador(gui, app);
		gui.setControlador( controlador );
	}

}
