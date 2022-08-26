/**
*	Dominici Leonardo Matricola: 0000971128
*
* Il seguente file rappresenta uno scheletro per eseguire l'esame di Sistemi Operativi-T della laurea in Ingegneria Informatica UNIBO
* Da fare durante l'esame in base al task:
* - Implementazione eventuali handler
* - Controllo dei parametri in input argv
* - Codice child P1 e P2
* - Codice master P0
* - Cambia la string stampata da usage in base alla sintassi del programma
*
*	@author:	leodom01
*	@GitHub:	https://github.com/Leodom01
*	@version:	0.1
*
*	Visit my GitHub page to download the last version
*	For problems or infos please contact me at: leonardodominici01@gmail.com
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

void usage(char* progName);

//void signal_handler_default(int signum);

int pid0, pid1, pid2;   //Pids of the 3 process I'll create
int pp[2];              //Pipe FDs


int main(int argc, char* argv[]){

  pid0 = getpid();

  //TODO Params check
  if(argc != 4){
    usage("output");
    printf("Numero di parametri sbagliati\n");
    exit(EXIT_FAILURE);
  }//else if(atoi(argv[2]) < 0){
  //  usage("output");
  //  printf("Intero devono essere maggiori di 0\n");
  //  exit(EXIT_FAILURE);
  //}


  //Creating new pipe
  if (pipe(pp)<0){
    perror("Apertura pipe fallita...\n");
    exit(EXIT_FAILURE);
  }

  pid1=fork();
	if (pid1==0) {           //Dentro a P1!
    //TODO P1 Job!

	}else if(pid1>0){        //Di nuovo in P0
    //Genero P2
		pid2=fork();
		if (pid2==0) {        //Dentro a P2!
			//TODO P2 Job!

		}else if(pid2>0){     //Di nuovo in P0


		}else{
			perror("P0: Impossibile creare processo figlio P2...\n");
			exit(EXIT_FAILURE);
		}

	}else{
    perror("P0: Impossibile creare processo figlio P1...\n");
    exit(EXIT_FAILURE);
	}

}

void usage(char* progName){
  //TODO Change the string accordingly to the task request
  perror("Devi chiamare così il programma: \nouput Fin W T\n");
}

//void signal_handler_default(int signum){
//TODO Add some kind of handler if needed
//}
