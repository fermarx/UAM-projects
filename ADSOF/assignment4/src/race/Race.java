package race;

import component.Component;
import powerUp.AttackAll;
import powerUp.Dash;
import powerUp.IPowerUp;
import powerUp.Swap;
import util.Chances;
import vehicle.Vehicle;

import java.util.ArrayList;

public class Race {
    private ArrayList<Vehicle> vehicles;
    private Double length;

    public Race(ArrayList<Vehicle> vehicles, Double lenght) {
        this.vehicles = vehicles;
        for(Vehicle v:vehicles){
            v.setRace(this);
        }
        this.length = lenght;
    }

    @Override
    public String toString() {
        String ret = "";

        ret += "Race with maximum length: " + this.length + "\n";
        for(Vehicle v:this.vehicles){
            ret += v + "(" + v.getID() + "). Speed " + v.getMaxSpeed() + " Actual position: " + v.getActualPosition() + ".\n";
            for(Component c:v.getComponents()){
                ret += c.toString() + "\n";
            }
            for(Vehicle w:this.vehicles){
                if(w!=v){
                    ret += v + "(" + v.getID() + ") distance to " + w + "(" + w.getID() + ") " + (w.getActualPosition() - v.getActualPosition()) + "\n";
                }
            }
        }

        return ret;
    }

    public void simulate(){
        Boolean raceFinished = false;
        Integer turn = 1;
        Vehicle winner = null;

        while (!raceFinished){
            System.out.println("-------------\n");
            System.out.println("Starting turn: " + turn + "\n");
            System.out.println(this);
            for(Vehicle v: this.vehicles){
                if(turn%3 == 0) vehicleAttack(v);
                if(turn>3 && turn%3 != 0) vehiclePowerUp(v);
                v.repair();
                v.updatePosition();
                if(v.getActualPosition() >= this.length) {
                    winner = v;
                    raceFinished = true;
                }
            }
            System.out.println("Ending turn: " + turn++ + "\n");
        }
        System.out.println(winner + "(" + winner.getID() + ") Speed " + winner.getMaxSpeed() + ". Actual Position: " + winner.getActualPosition() + "\nhas won the race.");
    }

    public Vehicle vehicleAhead(Vehicle vehicle){
        Vehicle objective = null;

        /* We obtain the vehicle in front of us. */
        for(Vehicle v: this.vehicles){
            if(v.getActualPosition() > vehicle.getActualPosition()){
                if(objective != null){
                    if(v.getActualPosition() < objective.getActualPosition()){
                        objective = v;
                    }
                }
                else{
                    objective = v;
                }
            }
        }

        return objective;
    }

    public void vehicleAttack(Vehicle vehicle){
        Vehicle objective = vehicleAhead(vehicle);
        Component bananaDispenser = vehicle.hasComponent("BananaDispenser");

        /* We check that we can use the dispenser. */
        if(bananaDispenser==null){
            return;
        }
        if(bananaDispenser.isDamaged()){
            return;
        }

        /* We send the attack */
        if(objective != null){
            if(objective.getActualPosition()-30.0<vehicle.getActualPosition()){
                System.out.println(vehicle  + "(" + vehicle.getID() + ") attacks " + objective + "(" + objective.getID() + ")");
                objective.receiveAttack();
            }
        }
    }

    public void vehiclePowerUp(Vehicle vehicle){
        IPowerUp powerUp;

        if(Chances.randomBoolean(10.0)){
            Integer random = Chances.randomInteger(0,2);
            if(random == 0){
                powerUp = new AttackAll();
            }
            if(random == 1){
                powerUp = new Swap();
            }
            else{
                powerUp = new Dash();
            }

            System.out.println("Vehicle " + vehicle + "(" + vehicle.getID() + ") applying power-up: " + powerUp.namePowerUp() + ".\n");
            powerUp.applyPowerUp(vehicle);
        }
    }

    public ArrayList<Vehicle> getVehicles() {
        return vehicles;
    }
}
