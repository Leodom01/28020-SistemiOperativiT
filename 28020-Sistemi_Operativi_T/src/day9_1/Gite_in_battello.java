package day9_1;

import java.util.Random;

public class Gite_in_battello {

	public static void main(String args[]) {
		
		Random rand = new Random(System.currentTimeMillis());
		Battello thisShip = new Battello();
		Comandante commander = new Comandante(thisShip, rand);
		Turista[] array_turisti = new Turista[25];
		
		commander.start();
		for(Turista temp : array_turisti) {
			temp = new Turista(thisShip, rand);
			temp.start();
		}
		
	}
	
}
