package day8_2;

import java.util.Random;

public class Visitatore extends Thread{

	final static int MAX_BATH_SEC = 12;
	final static int PROB_ENTRA_BAGNO = 75;		//75%
	
	Sala s;
	int permanenzaSec;
	Random rand;
	
	//Tempo di attesa varia da 0 a 30 secondi
	
	public Visitatore(Sala s, int permanenzaSec, Random rand) {
		this.s = s;
		this.permanenzaSec = permanenzaSec;
		this.rand = rand;
	}
	
	public void occupaBagno() {
		synchronized(s){
			System.out.println("Sono entrato in bagno: "+getId());
			if(s.bagnoOccupato == true) {
				System.out.println("Malissimo, dovrebbe sempre essere libero!");
			}
			if(s.bagnoOccupato == false) {
				s.bagnoOccupato = true;
				s.cessolog.add((int) getId());
				try {
					System.out.println("Dormo in bagno: "+getId());
					sleep((3 + rand.nextInt(MAX_BATH_SEC - 1))*1000);
					System.out.println("Svegliato in bagno: "+getId());
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				s.bagnoOccupato = false;
				System.out.println("Esco dal bagno: "+getId());
			}
		}
	}
	
	public void run() {
		//System.out.println("Running: "+getId());
		while(s.entra() == 0) {
			try {
				int sleeptime = rand.nextInt(30000);
				//System.out.println(getId()+" going to sleep for "+sleeptime);
				sleep(sleeptime);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		this.s.entrylog.add((int) getId());
		//Bagno
		if(rand.nextInt(101) <= 75) {
			System.out.println("Provo a occupare: "+getId());
			occupaBagno();
		}
		
		try {
			sleep(this.permanenzaSec*1000);
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		s.esce();
		//System.out.println("Exiting: "+getId());
	}
	
}
