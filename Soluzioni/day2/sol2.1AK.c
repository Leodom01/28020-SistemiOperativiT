#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAXN 30
#define MAXG 7

int M[MAXN][MAXG];
int G;
int N;

void figlio(int indice);
int cerca_indice(int *pid, int p);

int main(int argc, char *argv[]) {
int i, j, k, status, n_children;
int pid[MAXN], p; 


if (argc!=3)
{   printf("%s: sintassi sbagliata - 2 argomenti interi (G e N) \n", argv[0]);
    exit(1);
}

G = atoi(argv[1]);
N = atoi(argv[2]);
if (G>MAXG)
{   printf("%s: sintassi sbagliata - G minore o uguale a %d!\n", argv[0], MAXG);
    exit(1);
}

srand(time(NULL));
for ( i=0; i<N; i++ ) {
   for(j=0; j<G; j++)
       M[i][j]=rand()%36; // compenso tra 0 e 35
}

// stampa matrice:
printf("\n ----- DATI -----\n");
for ( i=0; i<N; i++ ) {
    printf("Compensi del rider %d:",  i);
    for(j=0; j<G; j++)
    {   
        printf("\t%d", M[i][j]);
    }
    printf("\n");
}
printf("----------------\n\n");


for ( i=0; i<N; i++ ) { // creazione figli
    pid[i] = fork();
    if ( pid[i] == 0 ) { // Eseguito dai figli
        figlio(i);
    }
    else if ( pid[i] > 0 ) { // Eseguito dal padre
        printf("%d: ho creato il figlio %d (PID %d)\n", getpid(),i, pid[i]);
    }
    else {
        perror("Fork error:");
        exit(1);
    }
    

}

for ( i=0; i<N; i++ ) { // attesa figli
    p=wait(&status);
    k=cerca_indice(pid,p); // ricerca indice del figlio di pid p
    if ((char)status==0) 
        printf("\nIl compenso totale del rider n.%d (pid=%d) è %d\n",k,p, status>>8);
    else 
        printf("term. involontaria  per segnale %d\n",(char)status);
}



}

void figlio(int indice)
{   int  j;
    int somma=0;

    for(j=0;j<G; j++)
        somma+=M[indice][j];
    
    exit(somma);
}

int cerca_indice(int *pid, int p){
    int i;
    for(i=0;i<N; i++)
        if (pid[i]==p)
            return i;
    return 0;
}

