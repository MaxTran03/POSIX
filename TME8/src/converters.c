/**** converters.c ****/

#include "converters.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _POSIX_SOURCE 1

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


int main(int argc, char **argv){
	int i;
	int tube[2];
	pid_t pid_fils;
	
	conversion_message req;
	results_array rep;
	results_array result;
	
	req.amount = atof(argv[2]);
	strcpy(req.currency, argv[1]);
	
	if(pipe(tube) == -1){
		perror("pipe");
		return EXIT_FAILURE;
	}
	
	if((pid_fils = fork()) == -1){
		perror("fork");
		return EXIT_FAILURE;
	}
	
	if(pid_fils == 0){
		for(i = 0; i < NB_CONVERTERS; i++)
			handle_conversion_request(req, &rep[i], i);
		
		if(write(tube[1], rep, sizeof(results_array)) == -1){
			perror("write");
			return EXIT_FAILURE;
		}	
	}else{
		if(read(tube[0], result, sizeof(results_array)) == -1){
			perror("read");
			return EXIT_FAILURE;
		}
		printf("P%d> Conversion for: %s %s\n", result[0].pid_sender, argv[1], argv[2]);
		display_results(req, result);
		close(tube[0]);
		close(tube[1]);
	}
	
	
	return EXIT_SUCCESS;
}

