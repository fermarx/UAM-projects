package pr3.traffic.drivers.license;

import java.util.ArrayList;
import java.util.List;

public class License {

    private List<PermitKind> kinds = new ArrayList<>();
	private static Integer nextID = 1;
	private Integer ID;
	private Integer points;
	private Boolean suspended;

	public License(PermitKind ... kinds) {
		for (PermitKind kind: kinds){
			this.kinds.add(kind);
		}
		this.ID = nextID;
		nextID++;
		this.points = 12;
		this.suspended = false;
	}

	public PermitKind getHighestKind(){
		Integer i = 0;
		PermitKind ret = null;
		for (PermitKind p: kinds){
			if (i < p.getAge()) {
				i = p.getAge();
				ret = p;				
			}
		}
		return ret;
	}

	public void addPermit(PermitKind kind) {
		this.kinds.add(kind);	
	}
	
	public List<PermitKind> getKind(){
		return this.kinds;
	}

	public void setKind(PermitKind kind){
		this.kinds.add(kind);
	}

	/*License [id=1, permits=[A, C1], points=12]*/
	@Override
	public String toString(){
		String permits = "";

		for(PermitKind kind: this.kinds){
			permits += kind.toString() + ",";
		}

		return ("License [id=" + this.ID + ", permits=[" + permits + "], points=" +  this.points + "]");
	}

	public Integer getPoints() {
		return points;
	}

	public void setPoints(Integer points) {
		this.points = points;
	}

	public Boolean getSuspended() {
		return suspended;
	}

	public void setSuspended(Boolean suspended) {
		this.suspended = suspended;
	}
}
