#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>


struct stat st;


int main (int argc, char* argv []) {
	stat(argv[2],&st);
	
	int fich = open(argv[2],O_RDWR);

	if(argc <= 2 || argc > 4){
		printf("Erreur d'argument, il faut 2 ou 3 arguments!\n");
		return EXIT_FAILURE;
	}
	
	if(!S_ISREG(st.st_mode)){
		printf("Erreur: L'argument %s n'est pas un fichier régulier!\n", argv[2]);
		return EXIT_FAILURE;
	}
	
	if(access(argv[2],R_OK)){
		printf("Erreur: Pas le droit nécessaire pour la lecture!\n");
		return EXIT_FAILURE;
	}
	
	if (fich ==-1){
		printf("Erreur: mauvais nom!\n");
		return EXIT_FAILURE;
	}
	
	char c = argv[1][0];
	
	if(c== 'e' || c== 'E'){
		unlink(argv[2]);
		return EXIT_SUCCESS;
	}
	if(c== 'r' || c== 'R'){
		if(argc!=4){
			printf("Indiquez le nouveau nom en troisième paramètre!\n");
			return EXIT_FAILURE;
		}else{
			rename(argv[2],argv[3]);
			return EXIT_SUCCESS;
		}
	}
	if(c=='c'||c=='C'){
		if(argc!=4){
			printf("Indiquez les nouveaux droits en troisième paramètre!\n");
			return EXIT_FAILURE;
		}else{
			printf("%s\n",argv[2]);
			char c2 = argv[3][0];
			if(c2=='r' || c2=='R'){
				fchmod(fich,S_IRWXU|S_IRGRP|S_IROTH);
				return EXIT_SUCCESS;
			}
			if(c2=='w'||c2=='W'){
				fchmod(fich,S_IRWXU|S_IRWXG|S_IRWXO);
				return EXIT_SUCCESS;
			}
			
			printf("Erreur : indiquez r ou w en 3ème paramètre!\n");
			return EXIT_FAILURE;
		}
	}
	
	printf("Erreur : indiquez e, r ou c en 1er paramètre!\n");
	return EXIT_SUCCESS;
	
}
