/*************************************
 *
 * filename:	share-mem.h
 * description:	implement the system v shared memory interfaces
 * 				for processes access
 * 				system v ipc
 * author:
 * date:		2020-02-23
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/
/*
 * interface to create a shared memory
 * need to reconsider the interface for processes access, 
 * this now is not processes shared
 *
 * now, i think this is processes access ok, see create routine
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "share-mem.h"


/*
 * name: MODULE_FUN_NAME(SharedMem, create)
 * description: create a system v shared memory
 * return value: return 0 success, -1 failed, -2 create key failed
 * pathname: name of shared mem
 * id: id of shared mem
 * size: size of shared mem
 * flag: for create or open if exist
 */
int MODULE_FUN_NAME(SharedMem, create)(const char *pathname, int id,
				int size, int flag)
{
	key_t		key;
	int		shmid;

	assert(pathname != NULL);

	if ((key = ftok(pathname, id)) == -1) 
	{
		return -2;
	}

	/*
	 * first check the shared memory exist or not,
	 * if the shared mem exist, just open 
	 * it with read & write permission
	 */
	flag |= (IPC_CREAT | IPC_EXCL); /* create if shared mem not exist */
	if (((shmid = shmget(key, size, flag)) == -1) && (errno == EEXIST))
	{
		return -1;
	}

	return 0;
}

/*
 * name: MODULE_FUN_NAME(SharedMem, destroy)
 * description: destroy a system v shared memory
 * return value: return void
 * args: @shmpp: pointer's pointer to shared mem
 */
int MODULE_FUN_NAME(SharedMem, destroy)(const char *pathname, int id)
{
	key_t		key;
	int		shmid;
	int shmflg = 0;

	if ((key = ftok(pathname, id)) == -1) 
	{
		return -2;
	}

	if (((shmid = shmget(key, 0, shmflg)) == -1))
	{
		return -1;
	}

	shmctl(shmid, IPC_RMID, 0);

	return 0;
}

void *MODULE_FUN_NAME(SharedMem, open)(const char *pathname, int id, const void *addr, int flag)
{
	key_t		key;
	int		shmid;
	void *ptr = NULL;

	if ((key = ftok(pathname, id)) == -1) 
	{
		return NULL;
	}

	if ((shmid = shmget(key, 0, SHM_R | SHM_W)) == -1) 
	{ 
		return NULL;
	}

	if ((ptr = shmat(shmid, addr, flag)) == (void *)-1) 
	{
		return NULL;
	}

	return ptr;
}

int MODULE_FUN_NAME(SharedMem, close)(void *ptr)
{
	shmdt(ptr);
	return 0;
}


/*
 * name: MODULE_FUN_NAME(SharedMem, read)
 * description: read data from a system v shared memory
 * return value: 0: no data
 * 				>0: data read
 * args: @ptr: pointer from read data
 * 		 @buf: buf for stored data
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, read)(void *ptr, char *buf, int len)
{

	memcpy(buf, ptr, len);
	return len;
}

/*
 * name: MODULE_FUN_NAME(SharedMem, write)
 * description: write data to a system v shared memory
 * return value: 0: mem full, no space to write
 * 				 >0: bytes write
 * args: @ptr: pointer to write data
 * 		 @buf: data to write
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, write)(void *ptr, char *buf, int len)
{
	memcpy(ptr, buf, len);
	return len;
}



