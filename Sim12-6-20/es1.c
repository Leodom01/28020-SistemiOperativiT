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

void p2_timeout_received(int signum);

void alarm_p2_recieved(int signum);

int pid0;
int pp[2];
int fin;

int main(int argc, char* argv[]){

  int pid1, pid2;
  pid0 = getpid();

  //Controllo dei parametri
  if(argc != 4){
    usage("output");
    printf("Numero di parametri sbagliati\n");
    exit(-1);
  }else if(atoi(argv[2]) < 0 || atoi(argv[3]) < 0){
    usage("output");
    printf("Interi devono essere maggiori di 0\n");
    exit(-1);
  }else if(argv[1][0] != '/'){
    usage("output");
    printf("Valore del path non assoluto!\n");
    exit(-1);
  }
  //Controllo dei parametri finito (devo contrallare anche che il file esista?)

  if (pipe(pp)<0){
    perror("Apertura pipe fallita");
    exit(-3);
  }
  pid1 = fork();
  if(pid1 < 0){ perror("Problema nella creazione di un figlio, termino."); exit(-1);}
  if(pid1 == 0){  //Dentro a pid1
    close(pp[1]);    //Chiudo lato scrittura pipe
    int this_val = 0;
    int last_val = 0;
    int letture = 0;
    while(read(pp[0], &this_val, sizeof(this_val)) != 0){
      if(letture > 0){
        if(this_val > last_val){
          printf("P1: Lettura %d: %d caratteri in piu'\n", letture, this_val-last_val);
        }else if(this_val < last_val){
          printf("P1: Lettura %d: %d caratteri in meno\n", letture, last_val-this_val);
        }
      }
      letture++;
      last_val = this_val;
    }
    //Pipe chiusa, devo morire
    printf("P1: Lettura terminata!\nP1: Mi suicido\n");
    exit(0);
  }

  close(pp[0]);     //Chiudo lato lettura pipe

  pid2 = fork();
  if(pid2 < 0){ perror("Problema nella creazione di un figlio, termino."); exit(-1);}
  if(pid2 == 0){  //Dentro a pid2
    close(pp[1]);
    signal(SIGALRM, alarm_p2_recieved);
    alarm(atoi(argv[3]));
    while(true){
      printf("P2: mio pid: %d\n", getpid());
      sleep(2);
    }
  }


  signal(SIGUSR1, p2_timeout_received);
  //Apro Fin


  int sleep_feedback;
  while(true){
    fin = open(argv[1], O_RDONLY);
    if (fin<0)
    {   perror("open Fin fallita: ");
        exit(-2);
    }
    int char_count = 0;
    char just_read;
    while(read(fin, &just_read, 1) != 0){
      char_count++;
    }
    lseek(fin, 0, 0);
    write(pp[1], &char_count, sizeof(char_count));
    close(fin);
    sleep(atoi(argv[2]));
  }
}

void usage(char* progName){
  perror("Devi chiamare così il programma: \nouput Fin W T\n");
}

void p2_timeout_received(int signum){
  printf("P0: Ho ricavuto il timeout da P2!\n");
  close(pp[1]);
  close(fin);
  exit(0);
}

void alarm_p2_recieved(int signum){
  printf("P2: Arrivato il mio timeout\n");
  kill(pid0, SIGUSR1);
  exit(0);
}
