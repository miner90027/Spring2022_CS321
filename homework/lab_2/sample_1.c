/*
 * sample_1.c
 * Aleks McCormick
 * 2022/02/02
 * Spring 2022 CS321
 * A sample code provided by the instructor to demonstrate the installation
 *   of a signal handler that itterupts the built-in interupt signal
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigHandler (int);


int main()
{
	signal (SIGINT, sigHandler);
	printf("Waiting...\n");
	pause();
	return 0;
}

void sigHandler (int sigNum)
{
	printf(" received an interrupt signal through Ctrl-c. \n");
	printf("Going to sleep.\n Notice the process is not exiting immediately");

	// This is where the shutdown code would be  inserted

	sleep(1);
	printf("Out of here.\n");

	printf("Now the process will terminate wiht teh exit system call \n");
	exit(0);
}
