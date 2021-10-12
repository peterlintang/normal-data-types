/*************************************
 *
 * filename:	queue_link.h
 * description:	use the double directions link list
 * 		to implement the queue data struture
 * author:
 * date:		2019-12-20
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_QUEUE_LINK_H__
#define CI_QUEUE_LINK_H__

#include "module.h"

#define T QueueL_T
typedef struct T *T;



/*
 * name: MODULE_FUN_NAME(QueueL, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user sugget length of queue
 */
T MODULE_FUN_NAME(QueueL, new)(int length);

/*
 * name: MODULE_FUN_NAME(QueueL, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 * 		@withdata: 1: means free user's data too
 */
void MODULE_FUN_NAME(QueueL, free)(T *q);

/*
 * name: MODULE_FUN_NAME(QueueL, put)
 * description: put @x at the end of queue
 * return value: 0: put success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to put in queue
 */
int MODULE_FUN_NAME(QueueL, put)(T q, void *x);

/*
 * name: MODULE_FUN_NAME(QueueL, get)
 * description: get the head of the queue
 * return value: 0 : get success
 * 				-1 : error (maybe queue is empty)
 * args: @q pointer to queue
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(QueueL, get)(T q, void **x);

/*
 * name: MODULE_FUN_NAME(QueueL, isEmpty)
 * description: is queue is empty or not
 * return value: 1: queue is empty
 * 				0 : queue is not empty
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(QueueL, isEmpty)(T q);


#undef T

#endif	// end of CI_QUEUE_LINK_H__
