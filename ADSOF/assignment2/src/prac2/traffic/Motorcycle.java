package prac2.traffic;

public class Motorcycle extends Vehicle {
	private boolean electric;

	public Motorcycle(String mod, int year, String plate, boolean electric) {
		super(mod, year, plate);
		this.setElectric(electric);
	}

	@Override
	public int numWheels() {
		return 2;
	}


	@Override 
	public String toString() {
		return "Motorcycle" + (this.electric ? " electric, " : ", ") + super.toString();
	}

	public boolean isElectric() {
		return electric;
	}

	public void setElectric(boolean electric) {
		this.electric = electric;
	}

}
