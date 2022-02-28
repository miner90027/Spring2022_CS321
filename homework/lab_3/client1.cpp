/*
 * client1.cpp
 * Aleks McCormcik
 * 2022/02/25
 * Spring 2022 CS321
 * server client program
 */


#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#define PORT 8080

using std::cout;
using std::endl;
using std::string;
using std::getline;
//using std::strlen;
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    
    string inputMessage;
    char outputMessage[1024] = {0};

    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "\n Socket creation error" << endl;
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        cout << "\nInvalid address/ Address not supported" << endl;
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "\nConnection Failed" << endl;
        return -1;
    }

    do{

   	getline(std::cin, inputMessage);
    send(sock , inputMessage.c_str() , inputMessage.size() , 0 );
    
    printf("Hello message sent\n");
    
    valread = read( sock , outputMessage, 1024);
    printf("%s\n", outputMessage );

    }while(std::strcmp(outputMessage, "STOP")== 0);
    return 0;
}
