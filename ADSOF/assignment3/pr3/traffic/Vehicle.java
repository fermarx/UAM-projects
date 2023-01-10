package pr3.traffic;

import pr3.traffic.drivers.*;
import pr3.traffic.drivers.license.PermitKind;

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
	private Driver owner;
	private Person driver;
	protected PermitKind kind;
	
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
	
	//WITH PERSONS
	public Vehicle(String mod, int year, String plate, Person person, PermitKind kind) {
		this.model = mod;
		this.purchaseYear = year;
		this.plate = plate;
		this.owner = person;
		this.kind = kind;
		if(person.getLicense().getKind().contains(this.kind)) {
			this.driver = person;
		}else {
			this.driver = null;
		}
			
	}

	//WITH COMPANIES
	public Vehicle(String mod, int year, String plate, Company company) {
		this.model = mod;
		this.purchaseYear = year;
		this.plate = plate;
		this.owner = company;
		this.driver = company.getPerson();
	}

	/*
	 * @return a string describing the vehicle 
	 */
	@Override
	public String toString() {
		if(this.driver!=null){
			return "model "+this.model+", number plate: "+this.plate+", purchase year "+this.purchaseYear+", with "+
			this.numWheels()+" wheels, index:"+this.getPollutionIndex() + ", owner: " + this.owner.getName() +
			", driver: "+ this.driver.getName();
		} else {
			return "model "+this.model+", number plate: "+this.plate+", purchase year "+this.purchaseYear+", with "+
			this.numWheels()+" wheels, index:"+this.getPollutionIndex() + ", owner: " + this.owner.getName() +
			", driver: not registered";
		}
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


	public void setOwner(Person owner) {
		this.owner = owner;
		if (owner != null)
			this.driver = owner;
		owner.setOwns(this);
	}

	public Person getDriver() {
		return driver;
	}

	public boolean setDriver(Person driver) {
		if (driver.getAge() >= 18) {
			this.driver = driver;
			return true;
		}
		return false;
	}

	public abstract void setKind();
	
	public PermitKind getKind(){
		return this.kind;
	}
}
