package day9_2;

import java.util.Random;

public class Guida extends Thread{

	public Battello ship;
	Random rand;
	
	Guida(Battello ship, Random rand) {
		this.ship = ship;
		this.rand = rand;
	}


	public void run() {
		while(true) {
			System.out.println(getId()+" - GUIDA : sveglio, dormo un po'");
			
			try {
				sleep(1000*(rand.nextInt(10)));
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			System.out.println(getId()+" - GUIDA : voglio salire in barca...");
			ship.Imbarco_G();
			synchronized(this.ship) {
				System.out.println(getId()+" - GUIDA : Salito in barca!");
				System.err.println(getId()+" : Guide in  barca: "+this.ship.gui_on_board);
			}
			try {
				sleep(1000*(2 + rand.nextInt(8)));
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			System.out.println(getId()+" - GUIDA : voglio scendere...");
			ship.Sbarco_G();
			synchronized(this.ship) {
				System.out.println(getId()+" - GUIDA : Sbarcato!");	
				System.err.println(getId()+" : Guide in  barca: "+this.ship.gui_on_board);
			
			}
		}
	}
	
}
