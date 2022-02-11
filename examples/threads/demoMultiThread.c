//g++ demoThread.c -lpthread -lrt

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_func(void *ptr);

void *print_message_func(void *ptr){
	char* message;
	message = (char *) ptr;
	printf("%s \n", message);
}

main(void){
	pthread_t thread1, thread2;
	char *message1 = "thread 1";
	char *message2 = "thread 2";
	int iret1, iret2;

	iret1 = pthread_create(&thread1, NULL, print_message_func, (void*) message1);
	iret2 = pthread_create(&thread2, NULL, print_message_func, (void*) message2);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("thread 1 returns: %d\n", iret1);
	printf("thread 2 returns: %d\n", iret2);
	exit(0);
}
