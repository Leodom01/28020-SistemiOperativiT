package day10_1;

import java.util.Random;

public class Utente extends Thread{

	private CAF myCaf;
	private Random rand;
	
	public Utente(CAF myCaf, Random rand) {
		this.myCaf = myCaf;
		this.rand = rand;
	}
	
	public void run() {
		try {
			System.out.println(getId() + " USER: sveglio");
			sleep(1000 * (rand.nextInt(5)));

			// Inizio processo di entrata
			System.out.println(getId() + " USER: voglio entrare");
			myCaf.entra_U(this);
			System.err.println(getId() + " USER: entrato --- Occupazione: " + myCaf.occupazione + " Capienza: " + myCaf.CAPIENZA);
			sleep(1000 * (3 + rand.nextInt(5)));

			// Inizio processo di uscita
			System.out.println(getId() + " USER: voglio uscire");
			myCaf.esci_U(this);
			System.err.println(getId() + " USER: uscito");

		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
