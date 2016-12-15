#define _OPEN_SOURCE 700
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "../include/converters.h"

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



struct segment{
	conversion_message req;
	results_array result;
};


int main(int argc, char **argv){
	int fd;
	
	struct segment *seg;

	sem_t *mutex;
	mutex= sem_open("mutex",O_RDWR,S_IRWXU,0);
	
	sem_t *mutex2;
	mutex2= sem_open("mutex2",O_RDWR,S_IRWXU,0);

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

	seg->req.amount = atof(argv[3]);
	strcpy(seg->req.currency, argv[2]);
	
	sem_post(mutex); // Autorise le serveur à modifier le segment partagé
	sem_wait(mutex2); // Puis attend que le serveur finisse de convertir
	
	printf("P%d>", seg->result[0].pid_sender);
	display_results(seg->req, seg->result);
	
	sem_close(mutex2);
	sem_close(mutex);
	
	return 0;
	
}
