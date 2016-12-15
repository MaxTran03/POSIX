#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int main (int arg, char* argv[]) {

	int i, j, p;

	for (i=0; i<3 ; i++){
		if ((p=fork ()) == 0) {
                	printf ("i=%d\n", i);
                	j=0;
                	while (j<i && ((p=fork()) == 0))
                    		j++;
                	if (p==0)
                    		printf("j=%d\n",j);
			else{
				pid_t pid_f=wait(NULL); 
				printf("jai attendu le fils : %d\n",pid_f);
			}			
			exit(j);
		}else{
			pid_t pid_f=wait(NULL); 
			printf("jai attendu le fils : %d\n",pid_f);
		}
	}
        return (0);
}

