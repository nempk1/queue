#include <stdio.h>

#include "queue.h"

#define NUMBER_OF_THREADS 10

queue_t *tqueue = NULL;


void * thread_test1(void *);
void * thread_test2(void *);


int main(int argc, char *argv[])
{
	pthread_t thread_ids[NUMBER_OF_THREADS];
	queue_init(&tqueue, 1);

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
	int i = 0;
	

	int size = 0;
	
	int k = 0;	
	printf("before loop\n");
	while (k < 100) {
		k++;
		void *i = NULL;
		i = queue_dequeue_data(tqueue);
		printf("%d\n", *(int*) i);
		free(i);
	}
	printf("after -100\n");	
	for(queue_node_t *tmp = tqueue->first; tmp !=NULL; tmp = tmp->next) {
		size++;		
	}
	printf("size of tqueue %d\n", size);
	
	queue_clear_all(&tqueue);
	printf("after clear all\n");

	queue_destroy(&tqueue);
	printf("after destroy\n");
	queue_init(&tqueue, 1);
	while(i < 1000) {
		queue_enqueue_data(tqueue, &i);
		i++;
	}
	queue_clear_nodes(&tqueue);
	queue_destroy(&tqueue);
	exit(0);
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
