#define _OPEN_SOURCE 700
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "../include/converters.h"
#include <sys/wait.h>


/* Converts from any to EUR */
double convertfrom(char* input_currency, double input_amount) {
	if (strcmp(input_currency, "GBP") == 0)
		return (input_amount / GBP);
	else if (strcmp(input_currency, "USD") == 0)
		return (input_amount / USD);
	else if (strcmp(input_currency, "JPY") == 0)
		return (input_amount / JPY);
	else if (strcmp(input_currency, "CNY") == 0)
		return (input_amount / CNY);
	else if (strcmp(input_currency, "EUR") == 0)
		return input_amount;
	else {
		perror("Unknown currency");
		exit(1);
	}
}


/* Converts from EUR to any */
double convertto(char* target_currency, double input_amount) {
	if (strcmp(target_currency, "GBP") == 0)
		return (input_amount * GBP);
	else if (strcmp(target_currency, "USD") == 0)
		return (input_amount * USD);
	else if (strcmp(target_currency, "JPY") == 0)
		return (input_amount * JPY);
	else if (strcmp(target_currency, "CNY") == 0)
		return (input_amount * CNY);
	else if (strcmp(target_currency, "EUR") == 0)
		return input_amount;
	else {
		perror("Unknown currency");
		exit(1);
	}
}


/* Converts from any to any */
double convert(char* input_currency, char* target_currency, double input_amount) {
	return convertto(target_currency, convertfrom(input_currency, input_amount));
}


/* Determines a currency string identifier, given its integer identifier */
char* determine_currency(int curr_nb) {
	switch(curr_nb) {
		case EUR_CONV: return "EUR";
		case GBP_CONV: return "GBP";
		case USD_CONV: return "USD";
		case JPY_CONV: return "JPY";
		case CNY_CONV: return "CNY";
		default: perror("Unknown currency"); exit(1);
	}
}


/* Processes a conversion request */
void handle_conversion_request(conversion_message request, conversion_message *result, int curr_nb) {
	result->pid_sender = getpid();
	strcpy(result->currency, determine_currency(curr_nb));
	result->amount = convert(request.currency, result->currency, request.amount);
}
	

/* Displays the contents of a conversion results message */
void display_results(conversion_message request, results_array results) {
	int i;
	printf("Conversion for: %s %.3f\n", request.currency, request.amount);
	for(i = 0; i < NB_CONVERTERS; i++)
		printf("\t %s %.3f\n", results[i].currency, results[i].amount);
}







/*-------------------*/




static jmp_buf env;

void f(){
	longjmp(env,0);
}

struct segment{
	conversion_message req;
	results_array result;
};

int main(int argc, char **argv){
	int fd,fd2;

	struct segment *seg;
	conversion_message *rep;

	if((fd=shm_open(argv[1],O_RDWR|O_CREAT,S_IRWXU))==-1){
		perror("shm_open");
		return EXIT_FAILURE;
	}

	if(ftruncate(fd,sizeof(struct segment))==-1){
		perror("ftruncate");
		return EXIT_FAILURE;
	}

	if((seg=mmap(NULL,sizeof(struct segment),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
		perror("mmap");
		return EXIT_FAILURE;
	}

	if((fd2=shm_open("temp",O_RDWR|O_CREAT,S_IRWXU))==-1){
		perror("shm_open");
		return EXIT_FAILURE;
	}
	if(ftruncate(fd2,sizeof(conversion_message))==-1){
		perror("ftruncate");
		return EXIT_FAILURE;
	}
	
	if((rep=mmap(NULL,sizeof(conversion_message),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0))==MAP_FAILED){
		perror("mmap");
		return EXIT_FAILURE;
	}
	

	sigset_t sig;
	struct sigaction sigact;
	struct sigaction old;
	
	sigfillset (&sig);
	sigdelset(&sig,SIGINT);
	sigdelset(&sig,SIGQUIT);
	
	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	sigact.sa_handler = f;
	sigaction (SIGINT, &sigact, &old);
	sigaction (SIGQUIT, &sigact, &old);
	
	sem_t *mutex;
	mutex= sem_open("mutex",O_CREAT|O_RDWR,S_IRWXU,0);
	
	sem_t *mutex2;
	mutex2= sem_open("mutex2",O_CREAT|O_RDWR,S_IRWXU,0);
	
	printf("\nPour quitter proprement, faire 'Ctrl + c' ou 'Ctrl + ]'\n");
	
	while(1){
	
		if(setjmp(env)!=0){
			break;
		}
		
		
		int i;
		sem_wait(mutex);
		
		for(i = 0; i < NB_CONVERTERS; i++){	
			if(fork() == 0){
				handle_conversion_request(seg->req, &rep[i], i);	
				exit(i);
			}
		}
		
		for(i = 0; i < NB_CONVERTERS; i++)
			wait(NULL);
			
		for(i = 0; i < NB_CONVERTERS; i++)
			seg->result[i]=rep[i];
		
		sem_post(mutex2);
		
	}
	
	sigaction (SIGINT, &old, NULL);
	sigaction (SIGQUIT, &old, NULL);
	
	munmap(seg,sizeof(struct segment));
	shm_unlink(argv[1]);
	
	munmap(rep,sizeof(conversion_message));
	shm_unlink("temp");
	
	sem_close(mutex2);
	sem_close(mutex);
	sem_unlink("mutex");
	sem_unlink("mutex2");
	
	printf("\nLe programme s'est terminé proprement\n");
	
	return EXIT_SUCCESS;
}
