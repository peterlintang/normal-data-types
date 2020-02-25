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


#define T SharedMem_T

struct T {
	int	open_count;
	int	size;
	int	read_cur;
	int	write_cur;
	key_t	key;
	int	shmid;
	pthread_mutex_t lock;
//	pthread_cond_t	cond;
};

#define IS_EMPTY(shm)	((shm)->read_cur == (shm)->write_cur)
#define IS_FULL(shm)	((shm)->read_cur % (shm)->size == ((shm)->write_cur + 1) % (shm)->size)
#define SPACE_LEFT(shm)		shm->write_cur >= shm->read_cur ? shm->size - (shm->write_cur - shm->read_cur) - 1 : shm->read_cur - shm->write_cur - 1
#define DATA_AVAILABLE(shm)	shm->write_cur >= shm->read_cur ? shm->write_cur - shm->read_cur : shm->size - (shm->read_cur - shm->write_cur)

/*
 * name: MODULE_FUN_NAME(SharedMem, create)
 * description: create a system v shared memory
 * return value: return pointer to shared memory
 * 				else NULL for error
 * args: @pathname: create the key for shared memory
 * 		 @id: used with @pathname to create the key
 * 		 @size: bytes of memory to create for shared memory
 * 		 @shmflg: flags for create or open shared memory, like permission and group
 * 		 @shmaddr: the address of the calling process to attach shared memory to
 * 		 @flags: flags for attaching memory for readonly or write or exec ...
 */
T MODULE_FUN_NAME(SharedMem, create)(const char *pathname, int id,
				int size, int shmflg,
				const void *shmaddr, int flags)
{
/*
 * create memory shared between processes
 * label	0	not created
 * 		1	created, just open
 */
	key_t		key;
	int		label = 0;
	int		shmid;
	void		*ptr = NULL;
	T shm = NULL;
	pthread_mutexattr_t attr;

	assert(pathname != NULL);

	if ((key = ftok(pathname, id)) == -1) {
		return NULL;
	}

	/*
	 * first check the shared memory exist or not,
	 * if the shared mem exist, just open 
	 * it with read & write permission
	 */
	shmflg |= (IPC_CREAT | IPC_EXCL); /* create if shared mem not exist */
	if (((shmid = shmget(key, size + sizeof(*shm), // create and open should detach
			shmflg)) == -1) && errno == EEXIST) {
		if ((shmid = shmget(key, size + sizeof(*shm), 
					SHM_R | SHM_W)) == -1) { /* if shared mem exist, just open it for read and write */
			return NULL;
		}
		label = 1;	// alread exist
	}

	/* attach to process address space */
	if ((ptr = shmat(shmid, shmaddr, flags)) == (void *)-1) {
		shmctl(shmid, IPC_RMID, 0);
		return NULL;
	}

	shm = (T)ptr;
	if (label) goto out;

	shm->size	= size;
	shm->key	= key;
	shm->shmid	= shmid;
	shm->read_cur	= 0;
	shm->write_cur	= 0;
	shm->open_count	= 0;
	pthread_mutexattr_init(&attr); 
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&shm->lock, &attr);
	pthread_mutexattr_destroy(&attr);

out:
	shm->open_count++;

	return shm;
}

/*
 * name: MODULE_FUN_NAME(SharedMem, destroy)
 * description: destroy a system v shared memory
 * return value: return void
 * args: @shmpp: pointer's pointer to shared mem
 */
void MODULE_FUN_NAME(SharedMem, destroy)(T *shmpp)
{
	T shm = NULL;

	assert(shmpp != NULL);
	assert(*shmpp != NULL);

	shm = *shmpp;

	shm->open_count--;
	if (!shm->open_count) 
		pthread_mutex_destroy(&shm->lock);

	shmctl(shm->shmid, IPC_RMID, 0);
	shmdt(shm);

	*shmpp = NULL;
}

/*
void *address_at(const void *shmaddr, int flags)
{
	void 		*ptr = NULL;
	T shm = shmaddr;

	if ((ptr = shmat(shm->shmid, shmaddr, flags)) == (void *)-1) {
		fprintf(stderr, "unable to get the address of shared memory\n");
		perror("shmat");
		return NULL;
	}

	return ptr;
}

int address_dt(void *shmaddr)
{
	shmdt(shmaddr);
	return 0;
}
*/

