package day10_1;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class CAF extends Thread{

	public int CAPIENZA = 15;
	
	private Consulente consulenti[];			//null se è vuoto oppure il consulente dentro al CAF
	private Utente utenti[];					//null se è vuoto oppure l'utente dentro al CAF
	
	private Condition usr_in;
	private int usr_in_waiting;
	private Condition cons_in;
	private int cons_in_waiting;
	private Condition cons_out;
	private int cons_out_waiting;
	private Lock mainLock;
	
	public int occupazione;
	
	//Constructor
	public CAF(int capienzaMax) {
		this.consulenti = new Consulente[CAPIENZA];
		for(Consulente temp : consulenti) {
			temp = null;
		}
		this.utenti = new Utente[CAPIENZA];
		for(Utente temp : utenti) {
			temp = null;
		}
		this.mainLock = new ReentrantLock();
		this.usr_in = mainLock.newCondition();
		this.usr_in_waiting = 0;
		this.cons_in = mainLock.newCondition();
		this.cons_in_waiting = 0;
		this.cons_out = mainLock.newCondition();
		this.cons_out_waiting = 0;
		this.CAPIENZA = capienzaMax;
		this.occupazione = 0;
	}
	
	//Sospensivo se non c'è posto o se non c'è consulente libero
	public void entra_U(Utente user) {
		mainLock.lock();
		try {
			int free_spot = availableUserSpot();
			while(occupazione == CAPIENZA || (free_spot = availableUserSpot()) == -1){
				usr_in_waiting++;
				usr_in.await();
				usr_in_waiting--;
			}
			
			occupazione++;
			utenti[free_spot] = user;
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally {
			mainLock.unlock();
		}
	}

	public void esci_U(Utente user) {
		mainLock.lock();
		try {
			occupazione--;
			utenti[findUserSpot(user)] = null;
			
			if(cons_out_waiting != 0) {
				cons_out.signalAll();
			}
			if(cons_in_waiting != 0) {
				cons_in.signal();
			}
			if(usr_in_waiting != 0) {
				usr_in.signal();
			}
		}finally {
			mainLock.unlock();
		}
	}
	
	//Sospensivo se non c'è posto
	public void entra_C(Consulente cons) {
		mainLock.lock();
		try {
			int free_spot = availableConsultantSpot();
			while(occupazione == CAPIENZA || (free_spot = availableConsultantSpot()) == -1) {
				cons_in_waiting++;
				cons_in.await();
				cons_in_waiting--;
			}
			occupazione++;
			consulenti[free_spot] = cons;
			
			if(usr_in_waiting != 0) {
				usr_in.signal();
			}
			if(cons_in_waiting != 0) {
				cons_in.signal();
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally {
			mainLock.unlock();
		}
	}
	
	//Sospensivo se sta servendo utente
	public void esci_C(Consulente cons) {
		mainLock.lock();
		try {
			int my_spot = findConsultantSpot(cons);
			while(utenti[my_spot] != null) {
				cons_out_waiting++;
				cons_out.await();
				cons_out_waiting--;
			}
			
			occupazione--;
			consulenti[my_spot] = null;
			
			if(cons_in_waiting != 0) {
				cons_in.signal();
			}
			if(usr_in_waiting != 0) {
				usr_in.signal();
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally {
			mainLock.unlock();
		}
	}
	
	//Indica in quale sportello c'è in consulente disponibile, -1 se non c'è 
	private int availableUserSpot() {
		for(int i = 0; i < CAPIENZA; i++) {
			if(consulenti[i] != null && utenti[i] == null) {
				return i;
			}
		}
		return -1;
	}
	
	//Indica in quale sportello c'è spazio, -1 se non c'è (in teoria c'è sempre
	private int availableConsultantSpot() {
		for(int i = 0; i < CAPIENZA; i++) {
			if(consulenti[i] == null) {
				return i;
			}
		}
		return -1;
	}
	
	//Indica in quale cella dell'array (sportello) si trova il dato utente
	private int findUserSpot(Utente user) {
		for(int i = 0; i < CAPIENZA; i++) {
			if(utenti[i] != null && utenti[i].getId() == user.getId()) {
				return i;
			}
		}
		return -1;
	}
	
	//Indica in quale cella dell'array (sportello) si trova il dato consulente
		private int findConsultantSpot(Consulente cons) {
			for(int i = 0; i < CAPIENZA; i++) {
				if(consulenti[i] != null && consulenti[i].getId() == cons.getId()) {
					return i;
				}
			}
			return -1;
		}
	
	
}
