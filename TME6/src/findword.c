#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv []) {

	int fic1=open(argv[1],O_RDONLY);
	
	char t1;

	int cpt_occ = 0;
	int cpt_mot = 0;
	int size;
	
	while(1){
		
		int i=0;
		char tab[100];
		
		while((size=read(fic1,&t1,sizeof(char)))){
			if(t1==' '||t1=='\n'||t1==EOF){
				break;
			}
			tab[i] = t1;
			i++;
		}
		if(i!=0)
			cpt_mot++;
		
		char stt[i+1];
		strncpy(stt,tab,i);
		stt[i+1]='\0';
		
		printf("%c\n",stt[0]);
		
		if(strcmp(argv[2],stt)==0){
			cpt_occ++;
			printf("Le mot '%s' est à la position %d\n",argv[2],cpt_mot);
		}
		
		if(size==0){
			printf("Il y a %d occurrences de %s\n",cpt_occ,argv[2]);
			return EXIT_SUCCESS;
		}
	}
	
	return EXIT_SUCCESS;
}		
