#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


void n_fils_it_wait(int n){

	int i=0;
	srand(time(NULL));
	while(i<n){
		int r = rand()%9 + 1;
		if((fork())==0){
			printf("je suis le fils %d de %d et dors %d secondes \n",getpid(),getppid(),r);
			sleep(r);
			printf("Fin de %d\n", getpid());
			exit(2);
		}
		i++;
	}
	pid_t pid_f = wait(NULL);
	if(pid_f!=-1)
	printf("Le processur pere a attendu le processus fils de pid : %d\n",pid_f);
}


void n_fils_rec_wait(int n){

	if(n!=0){
		if((fork())==0){
			printf("je suis le fils %d et mon pere est %d\n",getpid(), getppid());
			printf("Fin de %d\n", getpid());
			exit(2);
		}
		n_fils_rec_wait(n-1);
	}else{
		pid_t pid_f = wait(NULL);
		if(pid_f!=-1)
		printf("Le processur pere a attendu le processus fils de pid : %d\n",pid_f);
	}

}

void n_fils_it_wait2(int n){
	int i=0;
	srand(time(NULL));
	while(i<n){
		int r = rand()%9 + 1;
		if((fork())==0){

			printf("je suis le fils %d de %d et dors %d secondes \n",getpid(),getppid(),r);
			sleep(r);
			printf("Fin de %d\n", getpid());
			return;
		}
		i++;
	}
	int j=0;
	while(j<n){
		pid_t pid_f = wait(NULL);
		if(pid_f!=-1)
		printf("Le processur pere a attendu le processus fils de pid : %d\n",pid_f);
		j++;
	}
	return;
}

void n_fils_rec_wait2(int n){
	if(n!=0){
		pid_t pid_f;
		if((fork())==0){
			printf("je suis le fils %d et mon pere est %d\n",getpid(), getppid());
			printf("Fin de %d\n", getpid());
			exit(1);
		}else{
			pid_t pid_f=wait(NULL);
			printf("Le processur pere a attendu le processus fils de pid : %d\n",pid_f);
		}
		n_fils_rec_wait2(n-1);
	}else{
		printf("Le pere a attendu tout le monde\n");
		exit(2);
	}
}

void n_fils_it_wait3(int n){
	int i=0;
	srand(time(NULL));
	while(i<n){
		int r = rand()%9 + 1;
		pid_t pid_fils;
		if((pid_fils=fork())==0){
			printf("je suis le fils %d de %d et dors %d secondes \n",getpid(),getppid(),r);
			sleep(r);
			printf("Fin de %d\n", getpid());
			return;	
		}else{
			if(n==i+1){
				waitpid(pid_fils,NULL,WUNTRACED);
				printf("Le processur pere a attendu le processus fils de pid : %d\n",pid_fils);
			}
		}
		i++;
	}
}

void n_fils_rec_wait3(int n){
	if(n>1){
		if((fork())==0){
			printf("je suis le fils %d et mon pere est %d\n",getpid(), getppid());
			sleep(5);
			printf("Fin de %d\n", getpid());
			return;
		}
		n_fils_rec_wait(n-1);
	}else{
		pid_t pid_f;
		if((pid_f=fork())==0){
			printf("je suis le dernier fils %d et mon pere est %d\n",getpid(), getppid());
			sleep(7);
			printf("Fin de %d\n", getpid());
			return;
		}else{		
			waitpid(pid_f,NULL,WUNTRACED);
			printf("Le processur pere a attendu le processus fils de pid : %d\n",pid_f);
		}
	}
}


int main(int argc, char** argv) {

	n_fils_rec_wait3(4);

	return EXIT_SUCCESS;
}
