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
  int giorno;
  int importo;
}info;

int main(int argc, char* argv[]){

  //Check dei parametri in input
  int fin, fe, fu;
  info in_list[MAX_INFOS];
  int in_list_size = 0;
  info out_list[MAX_INFOS];
  int out_list_size = 0;

  char line[12] = "";
  int line_index = 0;
  int read_char;

  fin = open(argv[1], O_RDONLY);
  if(fin == -1){
    printf("Errore in fase di apertura del file bye bye.\n");
    return -1;
  }else{
    printf("Ho aperto il file e sono dentro l'analisi\n");
    while(read(fin, &read_char, sizeof(char)) > 0){
      if(read_char != '\n'){
        line[line_index] = read_char;
        line[line_index+1] = '\n';
        line_index++;
      }else{
        info toAdd;
        char destination;
        sscanf(line, "%c-%d-%d", &destination, &(toAdd.giorno), &(toAdd.importo));
        printf("Ho letto: %s", line);
        //Controlla che i valori siano giusti
        if(destination == 'U'){
          out_list[out_list_size] = toAdd;
          out_list_size++;
        }else if(destination == 'E'){
          in_list[in_list_size] = toAdd;
          in_list_size++;
        }
        line_index = 0;
      }
    }
    printf("Finito la lettura\n");

    fe = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(fe < 0){
      printf("Errore in fase di apertura del file fe\n");
    }else{
      for(int i = 0; i<in_list_size; i++){
        write(fe, &in_list[i], sizeof(info));
      }
    }

    fu = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(fu < 0){
      printf("Errore in fase di apertura del file fu\n");
    }else{
      for(int i = 0; i<out_list_size; i++){
        write(fe, &out_list[i], sizeof(info));
      }
    }

    close(fin);
    close(fe);
    close(fu);

    printf("Finito people!\n");

  }


}
