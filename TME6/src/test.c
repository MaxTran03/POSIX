#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

void renommer(char* nom, char* nomNouveau){
/* rename */
}

void effacer(char* nom){
/* unlink */
}

void changer_mod(char* nom, char mode){
	switch(mode){
		case 'r': chmod(nom, ... );
					 break;
		case 'w':
		
		default;
}

int main (int argc, char* argv []) {
	if(argc<3){
		printf("erreur");
		exit(1);
	}
	if(stat(argv[2], &stat_info) == -1){
		perror(...);
		exit(1);
	}
	if(S_ISDIR(stat_info.st_mode)){
	printf("
}
