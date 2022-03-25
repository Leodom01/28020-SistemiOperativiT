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

  int gen_pid[argc-3];

  //Genero processi per cp
  for (int i = 3; i < argc; i++){
    int pid = fork();
    if(pid == 0){ //Sono dentro al processo figlio
      char file_path[255] = "";
      strcpy(file_path, dir_1);
      strcat(file_path, "/");
      strcat(file_path, argv[i]);
      int inner_pid = fork();
      int my_status;
      //Thread per la copia del file
      if(inner_pid == 0){ //Dentro al thread di cp
        //printf("%s -> param primo \n%s -> param secondo\n------------------\n", file_path, dir_2);
        execlp("cp", "cp", file_path, dir_2, (char*)0);
        char err_log[1024] = "";
        sprintf(err_log, "Problema nella cp di %s, pid:%d ", file_path, getpid());
        perror(err_log);
        exit(EXIT_FAILURE);
      }else if(inner_pid > 0){
        //printf("Processo %d sta facendo cp per conto di %d\n", inner_pid, getpid());
      }else{
        char err_log[1024] = "";
        sprintf(err_log, "Problema generazione di fork! ");
        perror(err_log);
        exit(EXIT_FAILURE);
      }
      wait(&my_status);
      if(WIFEXITED(my_status) && WEXITSTATUS(my_status) == EXIT_SUCCESS){
        printf("%d: figlio %d mi ha copiato correttamente il fie. Ora rinomino \n", getpid(), inner_pid);
      }else{
        char err_log[1024] = "";
        sprintf(err_log, "%d: figlio %d ha fatto casini, ciao. Exti code %d", getpid(), inner_pid, WEXITSTATUS(my_status));
        perror(err_log);
        exit(EXIT_FAILURE);
      }
      //Ora eseguo renaming
      char file_dest[255] = "";
      strcpy(file_dest, dir_1);
      strcat(file_dest, "/");
      char pid_string[255] = "";
      sprintf(pid_string, "%d", getpid());
      strcat(file_dest, pid_string);
      //printf("%s -> param primo \n%s -> param secondo\n------------------\n", file_path, file_dest);
      execlp("mv", "mv", file_path, file_dest, (char*)0);
      char err_log[1024] = "";
      sprintf(err_log, "Problema nella mv di %s, pid:%d ", file_path, getpid());
      perror(err_log);
      exit(EXIT_FAILURE);
    }else if(pid > 0){ //Dentro al padre
      printf("Generato figlio master pid: %d \n", pid);
      gen_pid[i-3] = pid;
    }else{
      char err_log[1024] = "";
      sprintf(err_log, "Problema generazione di fork! \n");
      perror(err_log);
      exit(EXIT_FAILURE);
    }
  }

    bool all_good = true;


    for(int i = 3; i < argc; i++){
      int pid_figlio = wait(&status);
      if(!(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) || WIFSIGNALED(status)){
        fprintf(stderr, "Errore con un processo, chiudo tutti babies!\n");
        return EXIT_FAILURE;
      }
    }

    return EXIT_SUCCESS;

}
