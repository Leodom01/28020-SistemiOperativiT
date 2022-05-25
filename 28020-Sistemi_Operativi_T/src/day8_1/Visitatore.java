package day8_1;

import java.util.Random;

public class Visitatore extends Thread{

	Sala s;
	int permanenzaSec;
	Random rand;
	//Tempo di attesa varia da 0 a 30 secondi
	
	public Visitatore(Sala s, int permanenzaSec, Random rand) {
		this.s = s;
		this.permanenzaSec = permanenzaSec;
		this.rand = rand;
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
		try {
			sleep(this.permanenzaSec*1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		s.esce();
		//System.out.println("Exiting: "+getId());
	}
	
}
