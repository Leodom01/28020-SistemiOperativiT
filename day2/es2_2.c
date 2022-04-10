#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 255

int main(int argc, char *argv[]){

  //Controllo dei parametri
  if(argc < 4){
    printf("Parametri sbagliati\n");
    return -1;
  }

  //Potrei controllare se i file sono davvero file e le directory davvero directory ma mi fido

  char dir_1[strlen(argv[1]+1)];
  char dir_2[strlen(argv[2]+1)];
  strcpy(dir_1, argv[1]);
  strcpy(dir_2, argv[2]);
  int status;

  //Creazione di tutti i figli con operazione dei figli
  for(int i = 3; i < argc; i++){
    int pid = fork();
    if(pid == 0){           //Codice eseguito dal figlio
      if(getpid()%2 == 0){  //Eseguito da figli con pid pari
        //Copia di file dir1 -> dir2
        printf("Figlio %d esegue cp...\n", getpid());
        char dir_and_file[MAX_STRING_LENGTH] = "";
        char error_log[MAX_STRING_LENGTH] = "";
        strcat(dir_and_file, dir_1);
        strcat(dir_and_file, "/");
        strcat(dir_and_file, argv[i]);
        execlp("cp", "cp", dir_and_file, dir_2, (char*)0);
        sprintf(error_log, "Errore in fase di copiatura di %s , pid:%d", dir_and_file, getpid());
        perror(error_log);
        exit(EXIT_FAILURE);
      }else{                //Eseguito da figli con pid dispari
        //Rmiozione di file su dir1
        printf("Figlio %d esegue rm...\n", getpid());
        char dir_and_file[MAX_STRING_LENGTH] = "";
        char error_log[MAX_STRING_LENGTH] = "";
        strcat(dir_and_file, dir_1);
        strcat(dir_and_file, "/");
        strcat(dir_and_file, argv[i]);
        execlp("rm", "rm", dir_and_file, (char*)0);
        sprintf(error_log, "Errore in fase di rimozione di %s , pid:%d", dir_and_file, getpid());
        perror(error_log);
        exit(EXIT_FAILURE);
      }
    }else if(pid < 0){     //Codice di errore
      char buff[1024] = "";
      sprintf(buff,"Padre: impossibile fare fork %d\n", i);
      perror(buff);
      exit(EXIT_FAILURE);
    }
  }

  //Se tutti i figli terminano volontariamente: stampa ls di dir2
  //Se almeno un figlio non termina volontariamente allora stampa errore e pid errore

  //Raccolgo tutti i risultati
  bool childError = false;
  for(int i = 3; i < argc; i++){
    int pid_figlio = wait(&status);
    if(WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS){
      fprintf(stderr, "Padre: errore del figlio: %d\n", pid_figlio);
      childError = true;
    }else if(WIFSIGNALED(status)){
      fprintf(stderr, "Padre: figlio interrotto, errore complesso.\n");
      childError = true;
    }
  }
  printf("Lo stato di childError e': %d \n", childError);
  if(!childError){
    execlp("ls", "ls", dir_2, (char*)0);
    perror("ls del padre fallita\n");
    return EXIT_FAILURE;
  }


}
