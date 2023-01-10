package vehicle;

import component.Component;
import race.Race;
import race.RaceException;
import util.Chances;

import java.util.ArrayList;

public abstract class Vehicle implements IVehicle{
    private Double maxSpeed;
    private Double position;
    private Boolean canMove;
    private Integer ID;
    private ArrayList<Component> components;
    private Integer repairedAmount;
    private Race race;

    @Override
    public double getActualPosition() {
        return this.position;
    }

    @Override
    public void setActualPosition(double newPosition) {
        this.position = newPosition;
    }

    @Override
    public boolean canMove() {
        return this.canMove;
    }

    @Override
    public void setCanMove(boolean newMovement) {
        this.canMove = newMovement;
    }

    @Override
    public double getMaxSpeed() {
        return this.maxSpeed;
    }

    @Override
    public String getName() {
        return null;
    }

    public Integer getID() {
        return ID;
    }

    public void setID(Integer ID) {
        this.ID = ID;
    }

    public Vehicle(Double maxSpeed, Integer ID) {
        this.maxSpeed = maxSpeed;
        this.ID = ID;
        this.position = 0.0;
        this.canMove = true;
        this.components = new ArrayList<Component>();
        this.repairedAmount = 0;
    }

    @Override
    public String toString() {
        return "Vehicle";
    }

    public void updatePosition(){
        this.position += this.maxSpeed;
    }

    public void addComponent(Component component) throws RaceException {
        this.components.add(component);
    }

    public Component hasComponent(String component){
        for(Component c:this.components) {
            if (c.getName().equals(component)) {
                return c;
            }
        }
        return null;
    }

    public void repair(){
        Component repairing = null;

        for(Component c:components){
            if(c.isDamaged()){
                repairing = c;
                break;
            }
        }

        if(repairing != null){
            repairedAmount++;
            if(repairing.costRepair()<=repairedAmount){
                repairing.repair();
                repairedAmount = 0;
            }
        }

        return;
    }

    public void receiveAttack(){
        Integer componentIndex;
        if(Chances.randomBoolean(50.0)){
            componentIndex = Chances.randomInteger(0, this.components.size()-1);
            this.components.get(componentIndex).setDamaged(true);
        }
    }

    public boolean criticalDamage(){
        for(Component c:components){
            if(c.isDamaged() && c.isCritical()){
                return true;
            }
        }
        return false;
    }

    public Race getRace() {
        return race;
    }

    public void setRace(Race race) {
        this.race = race;
    }

    public ArrayList<Component> getComponents() {
        return components;
    }
}
