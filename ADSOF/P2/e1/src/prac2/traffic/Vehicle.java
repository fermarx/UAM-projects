package prac2.traffic;

/* 
 * Abstract class vehicle
 * 
 * @author Jose Manuel Freire and Sofia Xiaofan Fernandez Marin jose.freire@estudiante.uam.es sofiax.fernandez@estudiante.uam.es
 * 
 */
public abstract class Vehicle {
	private String model;
	private int purchaseYear;
	private String plate;
	
	/*
	 * Constructor of the class vehicle
	 * 
	 * @param mod model of the vehicle
	 * @param a year of fabrication
	 */
	public Vehicle(String mod, int a) {
		this.model = mod;
		this.purchaseYear = a;
	}
	
	/*
	 * Constructor of the class vehicle
	 * 
	 * @param mod model of the vehicle
	 * @param a year of fabrication
	 * @param plate plate of the vehicle
	 */
	public Vehicle(String mod, int a, String plate) {		
		this.model = mod;
		this.purchaseYear = a;
		this.plate = plate;
	}
	
	/*
	 * @return a string describing the vehicle 
	 */
	@Override
	public String toString() {
		return "model "+this.model+", number plate: "+this.plate+", purchase year "+this.purchaseYear+", with "+
				this.numWheels()+" wheels, index:"+this.getPollutionIndex();
	}
	
	/*
	 * @return number of wheels of the vehicle 
	 */
	public abstract int numWheels();
	
	/*
	 * @return the index of pollution of the vehicle
	 */
	public PollutionIndex getPollutionIndex() {
		return PollutionIndex.getPollutionIndex(this.purchaseYear);
	}

	/*
	 * @return number of plate of the vehicle 
	 */
	public String getPlate() {
		return plate;
	}


	/*
	 * @param plate number of plate of the vehicle 
	 */
	public void setPlate(String plate) {
		this.plate = plate;
	}
}
