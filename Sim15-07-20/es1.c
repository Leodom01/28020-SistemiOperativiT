#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

void usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s Fin C S word\n", prog_name);
}

int pid0, pid1, pid2;
char C, S;
int MAX_FILESIZE = 8124;

int main(int argc, char* argv[]){

  if(argc != 5){
    usage("output");
    perror("Parametri inserito sbagliati, too bad...\n");
    exit(-1);
  }else if(argv[1][0] != '/'){
    usage("output");
    perror("Il file deve essere path assoluto, too bad...\n");
    exit(-2);
  }else if(strlen(argv[2]) != 1 || strlen(argv[3]) != 1){
    usage("output");
    perror("C e S devono essere caratteri, too bad...\n");
    exit(-3);
  }else if(strlen(argv[4]) <= 1){
    usage("output");
    perror("word deve essere una stringa, too bad...\n");
    exit(-4);
  }

  C = argv[2][0];
  S = argv[3][0];

  int this_pipe[2];
  if(pipe(this_pipe) < 0){
    perror("Errore nella creazione della pipe\n");
    exit(-5);
  }

  pid0 = getpid();

  pid1 = fork();
  if(pid1 == 0){                //Dentro a P1
    close(this_pipe[1]);
    close(0);
    dup(this_pipe[0]);
    execlp("grep", "grep", argv[4], (char *) NULL);
    close(this_pipe[0]);
    printf("P1: Ho fatto quello che dovevo fare ora muoio...\n");
    exit(0);
  }else if(pid1 > 0){           //Dentro  P0
    close(this_pipe[0]);
    pid2 = fork();
    if(pid2 == 0){              //Dentro a P2
      char read_char;
      char string_read[MAX_FILESIZE];
      int char_num_read = 0;
      int fin = open(argv[1], O_RDONLY);
      if(fin < 0){
        perror("Errore nella apertura del file, esco...");
        exit(-5);
      }
      if(getpid()%2 == 0){      //P2 ha pid pari
        printf("P2: Inizio a leggere da pid pari...\n");
        while(read(fin, &string_read[char_num_read], sizeof(char)) > 0){
          if(string_read[char_num_read] == C){
            string_read[char_num_read] = S;
          }
          char_num_read++;
          //write(this_pipe[1], &read, sizeof(char));
        }
        write(this_pipe[1], &string_read, strlen(string_read));
        close(this_pipe[1]);
      }else{
        printf("P2: Inizio a leggere da pid dispari...\n");                   //P2 ha pid dispari
        while(read(fin, &string_read[char_num_read], sizeof(char)) > 0){
          //write(this_pipe[1], &read_char, sizeof(char));
          char_num_read++;
        }
        write(this_pipe[1], &string_read, strlen(string_read));
        close(this_pipe[1]);
      }
      close(fin);
      printf("P2: Ho fatto quello che dovevo fare ora muoio...\n");
      exit(0);
    }else if(pid2 > 0){         //Dentro a P0
      close(this_pipe[1]);
    }else{
      perror("Errore nella creazione della fork, esco...");
      exit(-5);
    }
  }else{
    perror("Errore nella creazione della fork, esco...");
    exit(-5);
  }

  //RAaccolgo i pid
  for(int i = 0; i<2; i++){
    int gathered_pid, status;
    gathered_pid = wait(&status);
    if(pid1 == gathered_pid){
      if(WIFEXITED(status))
          printf("PADRE: terminazione volontaria del figlio P1 con pid %d e stato %d\n",
                  gathered_pid, WEXITSTATUS(status));
      else if(WIFSIGNALED(status))
          printf("PADRE: terminazione involontaria del figlio P1 con pid %d e segnale %d\n",
                  gathered_pid,WTERMSIG(status));
    }else if(pid2 == gathered_pid){
      if(WIFEXITED(status))
        printf("PADRE: terminazione volontaria del figlio P2 con pid %d e stato %d\n",
                gathered_pid, WEXITSTATUS(status));
      else if(WIFSIGNALED(status))
          printf("PADRE: terminazione involontaria del figlio P2 con pid %d e segnale %d\n",
                  gathered_pid,WTERMSIG(status));
    }
  }
  printf("P0: Ho chiuso tutto, ora vado a casa...\n");
  exit(0);


}
