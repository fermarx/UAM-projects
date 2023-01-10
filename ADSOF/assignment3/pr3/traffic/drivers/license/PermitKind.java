package pr3.traffic.drivers.license;

public enum PermitKind {

    A(18), B(18), C1(23);

    private Integer age;

    private PermitKind(Integer age){
        this.age = age;
    }

    public Integer getAge(){
        return this.age;
    }

}
