#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

DIR *pt_Dir;
struct dirent* dirEnt;

int main (int argc, char* argv []) {
	if(argc==1){
		if ( (pt_Dir = opendir ("." )) == NULL) {
			perror ("opendir");
			return EXIT_FAILURE;
		}	
		while ((dirEnt= readdir (pt_Dir)) !=NULL)
			printf ("%s\n", dirEnt->d_name);
		closedir (pt_Dir);
		return EXIT_SUCCESS;
	}
	
	if ( ( pt_Dir = opendir (argv[1]) ) == NULL) {
		mkdir(argv[1], S_IRWXU | S_IRGRP | S_IROTH);
		return EXIT_SUCCESS;
	}	
	while ((dirEnt= readdir (pt_Dir)) !=NULL)
		printf ("%s\n", dirEnt->d_name);
	closedir (pt_Dir);
	return EXIT_SUCCESS;
}
