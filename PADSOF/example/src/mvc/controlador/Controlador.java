package mvc.controlador;

import mvc.modelo.Aplicacion;
import mvc.vista.GuiSimple;

public class Controlador {	
	private GuiSimple gui;
	private Aplicacion app;
	
	public Controlador(GuiSimple gui, Aplicacion app) {
		this.gui = gui; this.app = app;
	}
	public boolean checkPar(Integer num) {
		return this.app.par(num);
	}
	public boolean checkPal(String str) {
		return this.app.pal(str);
	}
	public void login(String name, String passwd) {
		this.gui.loginResult( this.app.loginOK( name, passwd ) );
		
	}

}
