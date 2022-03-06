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
#include <iostream>
#include <string>
#define PORT 8080

using std::endl;
using std::cout;
using std::cin;
using std::getline;
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    std::string message;
    
    char buffer[1024] = {0};
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
    	std::cin.clear();
    	std::cin.sync();
    	// Receive message from user
	    getline(cin,message); // to get whole line including spaces
		if(!message.compare("EXIT")){
			message = "Client has left conversation.";

			send(sock, message.c_str(), message.size(), 0);
			break;
		}

    	// send message to server
	    send(sock , message.c_str() , message.size() , 0 );

		//clear out buffer for next round
		for(int i = 0; i< sizeof(buffer); i++){ buffer[i] = 0;}

	    //receive message from server
	    valread = read( sock , buffer, 1024);

    	printf("%s\n",buffer );

    }while(true);
    return 0;
}
