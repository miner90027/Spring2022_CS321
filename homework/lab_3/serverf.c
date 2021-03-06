



//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <signal.h>
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
#include <fcntl.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define PORT 8080

using std::cout;
using std::endl;

void sigintHandler(int sig_num);



int main(int argc , char *argv[])
{

signal(SIGINT, sigintHandler);

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

// SET SOCKET TO NONBLOCKING 
int flags = fcntl(master_socket, F_GETFL, 0);
fcntl(master_socket, F_SETFL, flags | O_NONBLOCK);



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
// or it means we just typed Ctrl C
if (FD_ISSET(master_socket, &readfds))
{


    if ((new_socket = accept(master_socket,
    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        // continue...
    }

    else{
//inform user of socket number - used in send and receive commands
printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
(address.sin_port));

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
}


//else its some IO operation on some other socket
for (i = 0; i < max_clients; i++)
{
            
sd = client_socket[i];
           
// SET SOCKET TO NONBLOCKING 
int flags = fcntl(sd, F_GETFL, 0);
fcntl(sd, F_SETFL, flags | O_NONBLOCK);

if (FD_ISSET( sd , &readfds))
{
//Check if it was for closing , and also read the
//incoming message

                // clearout old buffer
                for(int i = 0; i<max_clients; i++){
                    //cout << "OB" << i << ":" << buffer[i] << endl;
                    buffer[i] = 0;
                }

valread = read( sd , buffer, 1024);
if (valread == 0)
{
//Somebody disconnected , get his details and print
getpeername(sd , (struct sockaddr*)&address , \
(socklen_t*)&addrlen);
printf("Host disconnected , ip %s , port %d \n" ,
inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

//Close the socket and mark as 0 in list for reuse
close( sd );
client_socket[i] = 0;
//exit(0);
}

else if(valread < 0 ){
// continue...
}
           
//send the message that came in
else
{
//set the string terminating NULL byte on the end
//of the data read
buffer[valread] = '\0';
                   
                    

                    // pick another client to send it to 
                    // send it to a neigbor you don't get to pick who
                    if(client_socket[i-1] != 0){
                        send(client_socket[i-1], buffer , strlen(buffer) , 0 );
                        printf("message sent from client %d to client %d\n",i, i-1);
                    }
                    else if(client_socket[i+1] != 0){
                    send(client_socket[i+1], buffer, strlen(buffer), 0);
                    printf("message sent from client %d to client %d\n",i, i+1);
                    }
                    else{
                    send(sd, noOtherClientMsg , strlen(noOtherClientMsg), 0);
                    printf("server couldn't find another client to send to\n");
                    }

// make sure BYE wasnt sent and exit it if was
            bool byeSent = true;
            char* goodBye = "BYE";
            char* sentByeMsg = "Client sent BYE so server process has ended";
            for(int i=0; i<strlen(buffer); i++){
                if (buffer[i] != goodBye[i] || (strlen(buffer) != strlen(goodBye)) ){
                byeSent = false;
                }
            }
            if( byeSent){ // let both clients know
            
            printf("BYE was sent, exiting server process\n");
            send(sd, sentByeMsg, strlen(sentByeMsg), 0);

                if(client_socket[i-1] != 0){
                send(client_socket[i-1], sentByeMsg, strlen(sentByeMsg), 0 );
        
                }
                else if(client_socket[i+1] != 0){
                send(client_socket[i+1], sentByeMsg, strlen(sentByeMsg), 0 );
                
                }
                else{
                // we've already let people know
                    }

            // then exit
            exit(0);
            }
                 
}
}
}
}


return 0;
}


void sigintHandler(int sig_num){
signal(SIGINT, sigintHandler);
printf("cannot be terminated with Ctrl C\n");
return;
}
   







