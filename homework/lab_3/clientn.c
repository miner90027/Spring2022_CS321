/*
 * clientn.c
 * Aleks McCormick & Enigma Adams
 * 2022/03/02
 * Spring 2022 CS321
 * Client side C/C++ program to demonstrate Socket programming
 */
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080

using std::endl;
using std::cout;
using std::cin;
using std::getline;

int sock = 0, valread;


void sendM();
void receiveM();
void sigintHandlerLeave(int sig_num);
void sigintHandler(int sig_num);

int main(int argc, char const *argv[])
{

	signal(SIGINT, sigintHandler);


    struct sockaddr_in serv_addr;
   // std::string message;

    pthread_t send, receive;
   
    //char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

	// MAKE STDIN NON-BLOCKING
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	// MAKE THE SOCKET NON-BLOCKING
	flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);



	std::cout << "Connected to chat server. Type 'BYE' to end the connection." << std::endl;

    do{
    sendM();
    receiveM();
   
    //sleep(1);
    }while(true);
   
    return 0;
}

void sendM(){
	std::string message;

    std::cin.clear();
    std::cin.sync();
    // Receive message from user
   	if(getline(cin,message)){ // to get whole line including spaces
       
        if(!message.compare("BYE")){
		cout << "----You have left the conversation.----" << endl;
        // send message to server
        send(sock , message.c_str() , message.size() , 0 );
        // then exit
        exit(0);
        }

    	// send message to server
        //printf("sending message...\n");
        //sleep(1);
   		send(sock , message.c_str() , message.size() , 0 );
    }
}

void receiveM(){
	char buffer[1024] = {0};

   	//receive message from server
    if(valread = read( sock , buffer, 1024) > 0){
    	cout << "Received: " << buffer << endl;
    }

}


void sigintHandler(int sig_num){
signal(SIGINT, sigintHandler);
printf("cannot be terminated with Ctrl C\n");
//exit(0);
return;
}


void sigintHandlerLeave(int sig_num){
signal(SIGINT, sigintHandler);
printf("EXITING!\n");
exit(0);
return;
}


















