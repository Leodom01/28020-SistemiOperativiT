package day9_2;

import java.util.Random;

public class Comandante extends Thread{

	public Battello ship;
	Random rand;
	
	Comandante(Battello ship, Random rand) {
		this.ship = ship;
		this.rand = rand;
	}


	public void run() {
		while(true) {
			System.out.println("Capitano sveglio");
			try {
				sleep(1000* rand.nextInt(2));
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println("CAP: Voglio iniziare la gita...");
			ship.Inizio_gita();
			System.err.println("CAP: Gita iniziata!");
			System.err.println("CAP: Occupazione barca: "+this.ship.tur_on_board+"   Capienza: "+this.ship.C_MAX);
			System.err.println("CAP: Guide in  barca: "+this.ship.gui_on_board);
			
			try {
				sleep(1000*(2+ rand.nextInt(5)));
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println("CAP: Finisco la gita...");
			ship.Fine_gita();
			System.err.println("CAP: Gita finita!");
			System.out.println("CAP: Ora dormo...");
			
			try {
				sleep(1000*(rand.nextInt(3)));
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
}
