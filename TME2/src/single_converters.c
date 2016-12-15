#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <converters.h>
#include <string.h>

int main(int argc, char **argv){
	int k;
	printf("Conversion for: %s %s\n", argv[1], argv[2]); 
	
	for(k=0;k<NB_CONVERTERS;k++){
		if(fork()==0){
			printf("%s  %.3f\n",
			determine_currency(k),convert(argv[1],determine_currency(k),atof(argv[2])));
				exit(0);
		}
	}
	
	int m;
	for(m=0;m<NB_CONVERTERS;m++)
		wait(NULL);
		
	printf("End of Conversion\n"); 	
}
	
