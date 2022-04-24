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

void usage(){
  printf("L'utilizzo corretto e': output Fin Fe Fu\nTermino, addio...\n\a");
}

int main(int argc, char* argv[]){

  if(argc != 4){
    usage();
    exit(EXIT_FAILURE);
  }

  int fin, fe, fu;
  fin = open(argv[1], O_RDONLY);
  fe = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
  fu = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);

  if( fin < 0 || fe < 0 || fu < 0){
    perror("Errore in fase di apertura dei file.\n Addio...\n\a");
  }

  char box;
  char string[12] = "";
  while(read(fin, &box, sizeof(char)) > 0 ){
    if(box == '\n'){  //Arrivato a fine linea
      //printf("Ho finito di leggere: %s\n", string);
      record got = strToRec(string);
      info toWrite = recToInfo(got);
      if(got.tipo == 'E'){
        write(fe, &toWrite, sizeof(info));
        printf("Ho appena scritto su entrate: Data: %d Importo: %d\n", toWrite.giorno, toWrite.importo);
      }else{
        write(fu, &toWrite, sizeof(info));
        printf("Ho appena scritto su uscite: Data: %d Importo: %d\n", toWrite.giorno, toWrite.importo);
      }
      string[0] = '\0';
    }else{
        int length = strlen(string);
        string[length] = box;
        string[length+1] = '\0';
    }
  }

  close(fin);
  close(fe);
  close(fu);

}
