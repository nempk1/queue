#include "queue.h"

int queue_init(queue_t **ref,int alloc)
{
	if(alloc) { 
		(*ref) = (queue_t *)malloc(sizeof(queue_t));
		if((*ref) == NULL) 
			return ENOMEM;
	}
	(*ref)->first = NULL;
	(*ref)->last = NULL;
	
	int result = pthread_mutex_init(&((*ref)->access_mutex), NULL);

	return result;
}

int queue_clear_all(queue_t **ref)
{
       	queue_node_t *traverse;
	
	pthread_mutex_lock(&(*ref)->access_mutex);
	while((*ref)->first != NULL) {
		traverse = (*ref)->first;
		(*ref)->first = (*ref)->first->next;
		free(traverse->data);
		free(traverse);
	}
	(*ref)->last = NULL;
	pthread_mutex_unlock(&(*ref)->access_mutex);

	return 0;
}

int queue_clear_nodes(queue_t **ref)
{
	if((*ref) == NULL)
		return 1;

       	queue_node_t *traverse;
	pthread_mutex_lock(&(*ref)->access_mutex);

	while((*ref)->first != NULL) {
		traverse = (*ref)->first;
		(*ref)->first = (*ref)->first->next;
		free(traverse);
	}
	(*ref)->last = NULL;
	pthread_mutex_unlock(&(*ref)->access_mutex);
	return 0;
}

int queue_destroy(queue_t **ref)
{
	if((*ref) == NULL)
		return 1;

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
		return 2;
	
	int result = 1;

	pthread_mutex_lock(&ref->access_mutex);
	
	if(ref->first == NULL) 
		result = 0;
	
	pthread_mutex_unlock(&ref->access_mutex);
	return result;	
}

queue_node_t * queue_create_node(void *data_arg)
{
	queue_node_t *result = (queue_node_t*)malloc(sizeof(queue_node_t));
	if(result == NULL)
		return NULL;

	result->next = NULL;
	result->data = data_arg;
	return result;
}


int queue_enqueue_data(queue_t *ref, void *data_arg)
{
	queue_node_t *data_node = queue_create_node(data_arg);

	pthread_mutex_lock(&ref->access_mutex);
	if(ref->first == NULL) { /* Check if queue is empty. */
		ref->last = ref->first = data_node;
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
	if(ref->first == NULL) { /* Check is queue is empty */
		ref->last = ref->first = node;
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
	memcpy(data_cpy, data_arg, data_size);
	queue_node_t *data_node = queue_create_node(data_cpy);

	pthread_mutex_lock(&ref->access_mutex);
	if(ref->first == NULL) {
		ref->last = ref->first = data_node;
	} else {
		ref->last->next= data_node;
		ref->last = data_node;
	}
	pthread_mutex_unlock(&ref->access_mutex);
	
	return 0;
}

void * queue_dequeue_data(queue_t *ref)
{
	queue_node_t *tmp = queue_dequeue_node(ref);
	if(tmp == NULL)
		return tmp;
	void *data_ref = tmp->data;
	free(tmp);
	return data_ref;
}

queue_node_t * queue_dequeue_node(queue_t *ref)
{
	if(ref == NULL) {
		return NULL;
	}

	pthread_mutex_lock(&ref->access_mutex);

	if(ref->first == NULL) {
		pthread_mutex_unlock(&ref->access_mutex);
		return NULL;
	}
	
	queue_node_t *tmp = ref->first;	
	ref->first = ref->first->next;

	pthread_mutex_unlock(&ref->access_mutex);

	return tmp;
}
