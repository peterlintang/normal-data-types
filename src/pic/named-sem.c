/*************************************
 *
 * filename:	named-sem.c
 * description:	implements the named semphore
 * 			for process usage
 * author:		john lin
 * date:		2020-02-22
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <assert.h>

#include "named-sem.h"

#define CREATE_MODE	(S_IRUSR | S_IWUSR)

#define T NamedSem_T

struct T {
	char	*name;
	sem_t	*sem;
};

/*
 * name: MODULE_FUN_NAME(NamedSem, create)
 * description: create a new named semphore 
 * return value: 0: create success
 * 				-1: create failed
 * args: @pathname: named semphore pathname
 * 		 @flags:	flags to open named semphore
 * 		 @res_num:	initial value for named semphore
 */
int MODULE_FUN_NAME(NamedSem, create)(const char *pathname, int flags, int res_num)
{
	sem_t	*sem = NULL;

	assert(pathname != NULL);

	flags |= (O_CREAT | O_EXCL | O_RDWR);
	sem = sem_open(pathname, flags, CREATE_MODE, res_num);
	if (sem == SEM_FAILED) {
		return -1;
	}

	sem_close(sem);

	return 0;
}


/*
 * name: MODULE_FUN_NAME(NamedSem, destroy)
 * description: destroy a named semphore 
 * return value: 0: destroy success;
 * 				-1: failed 
 * args: @pathname: named semphore pathname
 */
int MODULE_FUN_NAME(NamedSem, destroy)(const char *pathname)
{
	assert(pathname != NULL);

	return sem_unlink(pathname);
}


/*
 * name: MODULE_FUN_NAME(NamedSem, open)
 * description: open a named semphore
 * return value: pointer to named semphore
 * args: @pathname: named semphore pathname
 */
T MODULE_FUN_NAME(NamedSem, open)(const char *pathname)
{
	T nsem = NULL;

	assert(pathname != NULL);

	if (!(nsem = calloc(1, sizeof(*nsem)))) {
		return NULL;
	}

	if (!(nsem->name = strdup(pathname))) {
		free(nsem);
		return NULL;
	}

	if ((nsem->sem = sem_open(nsem->name, O_RDWR)) == SEM_FAILED) {
		free(nsem->name);
		free(nsem);
		return NULL;
	}

	return nsem;
}

/*
 * name: MODULE_FUN_NAME(NamedSem, close)
 * description: close a named semphore
 * return value: return void
 * args: @psem: pointer's pointer to named semphore
 */
void MODULE_FUN_NAME(NamedSem, close)(T *psem)
{
	T sem = NULL;

	assert(psem != NULL);
	assert(*psem != NULL);

	sem = *psem;
	if (sem->sem) {
		sem_close(sem->sem);
	}
	free(sem->name);
	free(sem);
	*psem = NULL;
}

/*
 * name: MODULE_FUN_NAME(NamedSem, wait)
 * description: wait named semphore to be greater than 1
 * return value: -2: arg error
 * 				 -1: wait failed
 * 				  0: success
 * args: @sem: pointer to named semphore
 * 		 @nonblock: block or not, 1: non block, 0: block wait
 */
int MODULE_FUN_NAME(NamedSem, wait)(T sem, int nonblock)
{
	assert(sem != NULL);

	if (!sem->sem) {
		return -2;
	}

	if (nonblock) {
		return sem_trywait(sem->sem);
	}

	return sem_wait(sem->sem);
}

/*
 * name: MODULE_FUN_NAME(NamedSem, post)
 * description: post a named semphore
 * return value: -2: arg error
 * 				 -1: post failed
 * 				  0: success
 * args: @sem: pointer to named semphore
 */
int MODULE_FUN_NAME(NamedSem, post)(T sem)
{
	assert(sem != NULL);

	if (!sem->sem) {
		return -2;
	}
	
	return sem_post(sem->sem);
}


