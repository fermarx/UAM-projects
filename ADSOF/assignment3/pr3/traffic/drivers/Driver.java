package pr3.traffic.drivers;
import java.util.ArrayList;
import java.util.List;

import pr3.traffic.Vehicle;

public class Driver {
	private String name;
	private List<Vehicle> owns = new ArrayList<>();

	public Driver(String name) {
		this.name = name;
	}
	
	/*
	 * @return a string describing the driver
	 */
	@Override public String toString() {
		if (owns == null)
			return this.name;
		
		String ret = " ";
		ret =  this.name + " owner of:\n";
		
		for (Vehicle v: this.owns)
			ret += v.toString() + "\n";
		
		return ret;
	}


	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public List<Vehicle> getOwns() {
		return owns;
	}

	public void setOwns(Vehicle owns) {
		this.owns.add(owns);
	}

}