/*
 * name: MODULE_FUN_NAME(SharedMem, read)
 * description: read data from a system v shared memory
 * return value: 0: no data
 * 				>0: data read
 * args: @shm: pointer to shared mem
 * 		 @buf: buf for stored data
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, read)(T shm, char *buf, int len)
{
	void *ptr = NULL;
	int read = 0;
	int data_available = 0;

	assert(shm != NULL);
	assert(buf != NULL);
	assert(len > 0);

	ptr = (void *)shm;

	pthread_mutex_lock(&shm->lock);

	/* version 2 */
	data_available = DATA_AVAILABLE(shm);
	read = len > data_available ? data_available : len;

	if (shm->read_cur < shm->write_cur)
	{
		memcpy(buf, ptr + (sizeof(*shm) + shm->read_cur), read);
		shm->read_cur += read;
	}
	else if (shm->read_cur > shm->write_cur)
	{
		if (shm->size - shm->read_cur >= read)
		{
			memcpy(buf, ptr + (sizeof(*shm) + shm->read_cur), read);
			shm->read_cur = (shm->read_cur + read) % shm->size;
		}
		else if (shm->size - shm->read_cur < read)
		{
			memcpy(buf, ptr + (sizeof(*shm) + shm->read_cur), shm->size - shm->read_cur);

			shm->read_cur = 0;
			buf += shm->size - shm->read_cur;
			read -= shm->size - shm->read_cur;

			memcpy(buf, ptr + (sizeof(*shm) + shm->read_cur), read);
			shm->read_cur += read;
		}
		/*
		else if (shm->size - shm->read_cur == read)
		{
			memcpy(buf, ptr + (sizeof(*shm) + shm->read_cur), read);
			shm->read_cur = (shm->read_cur + read) % shm->size;
		}
		*/
	}
	else if (shm->read_cur == shm->write_cur)
	{
			// no data
	}

#if 0 // version 1
	if (IS_EMPTY(shm))
		goto out;

	read = len > DATA_AVAILABLE(shm) ? DATA_AVAILABLE(shm) : len;

	memcpy(buf, ptr + (sizeof(*shm) + shm->read_cur), read);
	shm->read_cur += read;
#endif 

out:
	pthread_mutex_unlock(&shm->lock);
	return read;
}

/*
 * name: MODULE_FUN_NAME(SharedMem, write)
 * description: write data to a system v shared memory
 * return value: 0: mem full, no space to write
 * 				 >0: bytes write
 * args: @shm: pointer to shared mem
 * 		 @buf: data to write
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, write)(T shm, char *buf, int len)
{
	void *ptr = NULL;
	int written = 0;
	int space_left = 0;

	assert(shm != NULL);
	assert(buf != NULL);
	assert(len > 0);

	ptr = (void *)shm;

	pthread_mutex_lock(&shm->lock);

	/* version 2, to improve efficency */
	space_left = SPACE_LEFT(shm);
	written = len > space_left ? space_left : len;

	if (shm->read_cur > shm->write_cur)
	{
		memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, written);
		shm->write_cur += written;
	}
	else if (shm->read_cur <= shm->write_cur)
	{
		if (shm->size - shm->write_cur > written)
		{
			memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, written);
			shm->write_cur += written;
		}
		else
		{
			memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, shm->size - shm->write_cur);

			written -= (shm->size - shm->write_cur);
			buf += shm->size - shm->write_cur;
			shm->write_cur = 0;

			if (0 != written) 
			{
				memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, written);
				shm->write_cur += written;
			}
		}
	}
	/*
	else if (shm->read_cur == shm->write_cur)
	{
		if (shm->size - shm->write_cur > written)
		{
			memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, written);
			shm->write_cur += written;
		}
		else
		{
			memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, shm->size - shm->write_cur);

			written -= (shm->size - shm->write_cur);
			buf += shm->size - shm->write_cur;
			shm->write_cur = 0;

			if (0 == written) break;

			memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, written);
			shm->write_cur += written;
		}
	}
	*/

#if 0  // version 1
	if (IS_FULL(shm))
		goto out;

	written = len > SPACE_LEFT(shm) ? SPACE_LEFT(shm) : len;

	if (shm->read_cur > shm->write_cur)	/* the left space are between shm->write_cur and shm->read_cur */
	{
	}
	else if (shm->read_cur == shm->write_cur)	/* the whole space is empty */
	{
		shm->read_cur = 0;
		shm->write_cur = 0;
	}
	else if (shm->read_cur < shm->write_cur) /* sacrifice efficency */
	{
		/* move the data to the begin of the space */
		memcpy(ptr + sizeof(*shm), ptr + sizeof(*shm) + shm->read_cur, shm->write_cur - shm->read_cur);
		shm->read_cur = 0;
		shm->write_cur = shm->write_cur - shm->read_cur;
	}

	memcpy(ptr + (sizeof(*shm) + shm->write_cur), buf, written);
	shm->write_cur += written;
#endif

out:
	pthread_mutex_unlock(&shm->lock);
	return written;
}

/*

                         |        |
 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
-------------------------------------------------------------------------
|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
-------------------------------------------------------------------------


*/
