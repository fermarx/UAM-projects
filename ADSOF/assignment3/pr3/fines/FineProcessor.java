package pr3.fines;

import java.util.ArrayList;
import java.util.List;

import pr3.traffic.Vehicle;

public class FineProcessor {
	List<Vehicle> vehicles;

	public FineProcessor(List<Vehicle> vehicles) {
		this.vehicles = new ArrayList<Vehicle>(vehicles);
	}

	public void process(List<Fine> fines) {
		String driverName;
		Integer lostPoints;
		Integer driverPoints;

		for(Fine fine:fines){
			for(Vehicle vehicle:vehicles){
				if(fine.getPlate().equals(vehicle.getPlate())){
					if(vehicle.getDriver() != null){
						driverName = vehicle.getDriver().getName();
						lostPoints = fine.getPoints();
						driverPoints = vehicle.getDriver().getLicense().getPoints();
						System.out.println("Driver " + driverName + " loses " + lostPoints + " points");
						if(vehicle.getDriver().getLicense().getSuspended()){
							System.out.println("The license of driver " + driverName + " was already suspended");
						}
						else if(driverPoints-lostPoints == 0){
							System.out.println("Driver " + driverName + " remains with 0 points");
							vehicle.getDriver().getLicense().setPoints(0);
						}
						else if(driverPoints-lostPoints < 0){
							System.out.println("License suspended for driver " + driverName);
							vehicle.getDriver().getLicense().setPoints(0);
							vehicle.getDriver().getLicense().setSuspended(true);
						}
						else {
							vehicle.getDriver().getLicense().setPoints(driverPoints-lostPoints);
						}

					} else {
						System.out.println("The vehicle does not have a designated driver");
					}
				}
			}
		}
	}

}
