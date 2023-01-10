package component;

import vehicle.IVehicle;

public interface IComponent {
    public boolean isDamaged();
    public void setDamaged(boolean damage);
    public String getName();
    public int costRepair();
    public IVehicle getVehicle();
    public boolean isCritical();
    public void repair();
}
