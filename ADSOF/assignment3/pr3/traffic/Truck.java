package pr3.traffic;

import pr3.traffic.drivers.Company;
import pr3.traffic.drivers.license.PermitKind;

/* 
 * Truck class that creates a vehicle of type truck.
 * 
 * @author Jose Manuel Freire and Sofia Xiaofan Fernandez Marin jose.freire@estudiante.uam.es sofiax.fernandez@estudiante.uam.es
 * 
 */
public class Truck extends Vehicle {
	private int axles;

	/*
	 * Constructor of the class truck
	 * 
	 * @param mod model of the truck
	 * @param year year of fabrication
	 * @param plate plate of the truck
	 * @param axles axles that the truck has 
	 */
	public Truck(String mod, int year, String plate, int axles) {
		super(mod, year, plate);
		this.axles = axles;
	}

	public Truck(String mod, int year, String plate, int axles, Company company) {
		super(mod, year, plate, company);
		this.axles = axles;
		company.setOwns(this);
	}

	/*
	 * @return number of wheels that a truck has 
	 */
	@Override
	public int numWheels() {
		return this.axles*2;
	}

	/*
	 * @return a string describing the truck 
	 */
	@Override 
	public String toString() {
		return "Truck with " + this.axles + " axles, "+ super.toString();
	}
	
	@Override public void setKind(){
		super.kind = PermitKind.C1;
	}

}
