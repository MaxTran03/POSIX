#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>


int main(int argc, char **argv) {

	struct timeval tv;
	
	printf("Resultat de la commande 'date'\n");
	
	execlp("date", "date", "+%s", NULL);
	
	printf("Comparaison avec la fonction 'gettimeofday'\n");
	
	gettimeofday(&tv, NULL);
	printf("%d\n", (int)tv.tv_sec);
	
	return 0;
}
