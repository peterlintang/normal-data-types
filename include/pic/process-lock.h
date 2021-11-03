/************************************************************
 *
 * filename:	process-lock.c
 * description: implement the interfaces of 
 *              processes shared memory lock
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * copyright (c) john lin
 **************************************************************/

/*
 * implements the pthread lock in multiple processes
 * system v ipc
 * filename:	process-lock.h
 *
 */

#ifndef CI_PROCESS_LOCK_H__
#define CI_PROCESS_LOCK_H__


#include "module.h"

#define T ProcessLock_T
typedef struct T *T;

struct T {
	pthread_mutex_t lock;
};

/*
 * name: MODULE_FUN_NAME(ProcessLock, create)
 * description: create a memory shared lock 
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessLock, create)(void *ptr, int size);

/*
 * name: MODULE_FUN_NAME(ProcessLock, destroy)
 * description: destroy a memory shared lock 
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLock, destroy)(T *plc);


/*
 * name: MODULE_FUN_NAME(ProcessLock, get)
 * description: get a memory shared lock from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLock, getLock)(T plc);

/*
 * name: MODULE_FUN_NAME(ProcessLock, release)
 * description: release a memory shared lock from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLock, releaseLock)(T plc);

#undef T

#endif // CI_PROCESS_LOCK_H__

