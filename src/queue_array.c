/*************************************
 *
 * filename:	queue_array.c
 * description:	use the array
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

#include "array.h" // use array to implement queue

#include "queue_link.h"

#define T Queue_T
#define ARRAY Array_T

struct T {
	ARRAY head;
	int first;
	int last;
};



/*
 * name: MODULE_FUN_NAME(Queue, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user suggest length of queue
 */
T MODULE_FUN_NAME(Queue, new)(int length)
{
	T q = (T)calloc(1, sizeof(*q));
	if (q)
	{
		q->head = MODULE_FUN_NAME(Array, new)(length, sizeof(void *));
		if (NULL == q->head)
		{
			free(q);
			q = NULL;
		}
		q->first = 0;
		q->last = 0;
	}

	return q;
}

/*
 * name: MODULE_FUN_NAME(Queue, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 * 		@withdata: 1: means free user's data too
 *                  0: menas not free user's data
 */
void MODULE_FUN_NAME(Queue, free)(T *q, int withdata)
{
	void *p = NULL;

	assert(q);
	assert(*q);
	assert((*q)->head);
	assert(1 == withdata || 0 == withdata);

	while (MODULE_FUN_NAME(Queue, isEmpty)(*q) == 0)
	{
		MODULE_FUN_NAME(Queue, get)(*q, &p);
		if (withdata)
			free(p);
	}

	MODULE_FUN_NAME(Array, free)(&(*q)->head);
	*q = NULL;
}

/*
 * name: MODULE_FUN_NAME(Queue, put)
 * description: put @x at the end of queue
 * 			TODO chuan zhi copy
 * return value: 0: put success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to put in queue
 */
int MODULE_FUN_NAME(Queue, put)(T q, void *x)
{
	assert(q);

#define IS_QUEUE_FULL(q)	 \
	((q->last + 1) % MODULE_FUN_NAME(Array, length)(q->head)  \
		== q->first % MODULE_FUN_NAME(Array, length)(q->head))

	if (IS_QUEUE_FULL(q))
		return -1;

	MODULE_FUN_NAME(Array, put)(q->head, q->last, x);
	q->last = (q->last + 1) % MODULE_FUN_NAME(Array, length)(q->head);

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
	assert(q);
	assert(x);

	if (MODULE_FUN_NAME(Queue, isEmpty)(q))
		return -1;

	*x = MODULE_FUN_NAME(Array, get)(q->head, q->first);

	q->first = (q->first + 1) % MODULE_FUN_NAME(Array, length)(q->head);

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

	return q->first == q->last;
}



