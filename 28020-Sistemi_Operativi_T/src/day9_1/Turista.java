package day9_1;

import java.util.Random;

public class Turista extends Thread{

	
	public Battello ship;
	Random rand;
	
	Turista(Battello ship, Random rand) {
		this.ship = ship;
		this.rand = rand;
	}


	public void run() {
		System.out.println(getId()+" : sveglio, dormo un po'");
		
		try {
			sleep(1000*(rand.nextInt(10)));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		System.out.println(getId()+" : voglio salire in barca...");
		ship.Imbarco_T();
		synchronized(this.ship) {
			System.out.println(getId()+" : Salito in barca!");
			System.err.println(getId()+" : Occupazione barca: "+this.ship.tur_on_board+"   Capienza: "+this.ship.C_MAX);
		}
		try {
			sleep(1000*(2 + rand.nextInt(8)));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		System.out.println(getId()+" : voglio scendere...");
		ship.Sbarco_T();
		synchronized(this.ship) {
			System.out.println(getId()+" : Sbarcato!");	
			System.err.println(getId()+" : Occupazione barca: "+this.ship.tur_on_board+"   Capienza: "+this.ship.C_MAX);
		}
	}
	
}
