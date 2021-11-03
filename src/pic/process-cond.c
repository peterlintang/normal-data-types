
/************************************************************
 *
 * filename:	process-lock.c
 * description: implement the interfaces of 
 *              processes shared memory lock
 *              and condition
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * copyright (c) john lin
 **************************************************************/

/*
 * implements the interfaces of processes' lock
 * using shared memory
 * filename:	process-lock.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#include "process-lock.h"
#include "process-cond.h"


#define T ProcessCond_T

struct T {
	pthread_cond_t	cond;
};

/*
 * create a new processes shared memory for pthread cond
 * @pathname
 * @id			these two is used for create shared memory
 * return value:
 * 		NULL		error
 * 		pointer	to the shared memory in the process address
 */
/*
 * name: MODULE_FUN_NAME(ProcessCond, create)
 * description: create a memory shared lock and cond
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessCond, create)(void *ptr, int size)
{
	T condaddr = NULL;
	pthread_condattr_t cond_attr;

	assert(ptr && size >= sizeof(*condaddr));

	condaddr = (T)ptr;

	pthread_condattr_init(&cond_attr);
	pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&(condaddr->cond), &cond_attr);
	pthread_condattr_destroy(&cond_attr);

	return condaddr;
}

/*
 * destroy the shared memory for pthread cond
 * @pshmaddr	the pointer point to the shared memory's pointer
 * return value:
 * 		-2	arguments error
 * 		-1	someone is using the shared memory
 * 		0	ok
 */
/*
 * name: MODULE_FUN_NAME(ProcessCond, destroy)
 * description: destroy a memory shared lock and cond
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, destroy)(T *pc)
{
	T c = NULL;

	assert(pc != NULL);
	assert(*pc != NULL);

	c = *pc;
	return pthread_cond_destroy(&(c->cond));
}

/*
 * name: MODULE_FUN_NAME(ProcessCond, wait)
 * description: wait a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, wait)(T pc, ProcessLock_T pl)
{

	assert(pc != NULL);
	assert(pl != NULL);

	return pthread_cond_wait(&pc->cond, &pl->lock);
}

/*
 * name: MODULE_FUN_NAME(ProcessCond, signal)
 * description: signal a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, signal)(T pc)
{

	assert(pc != NULL);

	return pthread_cond_signal(&(pc->cond));
}

/*
 * name: MODULE_FUN_NAME(ProcessCond, broadcast)
 * description: broadcast a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, broadcast)(T pc)
{
	assert(pc != NULL);

	return pthread_cond_broadcast(&(pc->cond));
}



