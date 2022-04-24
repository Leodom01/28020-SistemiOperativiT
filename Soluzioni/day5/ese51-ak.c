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
    char riga[12], anno[2], voto[3];
  
  fd_in=open(filein, O_RDONLY);
  if (fd_in<0)
  {     perror("APERTURA FILE STUD FALLITA");
        exit(-5);
  }
  
  while (!fine)
  {     nread=read(fd_in, riga, 12);
        if (nread==0)
            fine=1;
        else // analisi riga letta:
        {   anno[0]=riga[7];
            anno[1]='\0';
            for (i=0; i<2;i++)
                voto[i]=riga[i+9];
            voto[2]='\0';
            if ((!strcmp(anno, A))&&(!strcmp(voto, V)))
            {   printf("trovata riga con anno %s e media %s\n", A, V);
                write(fd_out, riga, 12);
            }
        }
  }
  close(fd_out); //chiudo lato scrittura pipe
  close(fd_in); //chiudo il file STUD
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
