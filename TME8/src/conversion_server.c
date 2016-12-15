 /**** conversion_server.c ****/

#define _POSIX_SOURCE 1
#include "converters.h"
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

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








static jmp_buf env;

void f(){
	longjmp(env,0);
}

int main(int argc, char **argv){
	
	int fd_read;
	int fd_write;
	
	conversion_message req;
	results_array rep;
	
	sigset_t sig;
	struct sigaction sigact;
	struct sigaction old1;
	
	sigfillset (&sig);
	sigdelset(&sig,SIGINT);
	sigdelset(&sig,SIGQUIT);
	
	sigact.sa_mask = sig;
	sigact.sa_flags = 0;
	sigact.sa_handler = f;
	sigaction (SIGINT, &sigact, &old1);
	sigaction (SIGQUIT, &sigact, &old1);
	
	
	
	
	while(1){
		if(setjmp(env)!=0){
			break;
		}
		
		if(mkfifo(argv[2],S_IRUSR|S_IWUSR)==-1 || mkfifo(argv[1],S_IRUSR|S_IWUSR)==-1){
			perror("mkfifo");
			return EXIT_FAILURE;
		}

		if((fd_read=open(argv[1], O_RDONLY|O_CREAT|O_TRUNC))==-1){
			perror("open");
			return EXIT_FAILURE;
		}
		
		if(read(fd_read, &req, sizeof(conversion_message)) == -1){
			perror("read");
			return EXIT_FAILURE;
		}
		
		int i;
		for(i = 0; i < NB_CONVERTERS; i++)
			handle_conversion_request(req, &rep[i], i);

		if((fd_write=open(argv[2], O_WRONLY))==-1){
			perror("open");
			return EXIT_FAILURE;
		}

		if(write(fd_write, rep, sizeof(results_array)) == -1){
			perror("write");
			return EXIT_FAILURE;
		}
	
		close(fd_write);
		close(fd_read);
		unlink(argv[1]);
		unlink(argv[2]);
	}
	
	sigaction (SIGINT, &old1, NULL);
	sigaction (SIGQUIT, &old1, NULL);
	
	close(fd_write);
	close(fd_read);
	unlink(argv[1]);
	unlink(argv[2]);
	printf("\nLes tubes nommés ont bien été fermés et détruits\n");
	
	return EXIT_SUCCESS;
	
}
