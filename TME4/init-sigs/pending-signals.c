#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


sigset_t sig;

int main () {
	
	sigemptyset (&sig);
	sigaddset (&sig, SIGINT);
	sigaddset (&sig, SIGQUIT);
	sigprocmask (SIG_SETMASK, &sig, NULL);
	
	sigset_t sig2;
	sigemptyset (&sig2);
	sleep(5);
	sigpending(&sig2);
	
	sigprocmask (SIG_UNBLOCK, &sig, NULL);
	
	int i;
	for(i=1;i<=31;i++){
		if(sigismember(&sig2,i)){
			printf("Signal : %d\n",i);
		}
	}
	
	return 0;

}
