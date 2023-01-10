package vehicle;

import util.Chances;

public class Truck extends Vehicle{

    public Truck(Double maxSpeed, Integer ID) {
        super(maxSpeed, ID);
    }

    @Override
    public String toString() {
        return "Truck";
    }

    @Override
    public void updatePosition() {
        if(this.criticalDamage()) return;
        if(Chances.randomBoolean(10.0)){
            this.setActualPosition(this.getActualPosition()+(0.8*this.getMaxSpeed()));
            return;
        }
        this.setActualPosition(this.getActualPosition()+this.getMaxSpeed());
    }
}
