#include <stdio.h>
#include <pthread.h>

#define NUMBER_OF_THREADS 10


void * thread_test1(void *);
void * thread_test2(void *);


int main(int argc, char *argv[])
{
	pthread_t thread_ids[NUMBER_OF_THREADS];

	for(int i = 0; i < NUMBER_OF_THREADS; i++) {
		if(i % 2) {
			if(pthread_create(&thread_ids[i], NULL, thread_test1, NULL))
				perror("Erro na cricao de thread");
		} else {
			if(pthread_create(&thread_ids[i], NULL, thread_test2, NULL))
				perror("Erro na cricao de thread");

		}
	}
	for(int i = 0; i < NUMBER_OF_THREADS; i++) {
		pthread_join(thread_ids[i], NULL);
	}
	return 0;
}

void *
thread_test1(void *arg)
{
	for(int i = 0; i < 1000; i++) {
		int k = i;
	}
	pthread_exit(0);
}

void *
thread_test2(void *arg)
{
	for(int i = 0; i < 1000; i++) {
		int k = i;
	}
	pthread_exit(0);
}
