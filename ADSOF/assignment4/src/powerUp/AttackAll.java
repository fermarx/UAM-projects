package powerUp;

import vehicle.Vehicle;

import java.util.ArrayList;

public class AttackAll implements IPowerUp{

    public AttackAll() {
    }

    @Override
    public void applyPowerUp(Vehicle vehicle) {
        ArrayList<Vehicle> vehicles = vehicle.getRace().getVehicles();
        for(Vehicle v:vehicles){
            if(!v.equals(vehicle)) v.receiveAttack();
        }
    }

    @Override
    public String namePowerUp() {
        return "AttackAll";
    }
}
