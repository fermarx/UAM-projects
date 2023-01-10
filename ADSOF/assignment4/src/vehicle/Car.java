package vehicle;

import util.Chances;

public class Car extends Vehicle{

    public Car(Double maxSpeed, Integer ID) {
        super(maxSpeed, ID);
    }

    @Override
    public String toString() {
        return "Car";
    }

    @Override
    public void updatePosition() {
        if(this.criticalDamage()) return;
        if(Chances.randomBoolean(10.0)){
            this.setActualPosition(this.getActualPosition()+(0.9*this.getMaxSpeed()));
            return;
        }
        this.setActualPosition(this.getActualPosition()+this.getMaxSpeed());
    }
}
