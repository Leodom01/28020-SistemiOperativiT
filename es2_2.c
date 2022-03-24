#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

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
  bool childError = false;

  //Creazione di tutti i figli con operazione dei figli
  for(int i = 3; i < argc; i++){
    int pid = fork();
    if(pid == 0){           //Codice eseguito dal figlio
      if(getpid()%2 == 0){  //Eseguito da figli con pid pari
        char source[2048];
        strcpy(source, dir_1);
        strcat(source, "/");
        strcat(source, argv[i]);
        //printf("%d copying %s\n", getpid(), argv[i]);
        execl("/bin/cp", "cp", source, dir_2, (char*)0);
        exit(1);
      }else{                //Eseguito da figli con pid dispari
        char toDelete[2048];
        strcpy(toDelete, dir_1);
        strcat(toDelete, "/");
        strcat(toDelete, argv[i]);
        //printf("%d removing %s\n", getpid(), argv[i]);
        execl("/bin/rm", "rm", toDelete, (char*)0);
        exit(1);
      }
    }else if(pid < 0){     //Codice di errore
      perror("Fork creation error!\n");
      exit(1);
    }
  }

  //Raccolgo tutti i risultati
  for(int i = 3; i < argc && !childError; i++){
    int pid_figlio = wait(&status);
    if(WIFSIGNALED(status)){
      printf("Errore scatenato dal processo con pid:%d\n", pid_figlio);
      childError = true;
    }
  }
  if(childError){
    printf("Chiusura, causa errore no ls per nessuno! :/ \n");
    return 0;
  }else{
    execl("/bin/ls", "ls", dir_2, (char*)0);
    exit(1);
  }



}
