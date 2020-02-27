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

#ifndef CI_SHARED_MEM_H__
#define CI_SHARED_MEM_H__


#include "module.h"

#define T SharedMem_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(SharedMem, create)
 * description: create a system v shared memory
 * return value: return 
 * args: @length: total number of elements in array
 */
T MODULE_FUN_NAME(SharedMem, create)(const char *pathname, int id,
				int size, int flag,
				const void *shmaddr, int flag2);
/*
 * name: MODULE_FUN_NAME(SharedMem, destroy)
 * description: destroy a system v shared memory
 * return value: return void
 * args: @shmpp: pointer's pointer to shared mem
 */
void MODULE_FUN_NAME(SharedMem, destroy)(T *shmpp);


/*
 * name: MODULE_FUN_NAME(SharedMem, read)
 * description: read data from a system v shared memory
 * return value: return 
 * args: @shm: pointer to shared mem
 * 		 @buf: buf for stored data
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, read)(T shm, char *buf, int len);

/*
 * name: MODULE_FUN_NAME(SharedMem, write)
 * description: write data to a system v shared memory
 * return value: return 
 * args: @shm: pointer to shared mem
 * 		 @buf: data to write
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, write)(T shm, char *buf, int len);

/*
void *address_at(const void *shmaddr, int flags);
int address_dt(void *shmaddr);
*/

#undef T

#endif 	// CI_SHARED_MEM_H__

