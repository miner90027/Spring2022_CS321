/*
 * lab_2.cpp
 * Aleks McCormick
 * 2022/02/11
 * Spring 2022 CS321
 * Demonstrate signal handling and child process
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
using std::cout;
using std::endl;

void sig_handler(int signum);
int pid;

/******************************************/
/***            Main Program            ***/
/******************************************/
int main (void){
	// spawn a child process
	pid = fork();
	int status;

	/******************************************/
	/***            Child Process           ***/
	/******************************************/
	if(pid == 0)
	{
		do{
		// search for intrerupt signal
		signal(SIGINT, sig_handler);
		// print child & parent pid for debugging
		cout << "[son] pid " << getpid() << " from [parent] pid " << getppid() << endl;

		// send signal to parent
		kill(getppid(), SIGUSR1);
		kill(getppid(), SIGUSR2);

		// check parent status
		waitpid(getppid(), &status, 0);
		//sleep(1);
		}while(WIFEXITED(status));// loop while parent is alive.
		exit(0);// exit when parent exits
	}


	/******************************************/
	/***           Parent Process           ***/
	/******************************************/
	while(1){
		//wait(NULL);
		signal(SIGINT, sig_handler);
		signal(SIGUSR1, sig_handler);
		signal(SIGUSR2, sig_handler);
		wait(NULL);
	}
}

/******************************************/
/***       Function Definitions         ***/
/******************************************/

// sig_handler()
// handles the signals 
//  - SIGUSR1
//  - SIGUSR2
//  - SIGINT
// makes sure that child process ends when the parent process in sent
//  an interupt signal
void sig_handler(int signum){
	if(signum == SIGUSR1){
		cout << "Received SIGUSR1" << endl;
	}

	if(signum == SIGUSR2){
		cout << "Received SIGUSR2" << endl;
	}
	
	if(signum == SIGINT){
		if(pid == 0){
			cout << "\nChild terminated"<< endl;
			exit(0);
		}
		else{
			kill(pid, SIGINT);
			wait(NULL);
			cout << "\nParent terminated" << endl;
			exit(0);
		}
	}
}
