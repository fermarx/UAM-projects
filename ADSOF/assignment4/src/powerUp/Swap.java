package powerUp;

import vehicle.Vehicle;

import java.util.ArrayList;

public class Swap implements IPowerUp{

    public Swap() {
    }

    @Override
    public void applyPowerUp(Vehicle vehicle) {
        Vehicle swapped = vehicle.getRace().vehicleAhead(vehicle);
        if(swapped == null) return;
        Double swappedActualPosition = swapped.getActualPosition();
        swapped.setActualPosition(vehicle.getActualPosition());
        vehicle.setActualPosition(swappedActualPosition);
    }

    @Override
    public String namePowerUp() {
        return "Swap";
    }
}
