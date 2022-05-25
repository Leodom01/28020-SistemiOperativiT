package day8_1;

public class Logger extends Thread{

	public Sala s;
	
	public Logger(Sala s) {
		this.s = s;
	}
	
	public void printLog() {
		System.out.println("Visite:");
		System.out.println(s.entrylog.toString());
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
