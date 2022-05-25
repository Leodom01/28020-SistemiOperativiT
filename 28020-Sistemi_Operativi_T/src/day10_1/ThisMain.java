package day10_1;

import java.util.Random;

public class ThisMain {
	
	public static final int CAPIENZA = 15;

	public static void main(String args[]) {
		
		Random rand = new Random(System.currentTimeMillis());		
		CAF thisCaf = new CAF(CAPIENZA);
		Consulente[] arrayCons = new Consulente[8];
		Utente[] arrayUser = new Utente[24];
		
		thisCaf.run();
		for(Utente temp : arrayUser) {
			temp = new Utente(thisCaf, rand);
			temp.start();
		}
		for(Consulente temp : arrayCons) {
			temp = new Consulente(thisCaf, rand);
			temp.start();
		}
		
	}
	
}
