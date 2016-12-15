#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


sigset_t sig;
struct sigaction sigact_int;
struct sigaction sigact_quit;

int main () {
	
	sigemptyset (&sig);
	sigaddset (&sig, SIGINT);
	sigaddset (&sig, SIGQUIT);
	
	sigprocmask (SIG_SETMASK, &sig, NULL);
	
	
	sigset_t sig2;
	sigemptyset (&sig2);
	
	sigact_int.sa_mask = sig2;
	sigact_int.sa_flags = 0;
	sigact_int.sa_handler = SIG_IGN;
	sigaction (SIGINT, &sigact_int, NULL);
	
	sigact_quit.sa_mask = sig2;
	sigact_quit.sa_flags = 0;
	sigact_quit.sa_handler = SIG_IGN;
	sigaction (SIGQUIT, &sigact_quit, NULL);
	
	sleep(5);
	sigpending(&sig2);
	
	sigprocmask (SIG_UNBLOCK, &sig, NULL);
	
	int i;
	for(i=1;i<SIGRTMAX;i++){
		if(sigismember(&sig2,i)){
			printf("Signal : %d\n",i);
		}
	}
	
	return 0;

}
