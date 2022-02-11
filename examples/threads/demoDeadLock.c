#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void *func1(void *ptr);
void *func2(void *ptr);

sem_t mutex1;
sem_t mutex2;

void *func1(void *ptr){
	sem_wait(&mutex1);
	printf("Thread One acquired mutex1\n");
	sleep(1);
	sem_wait(&mutex2);
	printf("Thread One acquired mutex2\n");
	sem_post(&mutex2);
	printf("Thread One released mutex2\n");
	sem_post(&mutex1);
	printf("Thread One released mutex1\n");
}

void *func2(void *ptr){
	sem_wait(&mutex2);
	printf("Thread Two acquired mutex2\n");
	sleep(1);
	sem_wait(&mutex1);
	printf("Thread Two acquired mutex1\n");
	sem_post(&mutex1);
	printf("Thread Two released mutex1\n");
	sem_post(&mutex2);
	printf("Thread Two released mutex2\n");
}


int main(void){
	sem_init(&mutex1, 0, 1);
	sem_init(&mutex2, 0, 1);

	pthread_t one, two;

	pthread_create(&one, NULL, func1, NULL);
	pthread_create(&two, NULL, func2, NULL);

	pthread_join(one, NULL);
	pthread_join(two, NULL);

	printf("Threads joined\n");
}
