/*
 * server.cpp
 * Aleks McCormick
 * 2022/02/23
 * Spring 2022 CS321
 * A local server that handles communication between 2 clients
 */


#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
using std::cout, std::endl;

#define PORT 8080

sem_t x, y;
pthread_t tid;
pthread_t writerThread[2];
pthread_t readerThread[2];
int readCount = 0;

void* reader(void* _empty){
	sem_wait(&x);
	readCount++;

	if( readCount == 1){
		sem_wait(&y);
	}

	sem_post(&y);

	cout << readCount << " reader is insde" << endl;

	sleep(5);

	sem_wait(&x);
	readCount--;

	if(readCount == 0){
		sem_post(&y);
	}

	sem_post(&x);

	cout << readCount+1 << " Reader is leaving" << endl;

	pthread_exit(NULL);
}


void* writer(void* _empty){
	cout << "Writer is trying to enter" << endl;
	
	sem_wait(&y);

	cout << "Writer has entered" << endl;

	sem_post(&y);

	cout << "Writer has exited" << endl;

	pthread_exit(NULL);
}



int main(void)
{
    int servSocket, newSocket;
    int opt = 1;
    struct sockaddr_in address;
    struct sockaddr_storage serverStorage;
    
    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

       
    // Creating socket file descriptor
    if ((servSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(servSocket, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(servSocket, 4) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t tid[4];

    int i = 0;

    while(1){
		addr_size = sizeof(serverStorage);

		//newSocket = accept(servSocket, (struct sockaddr *)&serverStorage, &addr_size);

		if ((newSocket = accept(servSocket, (struct sockaddr *)&serverStorage, &addr_size)))
        {
	        perror("accept");
	        exit(EXIT_FAILURE);
	    }

	    int choice = 0;

	    recv(newSocket, &choice, sizeof(choice), 0);

	    if(choice == 1){
	    	if(pthread_create(&readerThread[i++], NULL, reader, &newSocket) != 0){
	    		cout << "Failed to create reader thread" << endl;
    		}
		}
		else if(choice == 2){
			if(pthread_create(&writerThread[i++], NULL, writer, &newSocket) != 0){
				cout << "Failed to create writer thread" << endl;
			}
		}

		if( i >= 4 ){

			i = 0;
			while(i < 4){
				pthread_join(writerThread[i++], NULL);
				pthread_join(readerThread[i++], NULL);
			}

			i = 0;
		}
    }
	return 0;
}
    /*
    

    if ((socket2 = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    do{
    
	    valread = read( socket1 , buffer, 1024);
	    printf("%s\n",buffer );
	    send(socket1 , messageOut , strlen(messageOut) , 0 );
	    printf("Hello message sent\n");
    }while(socket1);
    return 0;
}
*/
