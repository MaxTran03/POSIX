#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv) {
	
	int nb = atoi(argv[1]);
		if(nb!=0){
			nb--;
			char str[2] = "";
			sprintf(str, "%d", nb);
			printf("mon pid %d\n", getpid());
			execl("./bin/exo4","exo4",str,NULL);
			perror("execl");
		}
	return EXIT_SUCCESS;
}
