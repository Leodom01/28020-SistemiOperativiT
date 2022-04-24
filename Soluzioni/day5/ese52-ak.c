#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>




#define MAXS 256
void print_usage(char* prog_name){
    fprintf(stderr, "Usage:\n\t%s STUD A V\n", prog_name);
}
void figlio(int fd_out, char filein[], char A[], char V[]);
void wait_child();

int pp[2]; // devo predisporre una pipe per la comunicazione  figlio ->padre

int main(int argc, char **argv){
   int pid;
   int i,nread, fine=0; 
   char c;
   
   
   if (argc<4) // almeno 3 argomenti: stud A V
        {       print_usage(argv[0]);
                exit(-1);
        }
    //TODO:controllo fine argomenti
    
    if (pipe(pp)<0) 
        exit(-3); /* apertura pipe fallita */
    
 /* creazione figlio */
	if ((pid=fork())<0) {  perror("fork error"); exit(-3);}
    else if (pid==0)
    {   close(pp[0]); // ogni figlio chiude il lato di lettura di pp
        figlio(pp[1], argv[1], argv[2], argv[3]);
    }
    
    close(pp[1]); // il padre chiude il lato di scrittura di pp
    
    while (!fine)
    {   nread=read(pp[0], &c, 1);
        if (nread==0) // ho finito
            fine=1;
        else 
            write(1, &c, 1);
    }
    wait_child();
    close(pp[0]); // chiudo la pipe
    exit(0);
} /* fine main */


void figlio(int fd_out, char filein[], char A[], char V[])
{   int fd_in, i, nread, fine=0;
    char stringa[5];
    //costruzione della stringa da cercare:
    stringa[0]=A[0];
    stringa[1]=':';
    for (i=2; i<4;i++)
        stringa[i]=V[i-2];
    stringa[4]='\0';
    printf("Figlio: ecco la stringa da cercare: %s\n", stringa);
  //redirezione input da file:
  close(0);
  fd_in=open(filein, O_RDONLY);
  if (fd_in<0)
  {     perror("APERTURA FILE STUD FALLITA");
        exit(-5);
  }
  //redirezione output su pipe:
  close(1);
  dup(fd_out);
  close(fd_out);
  execlp("grep", "grep", stringa, (char *)0);
  exit(0);
}


void wait_child() {
    int pid_terminated,status;
    pid_terminated=wait(&status);
    if(WIFEXITED(status))
        printf("PADRE: terminazione volontaria del figlio %d con stato %d\n",
                pid_terminated, WEXITSTATUS(status));
    else if(WIFSIGNALED(status))
        printf("PADRE: terminazione involontaria del figlio %d a causa del segnale %d\n",
                pid_terminated,WTERMSIG(status));
}
