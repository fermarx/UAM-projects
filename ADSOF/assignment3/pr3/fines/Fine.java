package pr3.fines;

public class Fine {
    private String plate;
    private String reason;
    private Integer points;

    public Fine(String str){
        this.plate = str.split(";")[0];
        this.reason = str.split(";")[1];
        this.points = Integer.parseInt(str.split(";")[2]);
    }

    @Override
    public String toString(){
        return "Fine [plate=" + this.plate + ", Fine type=" + this.reason + ", points=" + this.points + "]";
    }

    public String getPlate() {
        return plate;
    }

    public String getReason() {
        return reason;
    }

    public Integer getPoints() {
        return points;
    }
}