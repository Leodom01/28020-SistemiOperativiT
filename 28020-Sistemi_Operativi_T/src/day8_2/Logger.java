package day8_2;

public class Logger extends Thread{

	public Sala s;
	
	public Logger(Sala s) {
		this.s = s;
	}
	
	public void printLog() {
		System.out.println("-----------------------------------------");
		System.out.println("Occupazione: "+s.occupazione);
		System.out.println("Bagno: "+s.bagnoOccupato);
		System.out.println("Cessolog: " + s.cessolog.toString());
		System.out.println("Entrylog: " + s.entrylog.toString());
		System.out.println();
	}
	
	public void run() {
		while(true) {
			try {
				sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			printLog();
		}
	}
	
}
