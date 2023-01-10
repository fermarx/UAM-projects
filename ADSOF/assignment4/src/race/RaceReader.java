package race;

import component.Component;
import vehicle.Motorcycle;
import vehicle.Vehicle;
import vehicle.Car;
import vehicle.Truck;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class RaceReader {

    public static Race read(String filename) throws IOException, RaceException {
        FileInputStream stream = new FileInputStream(filename);
        InputStreamReader reader = new InputStreamReader(stream);
        BufferedReader buffer = new BufferedReader(reader);

        String line;
        Vehicle nextVehicle;
        ArrayList<Vehicle> vehicles = new ArrayList<Vehicle>();
        Integer length = Integer.parseInt(buffer.readLine());
        Integer nextId = 1;

        while((line = buffer.readLine()) != null) {
            String[] parts = line.split(" ");
            for (int i = 0; i < Integer.parseInt(parts[0]); i++) {
                if (parts[1].equals("Motorcycle")) nextVehicle = new Motorcycle(Double.valueOf(parts[2]), nextId++);
                else if (parts[1].equals("Car")) nextVehicle = new Car(Double.valueOf(parts[2]), nextId++);
                else if (parts[1].equals("Truck")) nextVehicle = new Truck(Double.valueOf(parts[2]), nextId++);
                else throw new RaceException("Unkown vehicle type.");
                for(int j = 3; j < parts.length; j++){
                    nextVehicle.addComponent(new Component(parts[j], nextVehicle));
                }
                vehicles.add(nextVehicle);
            }
        }
        return new Race(vehicles, Double.valueOf(length));
    }
}
