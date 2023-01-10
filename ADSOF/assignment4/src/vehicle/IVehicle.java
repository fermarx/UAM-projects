package vehicle;

public interface IVehicle {
    public double getActualPosition();
    public void setActualPosition(double newPosition);
    public boolean canMove();
    public void setCanMove(boolean newMovement);
    public double getMaxSpeed();
    public String getName();

//For Exercise 3
//public void addComponent(IComponent c) throws InvalidComponentException;
//public List<IComponent> getComponents();
}
