/*************************************
 *
 * filename:	priority_queue.h
 * description:	use the double directions link list
 * 		to implement the priority queue data struture
 * author:
 * date:		2023-12-08
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_PRIORITY_QUEUE_H__
#define CI_PRIORITY_QUEUE_H__

#include "module.h"

#define T Pri_Queue_T
typedef struct T *T;



/*
 * name: MODULE_FUN_NAME(Pri_Queue, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user sugget length of queue
 */
T MODULE_FUN_NAME(Pri_Queue, new)(int length);

/*
 * name: MODULE_FUN_NAME(Pri_Queue, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 * 		@withdata: 1: means free user's data too
 */
void MODULE_FUN_NAME(Pri_Queue, free)(T *q);

/*
 * name: MODULE_FUN_NAME(Pri_Queue, push)
 * description: push @x at the end of queue
 * 		int priority 
 * return value: 0: push success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to push in queue
 */
int MODULE_FUN_NAME(Pri_Queue, push)(T q, int priority, void *x);

/*
 * name: MODULE_FUN_NAME(Pri_Queue, pop)
 * description: pop the head of the queue
 * return value: 0 : pop success
 * 				-1 : error (maybe queue is empty)
 * args: @q pointer to queue
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(Pri_Queue, pop)(T q, void **x);

/*
 * name: MODULE_FUN_NAME(Pri_Queue, isEmpty)
 * description: is queue is empty or not
 * return value: 1: queue is empty
 * 				0 : queue is not empty
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(Pri_Queue, isEmpty)(T q);


#undef T

#endif	// end of CI_PRIORITY_QUEUE_H__
