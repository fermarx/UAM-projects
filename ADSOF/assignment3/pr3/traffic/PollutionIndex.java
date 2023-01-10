package pr3.traffic;

/* 
 * Enumeration that classifies the vehicles taking into account their fabrication year
 * 
 * @author Jose Manuel Freire and Sofia Xiaofan Fernandez Marin jose.freire@estudiante.uam.es sofiax.fernandez@estudiante.uam.es
 * 
 */
public enum PollutionIndex {
	A, B, C;
	
	private static final int DATEA = 2018;
	private static final int DATEB = 2010;
	
	public static PollutionIndex getPollutionIndex(int year) {
		if (year >= DATEA) return A;
		if (year >= DATEB) return B;
		return C;
	}
}
