/*************************************
 *
 * filename:	thread_pool.h
 * description:	implement the thread pool operations
 * author:
 * date:		2019-12-26
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_THREAD_POOL_H__
#define CI_THREAD_POOL_H__

#include "module.h"

#define T ThreadPool_T
typedef struct T *T;


typedef void (*MODULE_FUN_NAME(ThreadPool, Cb_T))(void *p);

/*
 * name: MODULE_FUN_NAME(ThreadPool, new)
 * description: create a thread pool
 * return value: return the pointer to thread pool
 * args: size: thread pool size
 * stack_size: thread stack size
 */
T MODULE_FUN_NAME(ThreadPool, new)(int size, int stack_size);

/*
 * name: MODULE_FUN_NAME(ThreadPool, free)
 * description: free the thread pool
 * return value: return void
 * args: @p: pointer's pointer to thread pool
 */
void MODULE_FUN_NAME(ThreadPool, free)(T *p);

/*
 * name: MODULE_FUN_NAME(ThreadPool, init)
 * description: init a thread pool, create threads for the pool
 * return value: 0: success; 
 * 				<0 : error;
 * args: p: pointer to pool 
 */
int MODULE_FUN_NAME(ThreadPool, init)(T p);

/*
 * name: MODULE_FUN_NAME(ThreadPool, destroy)
 * description: stop threads for the pool
 * return value: 0: success;
 * 				<0: error
 * args: p: pointer to pool 
 */
int MODULE_FUN_NAME(ThreadPool, destroy)(T p);

/*
 * name: MODULE_FUN_NAME(ThreadPool, post)
 * description: post a task to thread pool
 * return value: 0: success
 * 				-1: no mem
 * args: @p: pointer to thread pool
 * 		 @cb: user's callback
 * 		 @cl: user's private data, will passed to cb
 */
int MODULE_FUN_NAME(ThreadPool, post)(
			T p, 
			MODULE_FUN_NAME(ThreadPool, Cb_T) cb, 
			void *cl);



#undef T

#endif	// end of CI_THREAD_POOL_H__
