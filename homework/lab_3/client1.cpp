/*
 * client1.cpp
 * Aleks McCormick & Enigma Adams
 * 2022/03/02
 * Spring 2022 CS321
 * Client side C/C++ program to demonstrate Socket programming
 */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <string>
#define PORT 8080

using std::endl;
using std::cout;
using std::cin;
using std::getline;

int sock = 0, valread;

void* sendM(void *ptr);
void* receiveM(void *ptr);

int main(int argc, char const *argv[])
{
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

	std::cout << "Connected to chat server. Type EXIT to end the connection." << std::endl;
	
    do{
    	pthread_create(&send, NULL, sendM, NULL);
		pthread_create(&receive, NULL, receiveM, NULL);

		pthread_join(send, NULL);
		pthread_join(receive, NULL);

    }while(true);
    return 0;
}

void* sendM(void *ptr){
		std::string message;
	
    	std::cin.clear();
    	std::cin.sync();
    	// Receive message from user
	    getline(cin,message); // to get whole line including spaces
		if(!message.compare("EXIT")){
			message = "Client has left conversation.";

			send(sock, message.c_str(), message.size(), 0);
			exit(0);
			//break;
		}

    	// send message to server
	    send(sock , message.c_str() , message.size() , 0 );
}

void* receiveM(void *ptr){
		char buffer[1024] = {0};
	
		//clear out buffer for next round
		//for(int i = 0; i< sizeof(buffer); i++){ buffer[i] = 0;}

	    //receive message from server
	    valread = read( sock , buffer, 1024);

    	printf("%s\n",buffer );
}
