/*************************************
 *
 * filename:	queue_array.h
 * description:	use the array
 * 		to implement the queue data struture
 * author:
 * date:		2019-12-20
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_QUEUE_ARRAY_H__
#define CI_QUEUE_ARRAY_H__

#include "module.h"

#define T Queue_T
typedef struct T *T;



/*
 * name: MODULE_FUN_NAME(Queue, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user suggest length of queue
 */
T MODULE_FUN_NAME(Queue, new)(int length, int size);

/*
 * name: MODULE_FUN_NAME(Queue, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 */
void MODULE_FUN_NAME(Queue, free)(T *q);

/*
 * name: MODULE_FUN_NAME(Queue, put)
 * description: put @x at the end of queue
 * 			TODO chuan zhi copy
 * return value: 0: put success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to put in queue
 */
int MODULE_FUN_NAME(Queue, put)(T q, void *x);

/*
 * name: MODULE_FUN_NAME(Queue, get)
 * description: get the head of the queue
 * return value: 0 : get success
 * 				-1 : error (maybe queue is empty)
 * args: @q pointer to queue
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(Queue, get)(T q, void **x);

/*
 * name: MODULE_FUN_NAME(Queue, isEmpty)
 * description: is queue is empty or not
 * return value: 1: queue is empty
 * 				0 : queue is not empty
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(Queue, isEmpty)(T q);


#undef T

#endif	// end of CI_QUEUE_ARRAY_H__
