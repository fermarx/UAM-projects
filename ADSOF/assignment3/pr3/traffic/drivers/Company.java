package pr3.traffic.drivers;

public class Company extends Driver{

	private Person person;
	
	public Company(String company, Person person) {
		super(company);
		this.setPerson(person);
	}

	public Person getPerson() {
		return person;
	}

	public void setPerson(Person person) {
		this.person = person;
	}

}
