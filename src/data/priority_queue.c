/*************************************
 *
 * filename:	priority_queue.c
 * description:	use the double directions link list
 * 		to implement the priority queue data struture
 * author:
 * date:		2023-12-08
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dlist.h" // use double link to implement queue

#include "priority_queue.h"

#define T Pri_Queue_T
#define LIST ListD_T
#define NODE ListDNode_T

struct T {
	LIST head;
};



/*
 * name: MODULE_FUN_NAME(QueueL, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user sugget length of queue
 */
T MODULE_FUN_NAME(QueueL, new)(int length)
{
	T q = (T)calloc(1, sizeof(*q));
	if (q)
	{
		q->head = MODULE_FUN_NAME(ListD, new)();
		if (NULL == q->head)
		{
			free(q);
			q = NULL;
		}
	}

	return q;
}

/*
 * name: MODULE_FUN_NAME(QueueL, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 */
void MODULE_FUN_NAME(QueueL, free)(T *q)
{
	NODE node = NULL;

	assert(q);
	assert(*q);
	assert((*q)->head);

	for (node = MODULE_FUN_NAME(ListD, head)((*q)->head); 
			node;
			node = MODULE_FUN_NAME(ListD, head)((*q)->head))
	{
		MODULE_FUN_NAME(ListD, remove)((*q)->head, node);
		MODULE_FUN_NAME(ListDNode, free)(&node);
	}

	MODULE_FUN_NAME(ListD, free)(&(*q)->head);
}

/*
 * name: MODULE_FUN_NAME(QueueL, put)
 * description: put @x at the end of queue
 * return value: 0: put success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to put in queue
 */
int MODULE_FUN_NAME(QueueL, put)(T q, void *x)
{
	NODE node = NULL;

	assert(q);

	node = MODULE_FUN_NAME(ListDNode, new)(x);
	if (NULL == node)
	{
		return -1;
	}

	MODULE_FUN_NAME(ListD, insert)(q->head, node);
	return 0;
}

/*
 * name: MODULE_FUN_NAME(QueueL, get)
 * description: get the head of the queue
 * return value: 0 : get success
 * 				-1 : error (maybe queue is empty)
 * args: @q pointer to queue
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(QueueL, get)(T q, void **x)
{
	NODE node = NULL;

	assert(q);
	assert(x);

	node = MODULE_FUN_NAME(ListD, head)(q->head);
	if (NULL == node)
		return -1;

	MODULE_FUN_NAME(ListD, remove)(q->head, node);
	*x = node->priv;
	MODULE_FUN_NAME(ListDNode, free)(&node);

	return 0;
}

/*
 * name: MODULE_FUN_NAME(QueueL, isEmpty)
 * description: is queue is empty or not
 * return value: 1: queue is empty
 * 				0 : queue is not empty
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(QueueL, isEmpty)(T q)
{
	assert(q);

	return MODULE_FUN_NAME(ListD, isEmpty)(q->head);
}



