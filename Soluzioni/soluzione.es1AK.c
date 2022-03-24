#include <stdio.h>
#include <string.h>
#include <ctype.h>

void usage(char* prog_name)
{
    fprintf(stderr,"Usage:\n\t%s: stud1 stud2 .. studM \n", prog_name);
}

int main(int argc, char* argv[]){    
    int Ni = 0;
    int Nm = 0;
    int Ne = 0;
    int i,j;
    
    if(argc == 1){
        printf("Sintassi sbagliata.\n");
        usage(argv[0]);
        return 0;
    }
    for(int i=1; i<argc; i++){
        if(strlen(argv[i])!=9){
            printf("Errore: la lunghezza di %s non è 9 caratteri\n", argv[i]);
            return 0;
        }
    }
    
    for(int i=1; i<argc; i++){
        if(strncmp(argv[i], "MED", 3) && strncmp(argv[i], "ING", 3) && strncmp(argv[i], "ECO", 3)){
            printf("Errore: l'identificatore della scuola di %s non è corretto.\n", argv[i]);
            return 0;
        }
        for(int j=3; j<9; j++){
            if(!isdigit(argv[i][j])){
                printf("Errore: la matricola di %s non è corretta.\n", argv[i]);
                return 0;
            }
        }
    }
    for(int i=1; i<argc; i++){
        if(!strncmp(argv[i], "ING", 3))
            Ni++;
        else if(!strncmp(argv[i], "MED", 3))
            Nm++;
        else if(!strncmp(argv[i], "ECO", 3))
            Ne++;
    }
    printf("Ci sono %d studenti di Ingegneria, %d di Medicina  e %d di Economia\n", Ni, Nm, Ne);
    if(Ni >= Nm && Ni >= Ne){
        printf("Il maggior numero di studenti è a Ingegneria; ecco le matricole:\n");
        for(i=1; i<argc; i++){
            if(!strncmp(argv[i], "ING", 3)){
                for(j = 3; j<9; j++)
                    printf("%c", argv[i][j]);
                printf("\n");
            }
        }
    }
    else if(Nm >= Ni && Nm >= Ne){
        printf("Il maggior numero di studenti è a Medicina; ecco le matricole:\n");
        for(i=1; i<argc; i++){
            if(!strncmp(argv[i], "MED", 3)){
                for(j = 3; j<9; j++)
                    printf("%c", argv[i][j]);
                printf("\n");
            }
        }
    }
    else if(Ne >= Ni && Ne >= Ni){
        printf("Il maggior numero di studenti è a Economia; ecco le matricole:\n");
        for(i=1; i<argc; i++){
            if(!strncmp(argv[i], "ECO", 3)){
                for(j = 3; j<9; j++)
                    printf("%c", argv[i][j]);
                printf("\n");
            }
        }
    }
    return 0;
}
