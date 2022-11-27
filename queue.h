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
 * and the next in the queue
 */
struct _queue_node {
	struct _queue_node *next;
	void *data;
};

typedef struct _queue_node queue_node_t;

/*
 * main queue structure where is
 * stored the access lock(mutex)
 * and the reference of the first
 * and last node
 */
typedef struct _queue_main {
	struct _queue_node *first;
	struct _queue_node *last;
	pthread_mutex_t access_mutex;
} queue_t;

/*
 * function used to initialize 
 * an queue structure
 *
 * arg 1: queue reference
 * arg 2: integer to check if
 * 	  should allocate memory
 * 	  for the reference
 */
extern int queue_init(queue_t **, int);

/*
 * function used to clear/free 
 * all the nodes of the queue
 *
 * returns: 0 if OK on error 1
 */
extern int queue_clear(queue_t **);

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
 * returns: 0 if empty else 1
 */
extern int queue_isempty(queue_t *);

/*
 * Function used to create a new node
 * with the data reference stored.
 */  
extern queue_node_t * queue_create_node(const void *data);

/*
 * function will create an node
 * with the data and insert on the 
 * queue referenced by queue_t *.
 */
extern int queue_enqueue_data(queue_t *, const void *);

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
 */
extern int queue_enqueue_cpy(queue_t *, const void *, size_t);

/*
 * function used to get the data from
 * the queue.
 *
 * returns: data if OK on error NULL
 */ 
extern void * queue_dequeue_data(queue_t *);

#endif
