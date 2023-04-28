/*************************************
 *
 * filename:	queue_array.h
 * description:	use the array
 * 		to implement the queue data struture
 * 		复制的是内容，不是指针,
 * 		由于queue_array，在内部用环表示，
 * 		所以可以放的元素等于new时length - 1个元素,
 * 		剩下一个元素用于表示是否到环结尾
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

#define T QueueA_T
typedef struct T *T;



/*
 * name: MODULE_FUN_NAME(QueueA, new)
 * description: create a new queue
 * return value: return the pointer to queue
 * args: @length: user suggest length of queue
 */
T MODULE_FUN_NAME(QueueA, new)(int length, int size);

/*
 * name: MODULE_FUN_NAME(QueueA, free)
 * description: free the queue
 * return value: void
 * args: @q pointer to queue
 */
void MODULE_FUN_NAME(QueueA, free)(T *q);

/*
 * name: MODULE_FUN_NAME(QueueA, put)
 * description: put @x at the end of queue
 * 			TODO chuan zhi copy
 * 		复制的时内容
 * return value: 0: put success
 * 				-1: no mem
 * args: @q pointer to queue
 * 		@x data to put in queue
 */
int MODULE_FUN_NAME(QueueA, put)(T q, void *x);

/*
 * name: MODULE_FUN_NAME(QueueA, get)
 * description: get the head of the queue
 * return value: 0 : get success
 * 				-1 : error (maybe queue is empty)
 * args: @q pointer to queue
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(QueueA, get)(T q, void **x);

/*
 * name: MODULE_FUN_NAME(QueueA, isEmpty)
 * description: is queue is empty or not
 * return value: 1: queue is empty
 * 				0 : queue is not empty
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(QueueA, isEmpty)(T q);

/*
 * name: MODULE_FUN_NAME(QueueA, isFull)
 * description: is queue is full or not
 * return value: 1: queue is full
 * 				0 : queue is not full 
 * args: @q pointer to queue
 */
int MODULE_FUN_NAME(QueueA, isFull)(T q);


#undef T

#endif	// end of CI_QUEUE_ARRAY_H__
