package day8_1;

import java.util.ArrayList;
import java.util.Random;

public class Sala {

	int occupazione;
	int capienza; 
	int donatori;
	int abbigliamento;
	int giocattoli;
	int soldi;
	ArrayList<Integer> entrylog;
	
	public Sala(int capienza) {
		this.occupazione = 0;
		this.capienza = capienza;
		this.donatori = 0;
		this.abbigliamento = 0;
		this.giocattoli = 0;
		this.soldi = 0;
		this.entrylog = new ArrayList<Integer>();
	}

	public String toString() {
		return 	"Capienza: "+capienza+
				"\nOccupazione: "+occupazione+
				"\nDonatori: "+donatori+
				"\nAbbigliamento: "+abbigliamento+
				"\nGiocattoli: "+giocattoli+
				"\nSoldi: "+soldi;
	}
	
	public synchronized void stampa() {
		System.out.println("Stato della sala:\n"+this.toString());
	}
	
	public synchronized int entra() {
		if(this.occupazione < this.capienza) {
			Random rand = new Random(System.currentTimeMillis());
			this.occupazione++;
			this.donatori++;
			this.abbigliamento += rand.nextInt(2);
			this.giocattoli += rand.nextInt(2);
			this.soldi += rand.nextInt(2);
			return 1;
		}else {
			return 0;
		}
	}
	
	public synchronized void esce() {
		this.occupazione--;
	}
	
	
}
