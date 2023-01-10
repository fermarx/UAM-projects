package pr3.traffic.drivers;

import pr3.traffic.drivers.license.*;

public class Person extends Driver{

	private Integer age;
	private License license;
	
	public Person(String name, int age) {
		super(name);
		this.age = age;
	}

	public Integer getAge() {
		return age;
	}

	public void setAge(Integer age) {
		this.age = age;
	}

	public Boolean setLicense(License license){
		if(license.getHighestKind() == PermitKind.A || license.getHighestKind() == PermitKind.B){
			if(this.age >= PermitKind.A.getAge()){
				this.license = license;
				return true;
			} else {
				return false;
			}
		} else {
			if(this.age >= PermitKind.C1.getAge()){
				this.license = license;
				return true;
			} else {
				return false;
			}
		}
	}

	public License getLicense(){
		return this.license;
	}
}