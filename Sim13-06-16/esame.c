#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void usage(char* programName);

void p0_alarm(int signo);

void p0_sigusr1(int signo);

void p0_sigusr2(int signo);

int pp[2];
int pid0, pid1, pid2;
int fout;

//NEED TO CHECK
int main(int argc,char* argv[]){

	if(argc != 5){
        perror("Numero di argomenti errato...\n");
				usage(argv[0]);
        exit(EXIT_FAILURE);
	}
	if(argv[1][0] != '/'){
		perror("Fin deve essere path assoluto...\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if(argv[2][0] != '/'){
		perror("Fin deve essere path assoluto...\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if(argv[3][0] != 'P' && argv[3][0] != 'D'){
		perror("C deve essere P o D...\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if(atoi(argv[4]) < 0){
		perror("T deve essere maggiore di zero...\n");
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	pid0 = getpid();

	if(pipe(pp)<0){
			fprintf(stderr,"Errore nella creazione della pipe tra P2 e P1\n");
			exit(EXIT_FAILURE);
	}

	pid1=fork();
	if(pid1<0){perror("Errore in fase di generazione di fork...\n"); exit(EXIT_FAILURE);}
	if(pid1 == 0){				//Dentro a P1
		close(pp[1]);
		int lines_num;
		char readed[100];
		read(pp[0], &readed, sizeof(char)*100);
		lines_num = atoi(readed);
		printf("P1: Ho letto linee da piepe: %d ...\n", lines_num);
		if(lines_num%2 == 0  && argv[3][0] == 'P'){
			printf("P1: Mando P a P0...\n");
			kill(pid0, SIGUSR1);
		}else if(lines_num%2 == 1  && argv[3][0] == 'D'){
			printf("P1: Mando D a P0...\n");
			kill(pid0, SIGUSR2);
		}
		close(pp[0]);
		printf("P1: Ora sono alla fine...\n");
		pause();
		//exit(EXIT_SUCCESS);
	}

	pid2 = fork();
	if(pid2<0){perror("Errore in fase di generazione di fork...\n"); exit(EXIT_FAILURE);}
	if(pid2 == 0){				//Dentro a P2
		//printf("P2: Vivo...\n");
		//Sosituisco output con input di pipe
		close(pp[0]);
		close(1);
		dup(pp[1]);
		close(pp[1]);
		//Sostituisco input con file da leggere
		int fin = open(argv[1], O_RDONLY);
		if(fin<0){
			perror("Problena in fase di aprtura del file, ciao...\n");
			exit(EXIT_FAILURE);
		}
		close(0);
		dup(fin);
		close(fin);
		execlp("wc", "wc", "-l", (char*) NULL);
		perror("Qualcosa non è andata durante il wc...\n");
		exit(EXIT_FAILURE);
	}
	close(pp[0]);
	close(pp[1]);
	signal(SIGALRM, p0_alarm);
	signal(SIGUSR1, p0_sigusr1);
	signal(SIGUSR2, p0_sigusr2);
	alarm(atoi(argv[4]));

	int fout = open(argv[2], O_WRONLY|O_CREAT, 0777);
	if(fout<0){
		perror("Errore in fase di apertura del file output...\n");
		exit(EXIT_FAILURE);
	}

	printf("P0: Ora sono alla fine...\n");
	while(1){
		pause();
	}

}

void usage(char* programName){
	fprintf(stderr, "Chiama il programma come %s Fin Fout C T\n", programName);
	return;
}

void p0_alarm(int signo){
	kill(pid1, SIGKILL);
	printf("P0: Terminato tutto per timeout...\n");
	exit(EXIT_SUCCESS);
}

void p0_sigusr1(int signo){
	//Nel caso ci siano pari righe
	char string_to_out[] = "Fin contiene un numero pari di righe!\n";
	write(fout, &string_to_out, sizeof(char)*strlen(string_to_out));
	close(fout);
	kill(pid1, SIGKILL);
	exit(EXIT_SUCCESS);
}

void p0_sigusr2(int signo){
	//Nel caso ci siano pari righe
	char string_to_out[] = "Fin contiene un numero dispari di righe!\n";
	write(fout, &string_to_out, sizeof(char)*strlen(string_to_out));
	close(fout);
	kill(pid1, SIGKILL);
	exit(EXIT_SUCCESS);
}
