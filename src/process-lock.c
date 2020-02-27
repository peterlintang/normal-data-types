
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

#define PROCESS_LOCK_NAME_LEN	256


#define T ProcessLockCond_T

struct T {
//	char	name[PROCESS_LOCK_NAME_LEN];
//	int	id;
//	int	status;		// 0 means can use, 1 means remove. not used now
	int	count;
	int	shmid;
//	void	*lockp;
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
};

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
T MODULE_FUN_NAME(ProcessLockCond, create)(const char *pathname, int id)
{
	int	flags = 0;
	key_t	key;
	int	shmid;
	void	*shmaddr 	 = NULL;
	T lockaddr = NULL;
	pthread_mutexattr_t attr;
	pthread_condattr_t cond_attr;

	assert(pathname != NULL);

	if ((key = ftok(pathname, id)) == -1) {
		return NULL;
	}

	/*
	 * shmget -- system v ipc
	 */
//	flags = (IPC_CREAT | IPC_EXCL | SHM_R | SHM_W);
	flags = (IPC_CREAT | SHM_R | SHM_W);
	if (((shmid = shmget(key, sizeof(*lockaddr), flags)) == -1) &&
			errno == EEXIST) {
		return NULL;
	}

	if ((shmaddr = shmat(shmid, shmaddr, SHM_R | SHM_W)) == (void *)-1) {
		shmctl(shmid, IPC_RMID, 0);
		return NULL;
	}

	lockaddr = (T)shmaddr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&lockaddr->lock, &attr);
        pthread_mutexattr_destroy(&attr);
	pthread_condattr_init(&cond_attr);
	pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&lockaddr->cond, &cond_attr);
	pthread_condattr_destroy(&cond_attr);
	pthread_mutex_lock(&lockaddr->lock);
	lockaddr->shmid = shmid;
	lockaddr->count = 1;
	pthread_mutex_unlock(&lockaddr->lock);
//	lockadd->status	= 0;

//	shmdt(shmaddr);
	return shmaddr;
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
	pthread_mutex_lock(&plock->lock);
	if (!(--plock->count)) {
//		plock->status = 1;
		pthread_mutex_unlock(&plock->lock);
		pthread_mutex_destroy(&plock->lock);
		pthread_cond_destroy(&plock->cond);
		shmctl(plock->shmid, IPC_RMID, 0);
		shmdt(plock);
		return 0;
	} else {
		pthread_mutex_unlock(&plock->lock);
		return -1;
	}
}

/*
 * map the shared memory to the caller's address space
 * return value:
 * 		NULL	error
 * 		pointer to the shared memory	success
 */
/*
 * name: MODULE_FUN_NAME(ProcessLockCond, map)
 * description: map a memory shared lock and cond to process address
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessLockCond, map)(const char *pathname, int id)
{
	key_t	key;
	int	flags = 0;
	int	shmid;
	void	*shmaddr = NULL;
	T plock = NULL;

	assert(pathname != NULL);

	if ((key = ftok(pathname, id)) == -1) {
		return NULL;
	}
	
	flags = (SHM_R | SHM_W);
	if ((shmid = shmget(key, sizeof(*plock), flags)) == -1) {
		return NULL;
	}

	if ((shmaddr = shmat(shmid, shmaddr, SHM_R | SHM_W)) == (void *)-1) {
		shmctl(shmid, IPC_RMID, 0);
		return NULL;
	}

	plock = (T)shmaddr;
	pthread_mutex_lock(&plock->lock);
	plock->count++;
	pthread_mutex_unlock(&plock->lock);

	return plock;
}

/*
 * unmap the shared memory from process's space
 * return value:
 * 		0	success
 * 		-1	arguments error
 */
/*
 * name: MODULE_FUN_NAME(ProcessLockCond, unmap)
 * description: unmap a memory shared lock and cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, unmap)(T plc)
{
	assert(plc != NULL);

	pthread_mutex_lock(&plc->lock);
	plc->count--;
	pthread_mutex_unlock(&plc->lock);
	shmdt(plc);
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

	return pthread_mutex_lock(&plc->lock);
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

	return pthread_mutex_unlock(&plc->lock);
}


/*
 * name: MODULE_FUN_NAME(ProcessLockCond, wait)
 * description: wait a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, wait)(T plc)
{

	assert(plc != NULL);

	return pthread_cond_wait(&plc->cond, &plc->lock);
}

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, signal)
 * description: signal a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, signal)(T plc)
{

	assert(plc != NULL);

	return pthread_cond_signal(&plc->cond);
}

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, broadcast)
 * description: broadcast a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int broadcast_process_cond(T plc)
{
	assert(plc != NULL);

	return pthread_cond_broadcast(&plc->cond);
}



