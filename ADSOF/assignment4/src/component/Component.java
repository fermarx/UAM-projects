package component;

import race.RaceException;
import vehicle.IVehicle;
import vehicle.Vehicle;

public class Component implements IComponent {
    private boolean damaged;
    private String name;
    private Integer repairCost;
    private Vehicle vehicle;
    private boolean critical;

    @Override
    public boolean isDamaged() {
        return this.damaged;
    }

    @Override
    public void setDamaged(boolean damage) {
        this.damaged = damage;
    }

    @Override
    public String getName() {
        return this.name;
    }

    @Override
    public int costRepair() {
        return this.repairCost;
    }

    @Override
    public IVehicle getVehicle() {
        return this.vehicle;
    }

    @Override
    public boolean isCritical() {
        return this.critical;
    }

    @Override
    public void repair() {
        this.setDamaged(false);
    }

    public Component(String name, Vehicle vehicle) throws RaceException{
        if(name.equals("Engine")) this.componentBuilder(name, 3, vehicle, true);
        else if(name.equals("Wheels")) this.componentBuilder(name, 3, vehicle, true);
        else if(name.equals("Window")) this.componentBuilder(name, 2, vehicle, false);
        else if(name.equals("BananaDispenser")) this.componentBuilder(name, 4, vehicle, false);
        else throw new RaceException("Unknown component");
    }

    public void componentBuilder(String name, Integer repairCost, Vehicle vehicle, boolean critical) {
        this.damaged = false;
        this.name = name;
        this.repairCost = repairCost;
        this.vehicle = vehicle;
        this.critical = critical;
    }

    @Override
    public String toString() {
        return "->" + this.name + ". Is damaged: " + this.damaged + ". Is critical: " + this.critical;
    }
}
