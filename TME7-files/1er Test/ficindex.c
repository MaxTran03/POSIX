#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
/*#include <ficindex.h>*/

int ind_File[1000000];

static int indValidite(int d, size_t nbytes){
	if(nbytes < ind_File[d]){
		errno = EINVAL;
		return 0;
	}
	return 1;
}
/*
int indopen(const char *path, int flags, int ind, mode_t mode){
	int f;
	
	if(access(path, F_OK)==0){
		f = open(path, flags, mode);
		read(f,ind_File+f,sizeof(int));
		printf("Taille index: %d\n",ind_File[f]);
	}else{
		f = open(path,O_RDWR|O_CREAT|O_TRUNC,mode);
		write(f, &ind, sizeof(int));
	}	
	return f;
}*/

int indopen(const char *path, int flags, int ind, mode_t mode){
	int fd;

	if((fd = open(path,flags,mode)) == -1){
		return -1;
	}

	if(read(fd, &ind, sizeof(int)) == 0){
		write(fd, &ind, sizeof(int)); 
	}
	
	return fd;  
}

int indclose(int fd){
	return close(fd);
}


ssize_t indread(int fd, void *buf, size_t nbytes){
	if(nbytes < ind_File[fd]){
		printf("Le nombre d'octets à lire est inférieur à la taille de l'index !\n");
		return -1;
	}
	
	if(read(fd, NULL, sizeof(int)) == -1){
		printf("Pas de int au début du fichier\n");
		return -1;
	}
	
	/*printf("Le nombre d'octets lus: %d\n",nblu);*/
	
	int x = read(fd, buf, ind_File[fd]); 
	if(x==nbytes)
		return 0;
	return x;
}

ssize_t indwrite(int fd, const void *buf, size_t nbytes){
	int index;
  off_t posActuelle = lseek(fd, 0, SEEK_CUR);
  
  if(lseek(fd, 0, SEEK_SET)==-1){
    perror("Impossible de se déplacer dans le fichier");
    return -1;
  }

  read(fd, &index, sizeof(int));
  
  /*printf("Passe dans indwrite\n);*/
  
  if( lseek(fd, posActuelle, SEEK_SET)==-1 ){
    perror("Impossible de continuer le déplacement dans le fichier");
    return -1;
  }
  
  if(nbytes < index){
    perror("Nbytes est inférieur à la taille de l'index");
    return -1;
  } else{
    
    return write(fd, buf, nbytes);  
  }}

off_t indlseek(int fd, off_t offset, int whence){}

void *indsearch(int fd, int (*cmp)(void *, void *), void *key){}

int indxchg(char *oldfic, char *newfic, unsigned int newind){}

/*
int main(){
	char t;
	int i = indopen("test",O_RDWR,sizeof(char),S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	int lecture = indread(i,&t,7);
	
	if(lecture == 0)
		printf("Arrivé en fin de fichier\n");
	else
		printf("Nombre d'octets lus: %d\n", lecture);
		
	indclose(i);
	
	return 0;
}*/
