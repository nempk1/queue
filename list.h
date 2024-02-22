#ifndef THREAD_SAFE_LIST
#define THREAD_SAFE_LIST

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>


/* Node structure for dlist. */
typedef struct dl_node {
	void 		*data;
	struct dl_node 	*next;
	struct dl_node  *prev;
} dlnode_t;

/* Strucutre definition of dlist */
typedef struct dlist {
	long 		 size;
	struct dl_node 	*head;
	struct dl_node 	*tail;

	pthread_mutex_t  lock;
} dlist_t; 

extern int 
dlist_init(dlist_t *);

extern int 
dlist_destroy(dlist_t *, void (*each)(void *data));

extern int
dlist_ins_next(dlist_t *, dlnode_t *, const void *);

extern int
dlist_ins_prev(dlist_t *, dlnode_t *, const void *);

extern int
dlist_remove(dlist_t *, dlnode_t *, void **);

#endif /*THREAD_SAFE_LIST*/
