package vehicle;

import component.Component;
import race.RaceException;

public class Motorcycle extends Vehicle{

    public Motorcycle(Double maxSpeed, Integer ID) {
        super(maxSpeed, ID);
    }

    @Override
    public String toString() {
        return "Motorcycle";
    }

    @Override
    public void updatePosition() {
        if(this.criticalDamage()) return;
        this.setActualPosition(this.getActualPosition()+this.getMaxSpeed());
    }

    @Override
    public void addComponent(Component component) throws RaceException {
        if(component.getName().equals("BananaDispenser")||component.getName().equals("Window")){
            throw new RaceException("Component " + component.getName() + " is not valid for vehicle " + this.getName() + "(" + this.getID() + ")");
        }
        super.addComponent(component);
    }
}
