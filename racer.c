/// racer.c - a thread controlling a small figure 
/// @author Mike Cao mc2522@rit.edu
/// @date: 4/23/19

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "racer.h"
#include "display.h"

// mytex for thread synchronization
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
// wait time for pause between steps in animation
long wTime;

/// initRacers - Do setup work for all racers at the start of the program.
/// @param milliseconds length of pause between steps in animation
///
void initRacers(long milliseconds) {	
	// maximum wait time
	wTime = milliseconds;
}

/// makeRacer - Create a new racer.
///
/// @param name the string name to show on the display for this racer
/// @param position the row in which to race
/// @return Racer pointer a dynamically allocated Racer object
/// @pre strlen( name ) <= MAX_NAME_LEN, for display reasons.
///
Racer * makeRacer(char * name, int position) {
	// allocate necessary space for the struct and the string
	Racer * rcr = (Racer*)malloc(sizeof(Racer));
	rcr->graphic = (char*)malloc(MAX_CAR_LEN + 1);
	// create the graphic
	strncpy(rcr->graphic, "~O=-------o>", MAX_CAR_LEN + 1);
	// replace the hyphens or dash with the characters of the name
	// first check the length of the name to see if valid
	if(strlen(name) <= 6) {
		memcpy(rcr->graphic + 3, name, strlen(name));
	} else {
		// print error message and exit
		fprintf(stderr, "Name length is too long (>6)!\n");
		exit(EXIT_FAILURE);
	}
	// initial position
	rcr->row = position;
	// distance from the start line
	rcr->dist = 0;
	// return the racer
	return rcr;
}

/// destroyRacer - Destroy all dynamically allocated storage for a racer.
///
/// @param racer the object to be de-allocated
///
void destroyRacer(Racer * racer) {
	// free the string which is the visual car
	free(racer->graphic);
	// free the struct itself
	free(racer);
}

/// Run one racer in the race.
/// Initialize the display of the racer*:
///   The racer starts at the start position, column 0.
///   The racer's graphic (a string) is displayed.
///
/// These actions happen repetitively, until its position is at FINISH_LINE:
///
///  Calculate a random waiting period between 0 and
///    the initial delay value given to initRacers at the start of the race.
///  Sleep for that length of time.
///  If sleep value is less than or equal 3, racer gets flat and can't finish.
///    A flat tire is displayed by 'X' in the second character of the graphic.
///    Display the car with the flat tire and stop further racing.
///  Change the display position of this racer by +1 column this way:
///    Erase the racer's car and name from the display.
///    Update the racer's dist field by +1.
///    Display the racer's graphic (car and name) at the new position.
///
/// The intention is to execute this function many times simultaneously,
/// each in its own thread.
///
/// Note: Be sure to keep the update of the display by one racer "atomic".
///
/// @pre racer cannot be NULL.
/// @param racer Racer object declared as void* for pthread compatibility
/// @return void pointer to status. A NULL represents success.
///
void * run(void * racer) {
	// cast to struct Racer type to access members
	Racer * rcr = (Racer*)racer;
	// lock so that only one thread can access this part of the code
	// so that each car could be properly placed
	pthread_mutex_lock(&mut);
	// go to the specific row of the racer...
	set_cur_pos(rcr->row, 1);
	// and print the graphical representation of the vehicle there
	printf("%s", rcr->graphic);
	fflush(stdout);
	// unlock so other threads can have access
	pthread_mutex_unlock(&mut);
	// perform the race until someone reaches the finish line
	while(1) {
		// randomly generate a wTime between 0 and wTime
		long waitTime = (rand() % wTime);
		// sleep for waitTime/1000 b/c the sleep function takes the
		// argument and sleeps for #waitTime seconds and the original
		// time given is in milliseconds
		usleep(waitTime * 1000);
		// lock so only one thread can access
		pthread_mutex_lock(&mut);
		// if wait time is greater than 3
		if(waitTime <= 3) {
			rcr->graphic[1] = 'X';
		} else {
			// set cursor position and increment distance from 
			// starting line
			set_cur_pos(rcr->row, ++rcr->dist);
			// put a space to represent the car proceeding
			put(' ');
		}
		// set cursor position to the next block to print the car
		set_cur_pos(rcr->row, rcr->dist + 1);
		// print the car
		//printf("%s\n", rcr->graphic);
		puts(rcr->graphic);
		fflush(stdout);
		// unlock since this thread is done for the moment
		pthread_mutex_unlock(&mut);
		if(rcr->dist >= FINISH_LINE || waitTime <= 3) {
			// racer finished, so end the infinite loop to get
			// to return statement
			break;
		}
	}
	// if this is reached, success
	return NULL;
}
