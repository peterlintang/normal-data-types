/*************************************
 *
 * filename:	queue_link.c
 * description:	use the double directions link list
 * 		to implement the queue data struture
 * author:
 * date:		2019-12-20
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dlist.h" // use double link to implement queue

#include "queue_link.h"

#define T Queue_T
#define LIST List_T
#define NODE ListNode_T

struct T {
	LIST head;
};



/*
 * name: MODULE_FUN_NAME(Queue, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user sugget length of queue
 */
T MODULE_FUN_NAME(Queue, new)(int length)
{
	T q = (T)calloc(1, sizeof(*q));
	if (q)
	{
		q->head = MODULE_FUN_NAME(List, new)();
		if (NULL == q->head)
		{
			free(q);
			q = NULL;
		}
	}

	return q;
}

/*
 * name: MODULE_FUN_NAME(Queue, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 */
void MODULE_FUN_NAME(Queue, free)(T *q)
{
	NODE node = NULL;

	assert(q);
	assert(*q);
	assert((*q)->head);

	for (node = MODULE_FUN_NAME(List, head)((*q)->head); 
			node;
			node = MODULE_FUN_NAME(List, head)((*q)->head))
	{
		MODULE_FUN_NAME(List, remove)((*q)->head, node);
		MODULE_FUN_NAME(ListNode, free)(&node);
	}

	MODULE_FUN_NAME(List, free)(&(*q)->head);
}

/*
 * name: MODULE_FUN_NAME(Queue, put)
 * description: put @x at the end of queue
 * return value: 0: put success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to put in queue
 */
int MODULE_FUN_NAME(Queue, put)(T q, void *x)
{
	NODE node = NULL;

	assert(q);

	node = MODULE_FUN_NAME(ListNode, new)(x);
	if (NULL == node)
	{
		return -1;
	}

	MODULE_FUN_NAME(List, insert)(q->head, node);
	return 0;
}

/*
 * name: MODULE_FUN_NAME(Queue, get)
 * description: get the head of the queue
 * return value: 0 : get success
 * 				-1 : error (maybe queue is empty)
 * args: @q pointer to queue
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(Queue, get)(T q, void **x)
{
	NODE node = NULL;

	assert(q);
	assert(x);

	node = MODULE_FUN_NAME(List, head)(q->head);
	if (NULL == node)
		return -1;

	MODULE_FUN_NAME(List, remove)(q->head, node);
	*x = node->priv;
	MODULE_FUN_NAME(ListNode, free)(&node);

	return 0;
}

/*
 * name: MODULE_FUN_NAME(Queue, isEmpty)
 * description: is queue is empty or not
 * return value: 1: queue is empty
 * 				0 : queue is not empty
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(Queue, isEmpty)(T q)
{
	assert(q);

	return MODULE_FUN_NAME(List, isEmpty)(q->head);
}



