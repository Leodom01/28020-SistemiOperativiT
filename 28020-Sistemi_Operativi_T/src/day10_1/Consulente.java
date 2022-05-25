package day10_1;

import java.util.Random;

public class Consulente extends Thread{

	private CAF myCaf;
	private Random rand;
	
	public Consulente(CAF myCaf, Random rand) {
		this.myCaf = myCaf;
		this.rand = rand;
	}
	
	public void run() {
		try {	
			while(true) {
				System.out.println(getId()+" CONS: sveglio");
				sleep(1000*(rand.nextInt(5)));
				
				//Inizio processo di entrata
				System.out.println(getId()+" CONS: voglio entrare");
				myCaf.entra_C(this);
				System.err.println(getId()+" CONS: entrato --- Occupazione: "+myCaf.occupazione+" Capienza: "+myCaf.CAPIENZA);
				sleep(1000*(3+rand.nextInt(5)));
				
				//Inizio processo di uscita
				System.out.println(getId()+" CONS: voglio uscire");
				myCaf.esci_C(this);
				System.err.println(getId()+" CONS: uscito");
			}
		}catch(InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	
}
