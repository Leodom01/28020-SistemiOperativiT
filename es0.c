#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

    /*
     * Returuns a number based on the course, based on this dictionary:
     * 0 = ING
     * 1 = MED
     * 2 = ECO
     * -1 = wrong code
     */
    int getCourse(char string[]){
        if(strncmp(string, "ING", 3) == 0){
            return 0;
        }else if(strncmp(string, "MED", 3) == 0){
            return 1;
        }else if(strncmp(string, "ECO", 3) == 0){
            return 2;
        }else{
            return -1;
        }
    }

    /*
     * Returns the matr number or -1 if it is uncorrect
     */
    int getNumber(char string[]){
        if(!((int)string[3]<=57 && (int)string[3]>= 48) &&
            ((int)string[4]<=57 && (int)string[4]>= 48) &&
            ((int)string[5]<=57 && (int)string[5]>= 48) &&
            ((int)string[6]<=57 && (int)string[6]>= 48) &&
            ((int)string[7]<=57 && (int)string[7]>= 48) &&
            ((int)string[8]<=57 && (int)string[8]>= 48) &&
            (string[9] == '\n')){
            return -1;
        }else{
            return (((int)string[8]-48)*1 + ((int)string[7]-48)*10 + ((int)string[6]-48)*100 + ((int)string[5]-48)*1000 + ((int)string[4]-48)*10000 + ((int)string[3]-48)*100000);
        }
    }

    bool isCorrect(char string[]){
        if(getCourse(string) == -1){return false;}
        if(getNumber(string) == -1){return false;} else{return true;}
    }

int main(int argc, char *argv[]){

    //Check that there's at least one entry
    if(!(argc >= 2 && isCorrect(argv[1]))){
            printf("We, my friend, non va mica bene quell'input li eh!    :/ \a \n");
            return -1;
    }

    //Check that all of the entries are legal
    for(int i = 1; i<argc; i++){
        if(!isCorrect(argv[i])){
           printf("We, my friend, non va mica bene questo input eh: %s    :/ \a \n", argv[i]);
            return -1;
        }
    }

    //Count the winning team (ENG obv)
    int engStud = 0;
    int medStud = 0;
    int ecoStud = 0;
    //Will be filled in with the same logic I used for the function getCourse
    int winningTeam;
    for(int i = 1; i<argc; i++){
        if(getCourse(argv[i]) == 0){engStud++;}
        if(getCourse(argv[i]) == 1){medStud++;}
        if(getCourse(argv[i]) == 2){ecoStud++;}
    }
    printf("And the winning team is......:");
    if(engStud>=medStud && engStud>=ecoStud){
        printf("ENG! :) \n");
        winningTeam = 0;
    }else if(medStud>=engStud && medStud>=ecoStud){
        printf("MED... :( \n");
        winningTeam = 1;
    }else if(ecoStud>=medStud && ecoStud>=engStud){
        printf("ECO... :( \n");
        winningTeam = 2;
    }

    //Print all of the winners
    for(int i = 1; i<argc; i++){
        if(getCourse(argv[i]) == winningTeam){
            printf("%s \n", argv[i]);
        }
    }

    return 0;
}
