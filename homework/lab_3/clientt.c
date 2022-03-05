// trying threading 
/*
 * client1.cpp
 * Aleks McCormick & ...
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
#include <thread>
#include <string>
#include <vector> 
#define PORT 8080

using std:: endl;
using std:: cout;
using std::cin;
using std::getline;
using std::thread;
using std::vector;

void getAndPrintMsg(); // gets message from server and prints to client
void getAndSendMsg(); // gets message from user and sends it to the server
   
//int main(int argc, char const *argv[])
//{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    std::string message;
    bool getOut = 0; // gets us out of the do while loop later
    
    char buffer[1024] = {0};

int main(){
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
	

vector<thread> threadsSend;
vector<thread> threadsPrint;
//for(int i = 0; i<5; i++){
    do{
        threadsSend.push_back(thread(getAndSendMsg));
        threadsPrint.push_back(thread(getAndPrintMsg));
        //printf("");
        usleep(100000);
        //printf("finished sleeping");
    	//thread t1(getAndSendMsg);
        //thread t2(getAndPrintMsg); 

    }while(!getOut);
//}
       for(auto & th: threadsSend){ th.join();}
       for(auto & th: threadsPrint){ th.join();}

    return 0;
}

// function definitions 
void getAndSendMsg(){
        std::cin.clear();
    	std::cin.sync();
    	// Receive message from user
		//std::cin >> message;
        getline(cin,message);
		if(!message.compare("EXIT")){
			message = "Client has left conversation.";

			send(sock, message.c_str(), message.size(), 0);
			getOut = 1;
		}

    	// send message to server
	    send(sock , message.c_str() , message.size() , 0 );
	    //printf("Hello message sent\n")
        
return;
}

void getAndPrintMsg(){
	    //receive message from server
	    valread = read( sock , buffer, 1024);
	    printf("%s\n",buffer );

//clear out buffer for next round
for(int i = 0; i< sizeof(buffer); i++){ buffer[i] = 0;}
return;
}

