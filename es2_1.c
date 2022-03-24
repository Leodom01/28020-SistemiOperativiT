#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_SALARY 35

int main(int argc, char* argv[]){

  if(argc<2){
    printf("Serovno parametri cicci*!\n");
    return -1;
  }
  //Add some input checks here

  //move params to local vars
  int n = atoi(argv[2]);
  int g = atoi(argv[1]);

  //check some input correctness, little cause it's boring
  if(n <= 0 || g <= 0 || g>7 ){
    return -1;
  }

  //rand init
  srand(time(NULL));

  //allocate and fill matrix
  int** matrix = (int**)malloc(n*sizeof(int*));

  for(int i = 0; i<n; i++){
    matrix[i] = (int*)malloc(g*sizeof(int));
  }

  for(int i = 0; i<n; i++){
    for(int j = 0; j<g; j++){
      matrix[i][j] = 1 + rand()%MAX_SALARY;
    }
  }

  //prints matrix
  for(int i = 0; i<n; i++){
    printf("\n");
    for(int j = 0; j<g; j++){
      printf(" %2d", matrix[i][j]);
    }
  }
  //Without this i got % at the end of the matrix
  printf("\n");

  //Start computating and printing salary for each rider

  //Here I have the list of all of the subprocess valvulating the i-rider salary
  //int pidList[] = (int*)malloc(sizeof(int)*n);
  int pid, status;
  for(int rider_val = 0; rider_val<n ; rider_val++){
    pid = fork();
    if(pid == 0){  //Child's branch
      int toReturn = 0;
      for(int loopIndex = 0; loopIndex<g; loopIndex++){
        toReturn+=matrix[rider_val][loopIndex];
      }
      exit(toReturn);
    }else if(pid > 0){  //Parent's branch
      pid = wait(&status);
      printf("Rider %d, PID %d, salario: %d\n", rider_val, pid, WEXITSTATUS(status));
    }
  }

  return 0;

}
