#include "queue.h"
#include <stdio.h>

int queue_init(queue_t **ref,int alloc)
{
	if(!alloc) 
		(*ref) = (queue_t *)malloc(sizeof(queue_t));
	(*ref)->first = NULL;
	(*ref)->last = NULL;
	
	int result = pthread_mutex_init(&((*ref)->access_mutex), NULL);

	return result;
}

int queue_clear(queue_t **ref)
{
       	queue_node_t *traverse;
	
	while((*ref)->first != NULL) {
		traverse = (*ref)->first;
		(*ref)->first = (*ref)->first->next;
		free(traverse->data);
		free(traverse);
	}
	
	(*ref)->last = NULL;
	return 0;
}

int queue_destroy(queue_t **ref)
{
	pthread_mutex_lock(&(*ref)->access_mutex);
	if((*ref)->first == NULL) {
		pthread_mutex_unlock(&(*ref)->access_mutex);
		pthread_mutex_destroy(&(*ref)->access_mutex);	
		free(*ref);
		(*ref) = NULL;
		return 0;
	}
	pthread_mutex_unlock(&(*ref)->access_mutex);
	return 1;
}

int queue_isempty(queue_t *ref)
{
	if(ref == NULL)
		return 1;

	pthread_mutex_lock(&ref->access_mutex);
	int result = 0;
	
	if(ref->first == NULL && ref->last == NULL) 
		result = 1;
	
	pthread_mutex_unlock(&ref->access_mutex);
	return result;	
}

queue_node_t * queue_create_node(const void *data_arg)
{
	queue_node_t *result = (queue_node_t*)malloc(sizeof(queue_node_t));
	result->next = NULL;
	result->data = data_arg;
	return result;
}


int queue_enqueue_data(queue_t *ref, const void *data_arg)
{
	pthread_mutex_lock(&ref->access_mutex);
	queue_node_t *data_node = queue_create_node(data_arg);
 
	if(ref->first == NULL) {
		ref->first = data_node;
		ref->last = ref->first;
	} else {
		ref->last->next= data_node;
		ref->last = data_node;
	}
	pthread_mutex_unlock(&ref->access_mutex);
	return 0;
}

int queue_enqueue_node(queue_t *ref, queue_node_t *node)
{
	pthread_mutex_lock(&ref->access_mutex);

	if(ref->first == NULL) {
		ref->first = node;
		ref->last = ref->first;
	} else {
		ref->last->next= node;
		ref->last = node;
	}

	pthread_mutex_unlock(&ref->access_mutex);
	return 0;
}

int queue_enqueue_cpy(queue_t *ref, const void *data_arg, size_t data_size)
{
	void *data_cpy = (void*)(malloc(data_size));
	memmove(data_cpy, data_arg, data_size);
	
	pthread_mutex_lock(&ref->access_mutex);
	queue_node_t *data_node = queue_create_node(data_cpy);
 
	if(ref->first == NULL) {
		ref->first = data_node;
		ref->last = ref->first;
	} else {
		ref->last->next= data_node;
		ref->last = data_node;
	}
	pthread_mutex_unlock(&ref->access_mutex);
	
	return 0;
}

void * queue_dequeue_data(queue_t *ref)
{
	if(ref->first != NULL) {
		queue_node_t *tmp = ref->first;
		ref->first = ref->first->next;
		return tmp->data;
	} else
		return NULL;
}
