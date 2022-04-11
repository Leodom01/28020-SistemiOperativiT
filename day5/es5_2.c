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

int p0, p1;
int A, V;
char STUD[255];
int terminato = 0;
int status;

bool is_correct(char* line);

int read_mat(char* line){
  char mat[7];
  strncpy(mat, &line[0], 6);
  mat[6] = '\0';
  return atoi(mat);
}

int read_year(char* line){
  char year[2];
  strncpy(year, &line[7], 1);
  year[1] = '\0';
  return atoi(year);
}

int read_avg(char* line){
  char avg[3];
  strncpy(avg, &line[9], 2);
  avg[2] = '\0';
  return atoi(avg);
}

void p0_handler(int signo){
  if(signo == SIGCHLD){
    terminato++;
  }
}

void usage(char* progName){
  fprintf(stderr, "Devi chiamare così il programma: \n%s STUD A V\n", progName);
}

int main(int argc, char* argv[]){

  //Controllo veloce dei parametri, poco perchè è noioso e not juicy
  if(argc != 4){
    usage("output");
  }

  //Lettura dei parametri in input
  strcpy(STUD, argv[1]);
  A = atoi(argv[2]);
  V = atoi(argv[3]);
  if((A<1 || A>3) || (V<0 || V>30)){
    perror("Hai inserito i parametri di input errati!\n");
    usage("output");
    exit(EXIT_FAILURE);
  }

  p0 = getpid();
  //creo la pipe
  int fd[2];
  int pipe_result = pipe(fd);
  if(pipe_result < 0){
    perror("Errore in fase di apertura della pipe, esco\n");
    exit(EXIT_FAILURE);
  }
  //Apro il file
  int stud_file = open(STUD, O_RDONLY);

  p1 = fork();

  if(p1 == 0){  //dentro a P1
    close(fd[0]);
    //Read the whole file and store the sendable lines
    char arg0[7];
    char arg1[255];
    close(1);
    dup(fd[1]);
    close(fd[1]);
    sprintf(arg0, ":%d:%d", A, V);
    sprintf(arg1, "%s", STUD);
    execlp("grep", "grep", arg0, arg1, (char*)0);
    //Close pipe
    close(fd[1]);
    exit(EXIT_SUCCESS);
  }else if(p1 >= 0){  //dentro a P0
    close(fd[1]);
    close(stud_file);
    int read_chars;
    do{
      char line[12];
      read_chars = read(fd[0], line, 12);
      if(read_chars > 0){
        printf("P0: Ho letto riga %s\n", line);
      }
    }while(read_chars > 0);
    if(terminato == 0){
      wait(&status);
    }
    printf("Sia io che il mio figlio abbiamo finito, ciao ciao! \n");
    exit(EXIT_SUCCESS);
  }else{
    perror("Ehy ciccio non posso creare un processo, harakiri, ciao\a\n");
    exit(EXIT_FAILURE);
  }

}
