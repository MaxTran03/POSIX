#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


void rediriger_stdout(char *file){
	int f1 = creat(file, S_IRWXU|S_IRGRP|S_IROTH);
	dup2(f1,	STDOUT_FILENO);
}

void restaurer_stdout(){
	int var = open("/dev/tty",O_RDWR);
	dup2(var,STDOUT_FILENO);
}

int main(int argc, char* argv []) {
	printf ("avant la redirection \n") ;
	rediriger_stdout ("fichier.out") ;
	printf ("après la redirection \n") ;
	restaurer_stdout ( ) ;
	printf ("après avoir restauré stdout \n") ;
	return EXIT_SUCCESS ;
}
