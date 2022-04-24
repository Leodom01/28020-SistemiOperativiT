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

void p0_handler(){
  exit(EXIT_SUCCESS);
}

typedef struct{
  int matr;
  int anno;
  int voto;
}student;

void usage(char programName[]){
  printf("Utilizza il comando come: %s STUD A V\nV compreso fra 0 e 30\nA compreso fra 1 e 3\nSTUD nome del file in cui ci sono i record degli studenti\n", programName);
}

int main(int argc, char* argv[]){

  if(argc != 4){
    usage("output");
    exit(EXIT_FAILURE);
  }

  int V = atoi(argv[3]);
  int A = atoi(argv[2]);
  char STUD[255] = "";
  strcpy(STUD, argv[1]);

  if(V > 30 || V < 0 || A < 1 || A > 3){
    usage("output");
  }

  int fd[2];
  int pipeResult = pipe(fd);
  if(pipeResult != 0){
    perror("Errore in fase di creazione della pipe, sto uscendo...\n");
    exit(EXIT_FAILURE);
  }
  signal(SIGCHLD, p0_handler);
  int pid1 = fork();
  if(pid1 == 0){  //Child branch
    close(fd[0]); //Chiudo lato di scrittura della pipe
    //Leggi tutte el righe e mettile in pipe
    char newLine[12] = "";
    int file = open(STUD, O_RDONLY);
    while(read(file, newLine, 12) > 0){
      student read;
      sscanf(newLine, "%d:%d:%d\n", &read.matr, &read.anno, &read.voto);
      if(read.anno == A && read.voto == V){
        char toPrint[12] = "";
        sprintf(toPrint, "%d:%d:%d", read.matr, read.anno, read.voto);
        write(fd[1], toPrint, 12);
      }
    }
    close(file);
    exit(EXIT_SUCCESS);
  }else if(pid1 > 0){ //Father branch
    close(fd[1]);
    char toPrint[12];
    while(read(fd[0], toPrint, 12) != 0){
      printf("Ho appena letto: %s\n", toPrint);
    };
    close(fd[1]);
  }else{
    perror("Errore in fase di generazione della branch chiudo tutto, ciao ciao...\n\a");
    exit(EXIT_FAILURE);
  }

  printf("Ho stampato tutto il necessario addios!\n\a");
  exit(EXIT_SUCCESS);

}
