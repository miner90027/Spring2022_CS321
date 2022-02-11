//g++ demoThread.c -lpthread -lrt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void *myThreadFun(void *vargp){
	sleep(1);
	printf("printing from thread\n");
	return NULL;
}

int main(){
	pthread_t thread_id;
	printf("Before thread\n");
	pthread_create(&thread_id, NULL, myThreadFun, NULL);
	pthread_join(thread_id, NULL);
	printf("after thread");
	exit(0);
}
