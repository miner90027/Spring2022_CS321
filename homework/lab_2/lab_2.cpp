/*
 * lab_2.cpp
 * Aleks McCormick
 * 2022/02/11
 * Spring 2022 CS321
 * Demonstrate signal handling and child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int);

int main (void){

	for(int i = 0; i < 5; ++i){
		if(fork() == 0){
			printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
//			if(signal(SIGUSR1, sigHandler) == SIG_ERR)
//				printf("\n can't catch SIGUSR1\n");
			while(1)
				sleep(1);
		}
	}
}


void sigHandler(int signum){
	if(signum == SIGUSR1){
		printf("Received SIGUSR1\n");
	}

	if(signum == SIGUSR2){
		printf("Received SIGUSR2");
	}
}
