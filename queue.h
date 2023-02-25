#ifndef _THREAD_SAFE_QUEUE_
#define _THREAD_SAFE_QUEUE_

#include<stdint.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>


/*
 * Structure used to 
 * store the actual data
 * and the next in the queue.
 */
typedef struct _queue_node {
	struct _queue_node *next;
	void *data;
} queue_node_t;

/*
 * main queue structure where is
 * stored the access lock(mutex)
 * and the reference of the first
 * and last node.
 */
typedef struct _queue_main {
	queue_node_t *first;
	queue_node_t *last;
	pthread_mutex_t access_mutex;
} queue_t;

/*
 * function used to initialize 
 * the queue structure
 *
 * arg 1: queue reference
 * arg 2: integer to check if
 * 	  should allocate memory
 * 	  for the reference 
 * 	  1 for alloc.
 * 	  0 to init non alloc queue.
 * returns: 0 on success and EAGAIN | ENOMEM | EPERM on error.
 */
extern int queue_init(queue_t **, int);

/*
 * function used to clear/free 
 * all the nodes of the queue
 * and its data
 *
 * returns: 0 if OK on error 1
 */
extern int queue_clear_all(queue_t **);

/*
 * function used to clear/free 
 * all the nodes of the queue
 * but leave leaves the data intact
 *
 * returns: 0 if OK on error 1
 */
extern int queue_clear_nodes(queue_t **);

/*
 * this function will clear/free
 * all the nodes of the queue
 * and also destroy the access lock
 * and after will clear free the queue
 *
 * returns: 0 if OK on error 1
 */
extern int queue_destroy(queue_t **);

/*
 * check if queue is empty.
 *
 * returns: 
 * 	0 if empty.
 * 	1 not empty.
 * 	2 invalid queue. 
 */
extern int queue_isempty(queue_t *);

/*
 * Function used to create a new node
 * with the data reference stored.
 */  
extern queue_node_t * queue_create_node(void *data);

/*
 * function will create an node
 * with the data and insert on the 
 * queue referenced by queue_t *.
 */
extern int queue_enqueue_data(queue_t *, void *);

/*
 * this function will insert an
 * already created node on to the
 * queue referenced by queue_t *.
 */
extern int queue_enqueue_node(queue_t *, queue_node_t *);

/*
 * this function will create and allocate 
 * a copy of the data referenced 
 * by void *.
 *
 * you will need to free the data later
 */
extern int queue_enqueue_cpy(queue_t *, const void *, size_t);

/*
 * function used to get the data from
 * the queue.
 *
 * returns: data if OK, NULL if empty
 */ 
extern void * queue_dequeue_data(queue_t *);

/*
 * function used to get node from
 * the queue.
 *
 * returns: 
 * 	node pointer if OK, 
 * 	NULL on invalid or empty queue.
 */
extern queue_node_t * queue_dequeue_node(queue_t *);

#endif /*_THREAD_SAFE_QUEUE_*/

