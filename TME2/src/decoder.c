/**** decoder.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <decoder.h>


#define _XOPEN_SOURCE 700


int magicsq[3][3] = {{4, -1, 8}, {-1, -1, -1}, {2, -1, 6}};


int check() {
	int i, j, sums[8];
	for (i = 0; i < 8; i++) 
		sums[i] = 0;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			sums[i] += magicsq[i][j];
			sums[i+3] += magicsq[j][i];
			if (i == j) sums[6] += magicsq[i][j];
			if ((i+j) == 2) sums[7] += magicsq[i][j];
		}
	}
	int result = 1;
	i = 1;
	while ((i < 8) && (result == 1)) {
		if (sums[0] != sums[i])
			result = 0;
		i++;
	}
	return result;
}


void display() {
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("  %d", magicsq[i][j]);
		printf("\n");
	}
	printf("\n");
}


int solve(int x) {
	magicsq[0][1]=x;
	magicsq[1][0]=x+6;
	magicsq[1][1]=8-x;
	magicsq[1][2]=x-2;
	magicsq[2][1]=x+4;
	
	return check();	
}

int main(int argc, char **argv)
{	
	int i;
	int cpt=0;
	for(i=0;i<10;i++){
		if(fork()==0){
			printf("je suis le processus n°%d de pid %d\n",i,getpid());
			if(solve(i)){
				display();
				exit(2);
			}
			exit(1);
		}
		
	}
	int j=0;
	while(j<10){
		int status;
		pid_t pid_ff=wait(&status);
		printf("je suis le fils n°%d de pid %d\n",j,pid_ff);
		if(WEXITSTATUS(status)==2){
			cpt++;
		}
		j++;
	}
	printf("Le nombre de carré magique possible est de %d\n",cpt);
}

