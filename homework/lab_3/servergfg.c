//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
	
#define TRUE 1
#define FALSE 0
#define PORT 8080

using std::cout;
using std::endl;
	
int main(int argc , char *argv[])
{
	int opt = TRUE;
	int master_socket , addrlen , new_socket , client_socket[30] ,
		max_clients = 30 , activity, i , valread , sd;
	int max_sd;
	struct sockaddr_in address;
		
	//char buffer[1025]; //data buffer of 1K
	char buffer[1024] = {0};
    char buffer2[1024] = {0};
	
	//set of socket descriptors
	fd_set readfds;
		
	//a message
	char *message = "ECHO Daemon v1.0 \r\n";
    char *hello = "Hello from server";
    char *noOtherClientMsg = "couldn't find another client to send to\n";
	
	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}
		
	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
		
	//bind the socket to localhost port 8080
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);
		
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");
		
	while(TRUE)
	{
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
			
		//add child sockets to set
		for ( i = 0 ; i < max_clients ; i++)
		{
			//socket descriptor
			sd = client_socket[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}
			
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
				(address.sin_port));

// say what the server recieved 
//for(int i= 0; i<5; i++){
   // valread = read( new_socket , buffer, 1024);
  //  printf("what we got string: %s\n",buffer );
    //send(new_socket , hello , strlen(hello) , 0 );
    //printf("Hello message sent %i from server\n", i);
//}


			//send new connection greeting message
			//if( send(new_socket, message, strlen(message), 0) != strlen(message) )
			//{
			//	perror("send");
			//}
				
			//puts("Welcome message sent successfully");
				
			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				//if position is empty
				if( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
						
					break;
				}
			}
		}
		
		//else its some IO operation on some other socket
		for (i = 0; i < max_clients; i++)
		{
            //printf("some other IO operation detected"); prints 30 times as expected
			sd = client_socket[i];
            
			//while(true){
			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
                printf("if entered\n");

                // clearout old buffer
                for(int i = 0; i<max_clients; i++){ 
                    //cout << "OB" << i << ":" << buffer[i] << endl;
                    buffer[i] = 0;
                }




				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}
				
	            
				//send the message that came in
				else
				{
                    printf("it was a message\n"); // don't get here for some reason??
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
                    
                    // also send to orignial client (echo back)
                    //send(client_socket[i], buffer, strlen(buffer), 0);
                    //printf("message sent back to sender\n");

                    // pick another client to send it to (a neigbor you don't get to pick who)
                    if(client_socket[i-1] != 0){
					send(client_socket[i-1], buffer , strlen(buffer) , 0 );
                        printf("message sent to previous client\n");
                    }
                    else if(client_socket[i+1] != 0){
                    send(client_socket[i+1], buffer, strlen(buffer), 0);
                    printf("message sent to next client\n");
                    }
                    else{
                    send(sd, noOtherClientMsg , strlen(noOtherClientMsg), 0);
                    printf("server couldn't find another client to send to\n");
                    }
                 
				}
			}
		}
	}
		
	return 0;
}

