package prac2.traffic;

public abstract class Vehicle {
	private String model;
	private int purchaseYear;
	private String plate;
	
	public Vehicle(String mod, int a) {
		this.model = mod;
		this.purchaseYear = a;
	}
	
	public Vehicle(String mod, int a, String plate) {		
		this.model = mod;
		this.purchaseYear = a;
		this.plate = plate;
	}
	
	@Override
	public String toString() {
		return "model "+this.model+", number plate: "+this.plate+", purchase year "+this.purchaseYear+", with "+
				this.numWheels()+" wheels, index:"+this.getPollutionIndex();
	}
	
	public abstract int numWheels();
	
	public PollutionIndex getPollutionIndex() {
		return PollutionIndex.getPollutionIndex(this.purchaseYear);
	}

	public String getPlate() {
		return plate;
	}

	public void setPlate(String plate) {
		this.plate = plate;
	}
}
