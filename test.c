#include <stdio.h>

#include "queue.h"

#define NUMBER_OF_THREADS 2

queue_t *tqueue = NULL;


void * thread_test1(void *);
void * thread_test2(void *);


int main(int argc, char *argv[])
{
	pthread_t thread_ids[NUMBER_OF_THREADS];
	queue_init(&tqueue, 0);

	for(int i = 0; i < NUMBER_OF_THREADS; i++) {
		if(i % 2) {
			if(pthread_create(&thread_ids[i], NULL, thread_test1, NULL))
				perror("Erro na cricao de thread");
		} else {
			if(pthread_create(&thread_ids[i], NULL, thread_test2, NULL))
				perror("Erro na cricao de thread");

		}
		/* printf("thread id %ld created\n", thread_ids[i]); */
	}
	for(int i = 0; i < NUMBER_OF_THREADS; i++) {
		/* printf("thread id %ld joined\n", thread_ids[i]); */
		pthread_join(thread_ids[i], NULL);
	}

	int size = 0;
	for(queue_node_t *tmp = tqueue->first; tmp !=NULL; tmp = tmp->next) {
		size++;		
	}
	printf("size of tqueue %d\n", size);
	queue_clear(&tqueue);
	queue_destroy(&tqueue);
	return 0;
}

void *
thread_test1(void *arg)
{
	for(int i = 0; i < 1000; i++) {
		queue_enqueue_cpy(tqueue, &i, sizeof(int));
	}
	pthread_exit(0);
}

void *
thread_test2(void *arg)
{
	for(int i = 0; i < 1000; i++) {
		queue_enqueue_cpy(tqueue, &i, sizeof(int));
	}
	pthread_exit(0);
}
