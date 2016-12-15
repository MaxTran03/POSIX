#define _POSIX_SOURCE 1

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void calcul_1 ( ) {
	int i;
	for (i = 0; i < 1E8; i++);
}

void calcul_2 () {
	int i;
	for (i = 0; i < 1E8; i++);
}

int cont = 0;
void hand_sig(int sig){
	if((sig == SIGUSR1) || (sig == SIGUSR2))
		cont++;
}

int main(int argc, char * argv[]){
	int i = 0;
	pid_t pid_fils[2];
	struct sigaction action;
	sigset_t mask;
	
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	action.sa_handler = hand_sig;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigaddset(&mask, SIGUSR2);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	sigaction(SIGUSR1, &action, NULL);
	sigaction(SIGUSR2, &action, NULL);
	
	while((i<2) && ((pid_fils[i] = fork()) != 0))
		i++;
	
	calcul_1();
	
	sigdelset(&mask, SIGUSR1);
	sigdelset(&mask, SIGUSR2);
	
	switch(i){
		case 0:
			kill(getppid(), SIGUSR1);
			sigsuspend(&mask);
			break;
		case 1:
			kill(getppid(), SIGUSR2);
			sigsuspend(&mask);
			break;
		case 2:
			while(cont<2)
				sigsuspend(&mask);
			kill(pid_fils[0], SIGUSR1);
			kill(pid_fils[1], SIGUSR1);
			printf("Compteur : %d\n", cont);
	}
	
	printf("Début du processus %d pour calcul_2\n",i);
	calcul_2();
	printf("fin processus %d\n", i);

	return 0;
}
