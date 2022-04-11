#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>

int N;
int pid0 = -2;
int pid1 = -2;
int pid2 = -2;
int pid1_1 = -2;
int p2_job = -1;  //If 1 -> sigusr1. If 2 -> sigusr2 else stop
int i = 0;
float S = 0.0;
int terminati = 0;

void p2_handler(int signo){
  if(signo == SIGUSR1){
    p2_job = 1;
  }else if(signo == SIGUSR2){
    p2_job = 2;
  }else if(signo == SIGALRM){
    printf("P2: Valore di S: %f\nValore di i: %d\n", S, i);
    kill(pid1, SIGCONT);
    printf("P2: Ho appena mandato il segnale a P1\n");
    exit(0);
  }else if(signo == SIGCONT){
    printf("P2: Ricevuto segnale che non dovevo ricevere\n");
  }
}

void p0_handler(int signo){
  int status;
  if(signo == SIGCHLD){
    if(terminati <= 0){
      terminati++;
    }else{
      printf("Tutti i figli terminati, chiudo tutto...\n");
      exit(EXIT_SUCCESS);
    }
  }
}

void p1_handler(int signo){
  if(signo == SIGCONT){
    printf("P1: sono stato appena chiamato da P2\n");
    printf("P1: Il mio PID e': %d\n", getpid());
    kill(pid1_1, SIGKILL);
    exit(EXIT_SUCCESS);
  }
}

void usage(char program_name[]){
  fprintf(stderr, "Invoca il comando con: %s N\n", program_name);
}

int main(int argc, char* argv[]){

  if(argc != 2){
    fprintf(stderr, "Numero di argomenti sbagliati\n");
    usage("output");
    exit(EXIT_FAILURE);
  }
  N = atoi(argv[1]);
  //aggiungi controllo se fork fallisce
  signal(SIGCONT, p1_handler);
  pid0 = getpid();
  pid1 = fork();

  if(pid1 == 0){  //Dentro P1
    pid1_1 = fork();
    if(pid1_1 == 0){ //Dentro a P1_1
      execlp("date", "date", (char*)0);
      perror("Errore con esecuzione di execlp in P1_1\n");
      exit(EXIT_FAILURE);
    }else{
      int signalP1;
      int signalCaller;
      pause();
    }
  }

  signal(SIGCONT, p2_handler);
  signal(SIGUSR1, p2_handler);
  signal(SIGUSR2, p2_handler);
  signal(SIGALRM, p2_handler);
  pid2 = fork();
  if(pid2 == 0){  //Dentro a P2
    alarm(N);
    //printf("P2: Attendo segnale\n");
    //pause();  //Aspetto che padre dica SIGUSR1 o 2
    //printf("P2: segnale ricevuto\n");
    while(true){
      if(p2_job == 1){
        S += (float)i/(i+1);
        i++;
      }else if(p2_job == 2){
        S += (float)(i+2)/(i+1);
        i++;
      }
    }

  }
  if(getpid() == pid0){
    signal(SIGCHLD, p0_handler);
    //Generato i due figlio
    if(pid0%2 == 0){  //Master pid pari
      kill(pid2, SIGUSR2);
    }else{            //Master pid dispari
      kill(pid2, SIGUSR1);
    }

    while(true){
      int remaining = sleep(2);
      printf("Sono il boss, il mio pid e': %d\n", getpid());
    }
  }
}
