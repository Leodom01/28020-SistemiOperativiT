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

void alarm_p0_recieved(int signum);

void sigusr_p1_recieved(int signum);

int pid0, pid1, pid2;
int pp[2];
int M;
//NEED TO CHECK
int main(int argc, char* argv[]){

  pid0 = getpid();

  //Come posso controllare che i file esistano o non esistano?
  if(argc != 4){
    usage("output");
    printf("Numero di parametri sbagliati\n");
    exit(-1);
  }else if(atoi(argv[2]) < 0){
    usage("output");
    printf("Intero devono essere maggiori di 0\n");
    exit(-1);
  }else if(argv[1][strlen(argv[1])-1] != 't' || argv[1][strlen(argv[1])-2] != 'x' ||
            argv[1][strlen(argv[1])-3] != 't' || argv[1][strlen(argv[1])-4] != '.'){
    usage("output");
    printf("Valore del path non assoluto!\n");
    exit(-1);
  }

  if (pipe(pp)<0){
    perror("Apertura pipe fallita");
    exit(-3);
  }

  pid1 = fork();
  if(pid1 < 0){ perror("Problema nella creazione di un figlio, termino."); exit(-1);}
  if(pid1 == 0){      //Dentro a P1
    close(pp[0]);
    signal(SIGUSR1, sigusr_p1_recieved);
    printf("P1: Apro il file di input...\n");
    int fin = open(argv[1], O_RDONLY);
    if (fin<0)
    {     perror("Apertura Fin fallita...\n");
          exit(-5);
    }
    M = 0;
    while(true){
      printf("P1: Inizio un ciclo di lettura...\n");
      int size_counter = lseek(fin, 0, SEEK_END);
      for(int i = 1; i<=size_counter; i++){
        lseek(fin, -i, SEEK_END);
        char read_char;
        if(read(fin, &read_char, sizeof(char)) < 0){perror("Errore in fase di lettura...\n"); exit (-6);}
        write(pp[1], &read_char, sizeof(char));
        if(read_char == '\n'){M++;}
      }
    }
  }
  close(pp[1]);
  pid2 = fork();
  if(pid2 < 0){ perror("Problema nella creazione di un figlio, termino."); exit(-1);}
  if(pid2 == 0){    //Dentro a P2
    printf("P2: Sono nato\n");
    int fout = open(argv[3], O_WRONLY|O_CREAT, 0777);
    if(fout < 0){perror("Errore in fase di apertura del file di output...\n"); exit(-6);}
    close(1);
    dup(fout);
    close(fout);
    close(0);
    dup(pp[0]);
    close(pp[0]);
    execlp("cat", "cat", (char *)0);
    printf("P2: Finito di stampare, ora muoio...\n");
    //close(pp[0]);
    exit(0);
  }

  //close(pp[0]);
  signal(SIGALRM, alarm_p0_recieved);
  alarm(atoi(argv[2]));
  while(true){
    sleep(1);
    printf("P0: pid: %d\n", pid0);
  }
}

void usage(char* progName){
  perror("Devi chiamare così il programma: \nouput Fin W T\n");
}

void alarm_p0_recieved(int signum){
    kill(pid1, SIGUSR1);
    printf("P0: Ho segnalato a P1 ora muoio...\n");
    exit(0);
}

void sigusr_p1_recieved(int signum){
  close(pp[1]);
  printf("P1: in totale ho inviato %d volte il carattere newline a P2.\n", M);
  exit(0);
}
