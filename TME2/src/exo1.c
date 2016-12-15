#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void n_fils_it(int n){
	int i=0;
	printf("je suis le papa %d\n", getpid());
	pid_t pid_f;
	while(i<n && (pid_f=fork())!=0){
		printf("je suis le fils de pid : %d\n", pid_f);
		i++;
	}

}

void n_fils_rec(int n){
	pid_t pid_f;
	printf("je suis le papa %d\n", getpid());
	if(n!=0 && (pid_f=fork())!=0){
		printf("je suis le fils de pid : %d\n", pid_f);
		n_fils_rec(n-1);
	}
}

int main(int argc, char** argv) {
	n_fils_rec(4);
	return EXIT_SUCCESS;
}
