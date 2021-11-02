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



/*
 * name: MODULE_FUN_NAME(SharedMem, create)
 * description: create a system v shared memory
 * return value: return 0 success, -1 failed
 * pathname: name of shared mem
 * id: id of shared mem
 * size: size of shared mem
 * flag: for create or open if exist
 */
int MODULE_FUN_NAME(SharedMem, create)(const char *pathname, int id,
				int size, int flag);
/*
 * name: MODULE_FUN_NAME(SharedMem, destroy)
 * description: destroy a system v shared memory
 * return value: return void
 * args: @shmpp: pointer's pointer to shared mem
 */
int MODULE_FUN_NAME(SharedMem, destroy)(const char *pathname, int id);

/*
 * 打开一个共享内存，有pathname，id指定，addr是建议隐射到的空间地址，flag是打开标志
 * pathname: 
 * id:
 * addr:
 * len:
 */
void *MODULE_FUN_NAME(SharedMem, open)(const char *pathname, int id, const void *addr, int flag);

/*
 * 关闭一个共享内存空间地址,从进程地址空间中解除隐射
 * ptr: 待解除地址
 */
int MODULE_FUN_NAME(SharedMem, close)(void *ptr);

/*
 * name: MODULE_FUN_NAME(SharedMem, read)
 * description: read data from a system v shared memory
 * return value: return  len of bytes read
 * args: @ptr: pointer from read data 
 * 		 @buf: buf for stored data
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, read)(void *ptr, char *buf, int len);

/*
 * name: MODULE_FUN_NAME(SharedMem, write)
 * description: write data to a system v shared memory
 * return value: return len of bytes write
 * args: @shm: pointer to write data 
 * 		 @buf: data to write
 * 		 @len: length of buf
 */
int MODULE_FUN_NAME(SharedMem, write)(void *ptr, char *buf, int len);


#endif 	// CI_SHARED_MEM_H__

