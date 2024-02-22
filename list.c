#include "list.h"

static int nb_dlist_remove(dlist_t *list, dlnode_t *node, void **data)
{
	if (!node && list->size != 0)
		return -1;

	if(*data)
		*data = node->data;
	if (node == list->head) {
		list->head = node->next;		
		if (!list->head) 
			list->tail = NULL;
		else 
			node->next->prev = NULL;
	} else {
		node->prev->next =  node->next;		
		if (!node->next)
			list->tail = node->prev;
		else
			node->next->prev = node->prev;
	}
	free(node);
	list->size--;
	return 0;
}

int dlist_init(dlist_t *ref)
{
	ref->size = 0;	
	ref->tail = NULL;
	ref->head = NULL;
	return pthread_mutex_init(&ref->lock, NULL);

}

int dlist_destroy(dlist_t *list, void(*each)(void *data)) 
{
	void *usr_data;

	pthread_mutex_lock(&list->lock);
	if (each) {
		while (list->size > 0) {
			nb_dlist_remove(list, list->tail, &usr_data);
			each(usr_data);
		}	
	} else {
		while (list->size > 0) {
			nb_dlist_remove(list, list->tail, &usr_data);
		}	
	}

	/* Unlock and destroy thread */
	pthread_mutex_unlock(&list->lock);
	return pthread_mutex_destroy(&list->lock);
}

int dlist_ins_next(dlist_t *list, dlnode_t *node, const void *data)
{
	dlnode_t *new = NULL;
	if(!node && list->size != 0)
		return -1;

	if(!(new = malloc(sizeof(*new))))
		return -1;

	new->data = (void *)data;

	pthread_mutex_lock(&list->lock);	
	/* Is empty? */ 
	if (list->size == 0) {
		list->head = new;	
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new;
	} else {
		new->next = node->next;			
		new->prev = node;

		/* Check if is TAIL*/
		if (!node->next)
			list->tail = new;
		else 
			node->next->prev = new;

		node->next = new;
	}
	list->size++;
	pthread_mutex_unlock(&list->lock);

	return 0;
}
int dlist_ins_prev(dlist_t *list, dlnode_t *node, const void *data)
{

	dlnode_t *new = NULL;


	if(!node && list->size != 0)
		return -1;

	if(!(new = malloc(sizeof(*new))))
		return -1;
	
	new->data = (void *)data;

	pthread_mutex_lock(&list->lock);	
	/* Is empty? */
	if (list->size == 0) {
		list->head = new;	
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new;
	} else {
		new->next = node;
		new->prev = node->prev;

		/* Check if is HEAD*/
		if (!node->prev)
			list->head = new;
		else 
			node->prev->next= new;

		node->prev= new;
	}
	list->size++;
	pthread_mutex_unlock(&list->lock);	
	return 0;
}

int dlist_remove(dlist_t *list, dlnode_t *node, void **data)
{
	if (!node && list->size != 0)
		return -1;

	if(*data)
		*data = node->data;

	pthread_mutex_lock(&list->lock);
	if (node == list->head) {
		list->head = node->next;		
		if (!list->head) 
			list->tail = NULL;
		else 
			node->next->prev = NULL;
	} else {
		node->prev->next =  node->next;		
		if (!node->next)
			list->tail = node->prev;
		else
			node->next->prev = node->prev;
	}
	pthread_mutex_unlock(&list->lock);
	free(node);
	list->size--;

	return 0;
}
