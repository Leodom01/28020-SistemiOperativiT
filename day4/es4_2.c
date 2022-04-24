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

#define MAX_INFOS 255

typedef struct{
  char tipo;
  int giorno;
  int importo;
} record;

typedef struct{
int giorno;
int importo;
}info;

record strToRec(char str[]){
  record toReturn;
  sscanf(str, "%c-%d-%d", &toReturn.tipo, &toReturn.giorno, &toReturn.importo);
  return toReturn;
}

info recToInfo(record rec){
  info toReturn;
  toReturn.giorno = rec.giorno;
  toReturn.importo = rec.importo;
  return toReturn;
}

void lineToString(int fileIn, char* toFill){
  char box;
  char string[255] = "";
  while(read(fileIn, &box, sizeof(char)) > 0 ){
    if(box == '\n'){  //Arrivato a fine linea
      strcpy(toFill, string);
      return;
    }else{
        int length = strlen(string);
        string[length] = box;
        string[length+1] = '\0';
    }
  }
}

void usage(){
  printf("L'utilizzo corretto e': output Fe Fu G N\nTermino, addio...\n\a");
}

int main(int argc, char* argv[]){

  int pid0, pid1, pid2;
  int G = atoi(argv[3]);
  int N = atoi(argv[4]);

  if(argc != 5){
    usage();
    exit(EXIT_FAILURE);
  }

  if(G < 1 || G > 31 || N < 1 || N > 1000000){
    usage();
    printf("G deve essere fra 1 e 31 e N deve essere fra 0 e 999.999");
  }

  int fe, fu;
  fe = open(argv[1], O_RDONLY);
  fu = open(argv[2], O_RDONLY);

  if(fe < 0 || fu < 0){
    perror("Errore in fase di apertura dei file.\n Addio...\n\a");
  }

  int terminatedbranches[2];
  terminatedbranches[0] = 0;
  terminatedbranches[1] = 0;
  pid0 = getpid();
  pid1 = fork();
  if(pid1 == 0){ //Dentro la branch P1
    //Cerco entrata da giorno G in poi, con importo maggiore di N, solo la piú recente
    info top, box;
    top.giorno = G-1;
    top.importo = N;
    while(read(fe, &box, sizeof(info)) > 0 ){
      if(box.giorno >= top.giorno && box.importo >= N){
        top = box;
      }
    }
    if(top.giorno == G-1){  //Non ho trovato nulla
      exit(1);
    }else{
      int temp1 = open("./temp1", O_WRONLY | O_CREAT | O_TRUNC, 0777);
      char toPrint[256];
      sprintf(toPrint, "Data: %d Importo: %d\n", top.giorno, top.importo);
      write(temp1, toPrint, strlen(toPrint));
      close(temp1);
    }
    exit(0);
  }else if(pid1 < 0){
    perror("Errore in fase di creazione della branch, chiudo tutto ed esco, bye...\n");
    close(fe);
    close(fu);
    exit(EXIT_FAILURE);
  }

  pid2 = fork();
  if(pid2 == 0){ //Dentro la branch P2
    info top, box;
    top.giorno = G-1;
    top.importo = N;
    while(read(fu, &box, sizeof(info)) > 0 ){
      if(box.giorno >= top.giorno && box.importo >= N){
        top = box;
      }
    }
    if(top.giorno == G-1){  //Non ho trovato nulla
      exit(1);
    }else{
      int temp2 = open("./temp2", O_WRONLY | O_CREAT | O_TRUNC, 0777);
      char toPrint[256];
      sprintf(toPrint, "Data: %d Importo: %d\n", top.giorno, top.importo);
      write(temp2, toPrint, strlen(toPrint));
      close(temp2);
    }
    exit(0);
  }else if(pid2 < 0){
    perror("Errore in fase di creazione della branch, chiudo tutto ed esco, bye...\n");
    close(fe);
    close(fu);
    exit(EXIT_FAILURE);
  }

  int status;
  int terminated;
  do{
    terminated = wait(&status);
    if(WEXITSTATUS(status) == 0){
      if(terminated == pid1){
        int temp1 = open("./temp1", O_RDONLY);
        char toReturnString[255] = "";
        lineToString(temp1, toReturnString);
        close(temp1);
        printf("P1 ha ritornato: %s\n", toReturnString);
        terminatedbranches[0] = 1;
      }else if(terminated == pid2){
        int temp2 = open("./temp2", O_RDONLY);
        char toReturnString[255] = "";
        lineToString(temp2, toReturnString);
        close(temp2);
        printf("P2 ha ritornato: %s\n", toReturnString);
        terminatedbranches[1] = 1;
      }
    }else if(WEXITSTATUS(status) == 1){
      printf("Il processo %d non ha trovato operazioni con importo maggiore di %d\n", terminated, N);
    }
  }while(!(terminatedbranches[0] == 1 && terminatedbranches[1] == 1));

  close(fe);
  close(fu);

}
