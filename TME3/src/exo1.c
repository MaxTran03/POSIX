#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


void n_fils(int n){

	int i;
	pid_t * tab_fils;
	tab_fils = calloc(n,sizeof(pid_t));
	pid_t * tab_pere;
	tab_pere = calloc(n,sizeof(pid_t));
	
	
	for(i=0;i<n;i++){
		
		tab_fils[i]=getpid();
		tab_pere[i]=getppid(); /* pour ensuite afficher le père est bien vérifier que c'est une chaine de fils*/
		
		if(fork()!=0){
			return;
		}
	}
	
	int j;
	
	for(j=0;j<n;j++){
		printf("je suis pid %d et mon père est %d\n",tab_fils[j], tab_pere[j]);
	}
	
	free(tab_fils);
	free(tab_pere);
	
}
		
int main(int argc, char** argv) {
	
	n_fils(5);
	
	return EXIT_SUCCESS;
	
}
