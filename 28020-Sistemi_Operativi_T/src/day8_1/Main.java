package day8_1;

import java.util.Random;

public class Main {

	private final static int CAPIENZA = 4;
	private final static int PERMANENZA_SEC = 8;
	private final static int MAX_CITTADINI = 50;
	
	
	public static final void main(String args[]) {
		
		Random rand = new Random(System.currentTimeMillis());
		Visitatore[] visitatori = new Visitatore[10 + rand.nextInt(MAX_CITTADINI-10)];
		Sala s = new Sala(CAPIENZA);
		//Logger thisLogger = new Logger(s);
		
		System.out.println("Visitatori: "+visitatori.length+"\nCapienza: "+CAPIENZA);
		
		//thisLogger.start();
		
		for(int i = 0; i < visitatori.length; i++) {
			visitatori[i] = new Visitatore(s, PERMANENZA_SEC, rand);
		}
		
		for(Visitatore temp : visitatori) {
			temp.start();
		}
		
		for(Visitatore temp : visitatori) {
			try {
				temp.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		System.out.println("Ecco i valori finali della sala:\n");
		s.stampa();
		
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return;
	};
}
