package prac2.traffic;

public class Truck extends Vehicle {
	private int axles;

	public Truck(String mod, int year, String plate, int axles) {
		super(mod, year, plate);
		this.axles = axles;
	}

	@Override
	public int numWheels() {
		return this.axles*2;
	}

	@Override 
	public String toString() {
		return "Truck with " + this.axles + " axles, "+ super.toString();
	}
	
}
