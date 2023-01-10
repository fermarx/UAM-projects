package pr3.test;

import pr3.fines.FineProcessor;
import pr3.fines.FineReader;
import pr3.traffic.Car;
import pr3.traffic.Motorcycle;
import pr3.traffic.Truck;
import pr3.traffic.Vehicle;
import pr3.traffic.drivers.Company;
import pr3.traffic.drivers.Person;
import pr3.traffic.drivers.license.License;
import pr3.traffic.drivers.license.PermitKind;

import java.util.Arrays;

// add imports and package
public class TesterFinesExtended {
	public static void main(String[] args) {
		Person ann = new Person("Ann Smith", 30);		 		// Ann 			
		Person anthony = new Person("Anthony Johnson", 27);
		Company fdinc = new Company("Fast Delivery Inc", ann);	// Ann es responsible for FDINC
		
		ann.setLicense(new License(PermitKind.B, PermitKind.C1));
		anthony.setLicense(new License(PermitKind.A));
		
		Vehicle fleet[] = { 
				new Car("Fiat 500x", 2019, "1245 HYN", true, ann), 	// Ann's car, who drives it		
				new Truck("IvecoDaily", 2010, "5643 KOI", 2, fdinc), // FDINC's car
				new Motorcycle("Harley Davidson", 2003, "0987 ETG", false, anthony),
				new Motorcycle("Yamaha YZF-R1M", 2018, "1460 III", false, ann)
		};
		
		FineProcessor pm = new FineProcessor(Arrays.asList(fleet));
		pm.process(FineReader.read("fines_radar2.txt"));
	}
}
