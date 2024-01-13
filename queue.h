#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include<stdint.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<pthread.h>


/* Node structure for squeue. */
struct sq_node {
	struct sq_node 	*next;
	void 		*data;
};

/* Strucutre definition of squeue */
struct squeue {
	struct sq_node 	*head;
	struct sq_node 	*tail;
	pthread_mutex_t	 mutex;
}; 

/*
 * Function initialize the squeue struct
 * if the parameter is NULL it will allocate
 * memory for it.
 *
 * returns:	ON SUCCESS 0
 * 		ON FAIL ENOMEM | EGAIN | EPERM
 */
extern int
squeue_init(struct squeue **);

/*
 * Function used to clear/free 
 * all the nodes of the queue
 * and its data
 *
 * returns: 	ON SUCCESS 0
 * 		ON FAIL 1
 */
extern int
squeue_clear_all(struct squeue **);

/*
 * Function used to free/clear all nodes
 * from squeue reference leaving an empty
 * queue after returns. 
 *
 * returns:	ON SUCCESS 0
 * 		ON FAIL 1
 */
extern int
squeue_clear_nodes(struct squeue **);

/*
 * Function used to clear the mutex lock and after 
 * will set squeue to NULL, so functions will see
 * and treat as an unitialized squeue. You should
 * clear the squeue ref before destroying it.
 *
 * returns: 	ON SUCCESS 0
 * 		ON FAIL 1
 */
extern int
squeue_destroy(struct squeue **);

/*
 * Check if squeue is empty.
 *
 * returns:	ON FALSE 0
 * 		ON TRUE 1 
 */
extern int
squeue_isempty(struct squeue *);

/*
 * Function used to create a new node
 * with the data reference stored in it.
 * 
 * return: 	Node pointer allocated in heap that 
 * 		needs to be freed by clear function or
 * 		manual free after dequeue node.
 *
 * 		NULL on fail
 */  
extern struct sq_node *
squeue_create_node(void *);

/*
 * Function will create a node in heap
 * with the data reference inside
 * and enqueue.
 * 
 * return:	ON SUCCESS 0
 * 		ON ERROR 1
 *  
 */
extern int
squeue_enqueue_data(struct squeue *, void *);

/*
 * this function will insert an
 * already created node on to the
 * queue referenced by squeue *.
 */
extern int
squeue_enqueue_node(struct squeue *, struct sq_node *);

/*
 * this function will create and allocate 
 * a copy of the data referenced 
 * by void *.
 *
 * you will need to free the data later
 */
extern int
squeue_enqueue_cpy(struct squeue *, const void *, size_t);

/*
 * function used to get the data from
 * the queue.
 *
 * returns: data if OK, NULL if empty
 */ 
extern void *
squeue_dequeue_data(struct squeue *);

/*
 * Function used to get node from
 * the queue.
 *
 * returns:	ON SUCCESS 0 
 * 		ON FAIL 1 | 2 in case
 * 		of invalid queue ref
 * 		or empty queue respectively
 */
extern int
squeue_dequeue_node(struct squeue *, struct sq_node **);

#endif /*THREAD_SAFE_QUEUE*/
