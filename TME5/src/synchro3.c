#define _POSIX_SOURCE 1

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

sigset_t sig;
struct sigaction sigact;
struct sigaction sigact2;

void f(){}

void calcul_1 ( ) {
	int i;
	for (i = 0; i < 1E8; i++);
}

void calcul_2 () {
	int i;
	for (i = 0; i < 1E8; i++);
}

int main (int argc, char * argv[]) {
	int i=0;
	pid_t pid_fils[2];
	
	sigemptyset (&sig);
	sigprocmask (SIG_SETMASK, &sig, NULL);
	
	while ((i<2) && ((pid_fils[i] = fork())!=0))
		i++;		
	
	printf("Le fils %d a pour pere %d, i=%d\n",getpid(),getppid(),i);	
	
	calcul_1 ();
	if(i==0){
		printf("\nfils %d pere %d\n",getpid(),getppid());	
		sleep(1);
		kill(getppid(),SIGUSR1);
	}
	if(i==1){
		printf("fils %d pere %d\n",getpid(),getppid());	
		sleep(2);
		kill(getppid(),SIGUSR2);
	}
	if(i==2){
		sigact.sa_mask = sig;
		sigact.sa_flags = 0;
		sigact.sa_handler = f;
		sigaction (SIGUSR1, &sigact, NULL);
		sigsuspend(&sig);
		sigact2.sa_mask = sig;
		sigact2.sa_flags = 0;
		sigact2.sa_handler = f;
		sigaction (SIGUSR2, &sigact2, NULL);
		sigsuspend(&sig);
	}
	
	printf("Début du processus %d pour calcul_2\n",i);
	
	calcul_2 ();
	printf ("fin processus %d \n",i);             
	
	return EXIT_SUCCESS;   
}
