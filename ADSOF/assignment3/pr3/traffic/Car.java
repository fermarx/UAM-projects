package pr3.traffic;

import pr3.traffic.drivers.Person;
import pr3.traffic.drivers.license.PermitKind;

/* 
 * Car class that creates a vehicle of type car.
 * 
 * @author Jose Manuel Freire and Sofia Xiaofan Fernandez Marin jose.freire@estudiante.uam.es sofiax.fernandez@estudiante.uam.es
 * 
 */
public class Car extends Vehicle {
	private boolean diesel;
	
	/*
	 * Constructor of the class car
	 * 
	 * @param mod model of the truck
	 * @param a year of fabrication
	 * @param diesel if the car is diesel or not
	 */
	public Car(String mod, int a, boolean diesel) {
		super(mod, a);
		this.diesel = diesel;
	}
	
	/*
	 * Constructor of the class car
	 * 
	 * @param mod model of the truck
	 * @param a year of fabrication
	 * @param plate plate of the car
	 * @param diesel if the car is diesel or not
	 */
	public Car(String mod, int a, String plate, boolean diesel) {
		super(mod, a, plate);
		this.diesel = diesel;
	}

	public Car(String mod, int year, String plate, boolean diesel, Person person) {
		super(mod, year, plate, person, PermitKind.B);
		this.diesel = diesel;
		person.setOwns(this);
	}

	/*
	 * @return number of wheels that a car has 
	 */
	@Override public int numWheels() { return 4; }


	/*
	 * @return a string describing the car 
	 */
	@Override public String toString() {
		if (this.diesel == true)
			return "Car diesel, " + super.toString();
				
		return "Car gasoline, "+ super.toString();
	}
	
	/*
	 * @return the index of pollution of the car
	 */
	@Override
	public PollutionIndex getPollutionIndex() {
		if (this.diesel) return PollutionIndex.C;
		return super.getPollutionIndex();
	}
	
	/*
	 * @return true if the car is diesel
	 */
	public boolean getDiesel() { return this.diesel; }
	
	/*
	 * @param b makes the car diesel or not
	 */
	public void setDiesel(boolean b) { this.diesel = b; }

	@Override public void setKind(){
		super.kind = PermitKind.B;
	}
	
}
