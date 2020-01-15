/// pt-crusers.c creates threads that each run a racer instance
/// @author: Mike Cao mc2522@rit.edu
/// @date: 4/23/2019

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "display.h"
#include "racer.h"

/// main function
/// @param argc number of command line arguments
/// @param argv array of command line arguments
int main(int argc, char * argv[]) {	
	// check if there is less than 3 arguments...
	if(argc < 3) {
		// print the usage statement
		fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] name1 "
			"name2 [name3...]\n");
		return EXIT_FAILURE;
	}
	// true/false value for whether or not the first argument is a
        // max speed delay or if it's a number
        int numerical = 1;
	// maximum delay between steps in animation
	int wTime = DEFAULT_WAIT;
	// starting index of the first racer
	int startingRacerIndex = 1;
	// seeding rand with time(NULL)
	srand(time(NULL));
	// check each character in the argument
	for(size_t i = 0; i < strlen(argv[1]); i++) {
		// number of decimal points in the first argument
		int decimal = 0;
		if(argv[1][i] == '.') {
			// increment decmal and if greater than one, not 
			// a valid number
			numerical = (decimal++ > 1) ? 0 : 1;
		}
		// check the ASCII and see if it's not a number or a decimal
		numerical = ((argv[1][i] < '0' || argv[1][i] > '9') 
				&& argv[1][i] != '.') ? 0 : 1;
	}
	// if the first argument is not a number, 
	// perform the following operations
	if(!numerical) {
		for(int i = startingRacerIndex; i < argc; i++) {
			if(strlen(argv[i]) > MAX_NAME_LEN) {
				fprintf(stderr, "Error: racer names must not "
						"exceed length 6.\n");
				return EXIT_FAILURE;
			}
		}

	// if the first argument is a number, perform the following operations
	} else {
		// check if argc is less than 4 because 4 is the minimum
		// number of arguements (file name, max-speed-delay, n1, n2)
		if(argc < 4) {
			fprintf(stderr, "Usage: pt-cruisers [max-speed-delay] "
					"name1 name2 [name3...]\n");
			return EXIT_FAILURE;
		} else {
			// set the starting index to 2 since that's the index
			// of the first racer
			startingRacerIndex++;
			// check every single name argument
			for(int i = startingRacerIndex; i < argc; i++) {
				// check if the names are too long
				if(strlen(argv[i]) > MAX_NAME_LEN) {
					fprintf(stderr, "Error: racer names "
							"must not exceed "
							"length 6.\n");
					return EXIT_FAILURE;
				}
			}
		}
		// convert the first argument to an int
		int conversion = (int)strtol(argv[1], NULL, 10);
		// check if the int is greater than 0, if true then set that
		// to the maximum delay time, else use DEFAULT_WAIT
		wTime = (conversion > 0) ? conversion : DEFAULT_WAIT;	
	}
	// total number of racers
	int numRacers = argc - startingRacerIndex;
	// array of threads for each racer
	pthread_t threads[numRacers];
	// array of racers
	Racer* rcr[numRacers];
	//Racer *rcr[numRacers];
	// row of the racers
	int row = 0;
	// clear the terminal
	clear();
	// fill the array with racers
	for(int i = 0; i < numRacers; i++) 
		rcr[i] = makeRacer(argv[i + startingRacerIndex], ++row);
	initRacers(wTime);
	// start the threads
	for(int i = 0; i < numRacers; i++) {
		if(pthread_create(&threads[i], NULL, run, (void*)rcr[i])) {
			perror("pthread_create");
			return EXIT_FAILURE;
		}
	}
	// join the threads
	for(int i = 0; i < numRacers; i++) {
		if(pthread_join(threads[i], NULL)) {
			perror("pthread_join");
			return EXIT_FAILURE;
		}
	}
	// destroy the racers (free them)
	for(int i = 0; i < numRacers; i++) 
		destroyRacer(rcr[i]);
	// go to the next free line and reprompt there
	set_cur_pos(numRacers + 1, 1);
	return EXIT_SUCCESS;
}
