#include "queue.h"

int squeue_init(struct squeue **ref)
{

	if((*ref) == NULL) { 
		(*ref) = malloc(sizeof(struct squeue));
		if((*ref) == NULL) 
			return ENOMEM;
	}
	(*ref)->head = NULL;
	(*ref)->tail = NULL;
	
	return pthread_mutex_init(&((*ref)->mutex), NULL);
}

int squeue_clear_all(struct squeue **ref)
{
       	struct sq_node *traverse;

	if((*ref) == NULL)
		return 1;

	pthread_mutex_lock(&(*ref)->mutex);
	while((*ref)->head != NULL) {
		traverse = (*ref)->head;
		(*ref)->head = (*ref)->head->next;
		free(traverse->data);
		free(traverse);
	}
	(*ref)->tail = NULL;
	pthread_mutex_unlock(&(*ref)->mutex);

	return 0;
}

int squeue_clear_nodes(struct squeue **ref)
{
	struct sq_node *traverse;

	if((*ref) == NULL)
		return 1;

       	pthread_mutex_lock(&(*ref)->mutex);
	while((*ref)->head != NULL) {
		traverse = (*ref)->head;
		(*ref)->head = (*ref)->head->next;
		free(traverse);
	}
	(*ref)->tail = NULL;
	pthread_mutex_unlock(&(*ref)->mutex);

	return 0;
}

int squeue_destroy(struct squeue **ref)
{
	if((*ref) == NULL)
		return 1;

	pthread_mutex_lock(&(*ref)->mutex);
	if((*ref)->head == NULL) {
		pthread_mutex_unlock(&(*ref)->mutex);
		pthread_mutex_destroy(&(*ref)->mutex);	
		free(*ref);
		(*ref) = NULL;
		return 0;
	}
	pthread_mutex_unlock(&(*ref)->mutex);

	return 0;
}

int squeue_isempty(struct squeue *ref)
{
	int result = 1;

    if(ref == NULL)
		return 2;

	pthread_mutex_lock(&ref->mutex);
	
	if(ref->head == NULL) 
		result = 0;
	
	pthread_mutex_unlock(&ref->mutex);
	return result;	
}

struct sq_node* squeue_create_node(void *value)
{
	struct sq_node *result = malloc(sizeof(struct sq_node));
	if(result == NULL)
		return NULL;

	result->next = NULL;
	result->data = value;
	return result;
}


int squeue_enqueue_data(struct squeue *ref, void *data_arg)
{
	struct sq_node *data_node;
        
	if(ref == NULL)
		return 1;
	
	data_node = squeue_create_node(data_arg);

	if(data_node == NULL)
		return 2;
		
	pthread_mutex_lock(&ref->mutex);
	if(ref->head == NULL) {
		ref->tail = ref->head = data_node;
	} else {
		ref->tail->next= data_node;
		ref->tail = data_node;
	}
	pthread_mutex_unlock(&ref->mutex);
	return 0;
}

int squeue_enqueue_node(struct squeue *ref, struct sq_node *node)
{
	if(ref == NULL)
		return 1;

	pthread_mutex_lock(&ref->mutex);
	if(ref->head == NULL) { 
		ref->tail = ref->head = node;
	} else {
		ref->tail->next= node;
		ref->tail = node;
	}
	pthread_mutex_unlock(&ref->mutex);
	return 0;
}

int squeue_enqueue_cpy(struct squeue *ref, const void *data_arg,
                       size_t data_size) {
    struct sq_node *data_node;
	void *data_cpy = malloc(data_size);
	if(data_cpy == NULL)
		return ENOMEM;

	memcpy(data_cpy, data_arg, data_size);
    data_node = squeue_create_node(data_cpy);
        
	if(data_node == NULL) {
		free(data_cpy);
		return 1;
	}

	pthread_mutex_lock(&ref->mutex);
	if(ref->head == NULL) {
		ref->tail = ref->head = data_node;
	} else {
		ref->tail->next= data_node;
		ref->tail = data_node;
	}
	pthread_mutex_unlock(&ref->mutex);
	
	return 0;
}

void* squeue_dequeue_data(struct squeue *ref)
{
    struct sq_node *tmp;
	void *data_ref;

	if(squeue_dequeue_node(ref, &tmp))
		return NULL;		
	
	data_ref = tmp->data;
	free(tmp);
	return data_ref;
}

int squeue_dequeue_node(struct squeue *ref, struct sq_node **result)
{
    struct sq_node *tmp;
            
	if(ref == NULL) {
		return 1;
	}

	pthread_mutex_lock(&ref->mutex);

	if(ref->head == NULL) {
		pthread_mutex_unlock(&ref->mutex);
		return 2;
	}
	
	tmp = ref->head;	
	ref->head = ref->head->next;

	pthread_mutex_unlock(&ref->mutex);
	*result = tmp;
	return 0;
}
