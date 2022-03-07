/*
 * client1.cpp
 * Aleks McCormick & Enigma Adams
 * 2022/03/02
 * Spring 2022 CS321
 * Client side C/C++ program to demonstrate Socket programming
 */
#include <stdio.h>
#include <sys/socket.h>
//#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080

using std::endl;
using std::cout;
using std::cin;
using std::getline;

/*
void* sendM(void *ptr);
void* receiveM(void *ptr);
*/
int main(int argc, char const *argv[])
{
    struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
	int sock = 0, valread;

    //pthread_t send, receive;
    
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

    //set stdin to non-blocking - Idea from RootCellar
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Make the socket non-blocking
    flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	std::cout << "Connected to chat server. Type 'Ctrl C' to end the connection." << std::endl;

	while(true){
		//reset error
		errno = 0;
		//read from server
		valread = read(sock, buffer, 1024);

		if(errno == EAGAIN){
			//No data... continue
		}
		else if(valread == 0){
			cout << "----Disconnected----" << endl;
			close(sock);
			sock = -1;
		}
		else if(errno != 0){
			perror("Read Error");
			exit(EXIT_FAILURE);
		}
		else{
			//print received data
			buffer[valread] = 0;
			cout << "Recieved: " << buffer << endl;
		}

		//receive user input & send to server
		errno = 0;
		valread = read(STDIN_FILENO, buffer, 1024);

		if(errno == EAGAIN){

		}
		else{
			buffer[valread] = 0;
			send(sock, buffer, strlen(buffer), 0);
		}
	}

}
/*
    do{
	//	pthread_create(&receive, NULL, receiveM, NULL);

    	pthread_create(&send, NULL, sendM, NULL);
    	
    	if(fork() == 0){
	//    	pthread_create(&send, NULL, sendM, NULL);
	//		pthread_join(send, NULL);

			pthread_create(&receive, NULL, receiveM, NULL);
			pthread_join(receive, NULL);
			
		}

		//wait(NULL);

		pthread_join(send, NULL);
	//	pthread_join(receive, NULL);

    }while(true);
    return 0;
}

void* sendM(void *ptr){
		std::string message;
	
    	std::cin.clear();
    	std::cin.sync();
    	// Receive message from user
	    getline(cin,message); // to get whole line including spaces
	    
		if(!message.compare("BYE")){
			//message = "Other user has left conversation.";
			cout << "----You have left the conversation----" << endl;
			send(sock, message.c_str(), message.size(), 0);
			kill(getppid(), SIGINT);
			return ptr;
		}

    	// send message to server
	    send(sock , message.c_str() , message.size() , 0 );
}

void* receiveM(void *ptr){
		char buffer[1024] = {0};

	    //receive message from server
	    valread = read( sock , buffer, 1024);

    	cout << "Received: " << buffer << endl;
    /*	if(strcmp(buffer, "BYE")){
			cout << "----You have left the conversation----" << endl;
			send(sock, buffer, strlen(buffer), 0);

			kill(getppid(), SIGINT);
		}
		
}
*/
