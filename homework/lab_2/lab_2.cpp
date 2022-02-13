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
#include <sys/wait.h>

void sig_handler(int);

int main (void){
	int pid = fork();
	int status;
	
	if(pid == 0)// Child Process
	{
		do{
		printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
		// send signal to parent
		kill(getppid(), SIGUSR1);
		
		//signal(SIGUSR1, sig_handler);
		// check parent status
		waitpid(getppid(), &status, 0);
		//sleep(1);
		}while(WIFEXITED(status));// loop while parent is alive.
		exit(0);// exit when parent exits
	}


	while(1){ // Parent Process
		//wait(NULL);
		signal(SIGUSR1, sig_handler);
		signal(SIGUSR2, sig_handler);
		wait(NULL);
	}
	//kill(pid, SIGINT);
}


void sig_handler(int signum){
	if(signum == SIGUSR1){
		printf("Received SIGUSR1: %d\n", getpid());
	}

	if(signum == SIGUSR2){
		printf("Received SIGUSR2: %d\n", getpid());
	}
/*	
	if(signum == SIGINT){
		if(getpid() == 0)
			exit(0);
		else
			kill(getpid(), SIGINT);
	}*/
}
