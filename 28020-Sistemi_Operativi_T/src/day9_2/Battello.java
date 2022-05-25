package day9_2;

import java.util.concurrent.Semaphore;

public class Battello {

	public final int C_MAX = 6;	//Capienza barca
	public final int G_NEEDED = 2;
	
	private Boolean on_tour; 			//false se fermo, true se in gita
	private Semaphore sosp_tur_i;		//Sospensione di turisti per imbarco
	private int sosp_tur_i_waiting;		//Numero di tusiti in attesa che si releasino sosp_tur_i
	private Semaphore sosp_tur_s;		//Sospensione di turisti per sbarco
	private int sosp_tur_s_waiting;		//Numero di tusiti in attesa che si releasino sosp_tur_s
	private Semaphore sosp_cap;			//Sospensione per capitano
	private int sosp_cap_waiting;		//Se il capito sta aspettando che venga fatta release di sosp_cap
	private Semaphore obj_mutex;		//Sospensione su tutto l'oggetto
	public int tur_on_board;
	private Semaphore sosp_gui_i;
	private int sosp_gui_i_waiting;
	private Semaphore sosp_gui_s;
	private int sosp_gui_s_waiting;
	public int gui_on_board;
	
	public Battello() {
		this.on_tour = false;
		this.sosp_tur_i = new Semaphore(0);
		this.sosp_tur_i_waiting = 0;
		this.sosp_tur_s = new Semaphore(0);
		this.sosp_tur_s_waiting = 0;
		this.sosp_cap = new Semaphore(0);
		this.sosp_cap_waiting = 0;
		this.obj_mutex = new Semaphore(1);
		this.tur_on_board = 0;
		this.sosp_gui_i = new Semaphore(0);
		this.sosp_gui_i_waiting = 0;
		this.sosp_gui_s = new Semaphore(0);
		this.sosp_gui_s_waiting = 0;
		this.gui_on_board = 0;
	}
	
	//Imbarco turista, sospensivo se battello pieno o se gita in corso
	public void Imbarco_T() {	
		//Resources locking
		try {
			obj_mutex.acquire();
			while(on_tour || tur_on_board >= C_MAX) {
				sosp_tur_i_waiting++;
				obj_mutex.release();
				sosp_tur_i.acquire();
				obj_mutex.acquire();
				sosp_tur_i_waiting--;
			}
			
			//Core task
			tur_on_board++;
			
			//Se sta aspettando sveglio il capitano 
			if(sosp_cap_waiting == 1) {
					sosp_cap.release();
			}
			
			obj_mutex.release();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	//Sbarco di un turista, sospensivo se la gita è in corso
	public void Sbarco_T() {
		//Resources locking
		try {
			obj_mutex.acquire();
			while(on_tour) {
				sosp_tur_s_waiting++;
				obj_mutex.release();
				sosp_tur_s.acquire();
				obj_mutex.acquire();
				sosp_tur_s_waiting--;
			}
			
			//Core task
			tur_on_board--;
			
			for(int i = 0; i < sosp_tur_i_waiting; i++) {
				sosp_tur_i.release();
			}
			obj_mutex.release();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
	
	public void Imbarco_G() {
		try {
			obj_mutex.acquire();
			while(on_tour || gui_on_board >= G_NEEDED) {
				sosp_gui_i_waiting++;
				obj_mutex.release();
				sosp_gui_i.acquire();
				obj_mutex.acquire();
				sosp_gui_i_waiting--;
			}
			
			gui_on_board++;
			
			if(sosp_cap_waiting == 1) {
				sosp_cap.release();
			}
			
			obj_mutex.release();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}
	
	public void Sbarco_G() {
		try {
			obj_mutex.acquire();
			while(on_tour) {
				sosp_gui_s_waiting++;
				obj_mutex.release();
				sosp_gui_s.acquire();
				obj_mutex.acquire();
				sosp_gui_s_waiting--;
			}
			
			gui_on_board--;
			
			for(int i = 0; i < sosp_gui_i_waiting; i++) {
				sosp_gui_i.release();
			}
			
			obj_mutex.release();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	//Permette al comandante di iniziare la gita, sospensivo se il battello non è pieno
	public void Inizio_gita() {
		//Resources locking
		try {
			obj_mutex.acquire();
			while(tur_on_board != C_MAX || gui_on_board != G_NEEDED) {
				sosp_cap_waiting++;
				obj_mutex.release();
				sosp_cap.acquire();
				obj_mutex.acquire();
				sosp_cap_waiting--;
			}
			
			//Core task
			on_tour = true;
			
			//Resources unlocking
			sosp_cap.release();
			obj_mutex.release();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}
	
	public void Fine_gita() {
		//Resources locking
		try {
			sosp_cap.acquire();
			obj_mutex.acquire();
			
			//Core task
			on_tour = false;
			
			for(int i = 0; i < sosp_tur_s_waiting; i++) {
				sosp_tur_s.release();
			}
			
			for(int i = 0; i < sosp_gui_s_waiting; i++) {
				sosp_gui_s.release();
			}
			
			//Resources unlocking
			obj_mutex.release();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}	
	}
	
	
	
	
}
