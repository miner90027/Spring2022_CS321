#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


void *aFunc(void *ptr);

sem_t mutex;

void *aFunc(void* ptr){
	printf("\nWaiting...\n");

	sem_wait(&mutex);

	printf("\nEntered...\n");
	//crit section
	printf("Important thread stuff");
	sleep(5);
	// signal exit
	printf("\nJust exiting...\n");
	sem_post(&mutex);
}

main(){
	sem_init(&mutex, 0, 1);
	pthread_t t1, t2;
	pthread_create(&t1, NULL, aFunc, NULL);
	sleep(2);
	pthread_create(&t2, NULL, aFunc, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	sem_destroy(&mutex);

	return 0;
}
