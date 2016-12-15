/**** capitalize.c ****/
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <setjmp.h>
#include <ctype.h>

static jmp_buf env;

void f(int sig){
	siglongjmp(env,sig);
}

int main(){
	char *sp;
	int fd;
	sem_t smutex1 /*= mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)*/;
	sem_t smutex2 /*= mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)*/;
	struct sigaction sigact;
	
	if((fd = shm_open("monshm", O_RDWR|O_CREAT, 0600)) == -1){
		perror("sem_open");
		return EXIT_FAILURE;
	}
	
	if(ftruncate(fd, sizeof(char)) == -1){
		perror("ftruncate");
		return EXIT_FAILURE;
	}
	
	if((sp = mmap(NULL, sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap");
		return EXIT_FAILURE;
	}
	
	if(sem_init(&smutex1, 1, 0) == -1){
		perror("sem_init");	
		return EXIT_FAILURE;
	}
	
	if(sem_init(&smutex2, 1, 1) == -1){
		perror("sem_init");	
		return EXIT_FAILURE;
	}
	
	
	/* Redéfinition du signal */
	sigact.sa_flags = 0;
	sigact.sa_handler = f;
	sigaction(SIGINT, &sigact, NULL);

	
	if(sigsetjmp(env, 0) != SIGINT){
		if(fork() == 0){
			while(1){
				sem_wait(&smutex2);
				printf("Ecrivez un message:\n");
				gets(sp);
				sem_post(&smutex1);
			}
		}else{
			int i = 0;
			while(1){
				sem_wait(&smutex1);
				while(sp[i]){
					printf("%c", toupper(sp[i]));
					i++;
				}
				sem_post(&smutex2);
			}
		}
	}	
	
	/* Détacher un segment de l'espace du processus */
	if(munmap(sp, sizeof(char)) == -1){
		perror("munmap");
		return EXIT_FAILURE;
	}
	
	/* Test de l'existance du fichier et détruire le segment */
	if(access("monshm", F_OK) == 0 && shm_unlink("monshm") == -1){
		perror("shm_unlink");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
