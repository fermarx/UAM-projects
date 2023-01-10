package powerUp;

import vehicle.Vehicle;

public class Dash implements IPowerUp{

    public Dash() {
    }

    @Override
    public void applyPowerUp(Vehicle vehicle) {
        vehicle.setActualPosition(vehicle.getActualPosition()+20.0);
    }

    @Override
    public String namePowerUp() {
        return "Dash";
    }
}
