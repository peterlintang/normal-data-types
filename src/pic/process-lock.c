
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

#define T ProcessLock_T

/*
struct T {
	pthread_mutex_t	lock;
};
*/

/*
 * create a new processes shared memory for pthread lock & cond
 * @pathname
 * @id			these two is used for create shared memory
 * return value:
 * 		NULL		error
 * 		pointer	to the shared memory in the process address
 */
/*
 * name: MODULE_FUN_NAME(ProcessLockCond, create)
 * description: create a memory shared lock and cond
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessLockCond, create)(void *ptr, int size)
{
	T lockaddr = NULL;
	pthread_mutexattr_t attr;
	pthread_condattr_t cond_attr;

	assert(size >= sizeof(*lockaddr));

	lockaddr = (T)ptr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(lockaddr->lock), &attr);
    pthread_mutexattr_destroy(&attr);

	return lockaddr;
}

/*
 * destroy the shared memory for pthread lock & cond
 * @pshmaddr	the pointer point to the shared memory's pointer
 * return value:
 * 		-2	arguments error
 * 		-1	someone is using the shared memory
 * 		0	ok
 */
/*
 * name: MODULE_FUN_NAME(ProcessLockCond, destroy)
 * description: destroy a memory shared lock and cond
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, destroy)(T *plc)
{
	T plock = NULL;

	assert(plc != NULL);
	assert(*plc != NULL);

	plock = *plc;

	pthread_mutex_destroy(&(plock->lock));

	return 0;
}


/*
 * get the processes shared lock
 * return value
 * 		-1	arguments error
 * 		0	ok
 */
/*
 * name: MODULE_FUN_NAME(ProcessLockCond, get)
 * description: get a memory shared lock from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, getLock)(T plc)
{
	assert(plc != NULL);

	return pthread_mutex_lock(&(plc->lock));
}

/*
 * release the processes shared lock
 * return value
 * 		-1	arguments error
 * 		0	ok
 */
/*
 * name: MODULE_FUN_NAME(ProcessLockCond, release)
 * description: release a memory shared lock from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, releaseLock)(T plc)
{
	assert(plc != NULL);

	return pthread_mutex_unlock(&(plc->lock));
}

