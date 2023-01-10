package prac2.traffic;

/* 
 * Motorcycle class that creates a vehicle of type motorcycle.
 * 
 * @author Jose Manuel Freire and Sofia Xiaofan Fernandez Marin jose.freire@estudiante.uam.es sofiax.fernandez@estudiante.uam.es
 * 
 */
public class Motorcycle extends Vehicle {
	private boolean electric;

	/*
	 * Constructor of the class motorcycle
	 * 
	 * @param mod model of the motorbike
	 * @param year year of fabrication
	 * @param plate plate of the motorbike
	 * @param electric if it is electric true
	 */
	public Motorcycle(String mod, int year, String plate, boolean electric) {
		super(mod, year, plate);
		this.setElectric(electric);
	}

	/*
	 * @return number of wheels that a motorcylce has 
	 */
	@Override
	public int numWheels() {
		return 2;
	}


	/*
	 * @return a string describing the motorcycle 
	 */
	@Override 
	public String toString() {
		return "Motorcycle" + (this.electric ? " electric, " : ", ") + super.toString();
	}

	/*
	 * @return if the motorcylce is electric or not 
	 */
	public boolean isElectric() {
		return electric;
	}

	/*
	 * @param electric if the motorcycle is electric or not
	 */
	public void setElectric(boolean electric) {
		this.electric = electric;
	}

}
