
/*
 *
 *   文件名称：arena.c
 *   实现功能：内存的分配与回收
 *   作者：lxj
 *   创建时间: 2021-09-26
 *
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "arena.h"

#define T Arena_T


// <macro 98>
#define THRESHOLD	10	/* 空闲链表的阈值，当空闲链表中的块数少于其值时，用户free的内存放到空闲链表  */


// <types 92>
struct T {
	T prev;
	char *avail;
	char *limit;
};

union align {
	int i;
	long l;
	long *lp;
	void *p;
	void (*fp)(void);
	float f;
	double d;
	long double ld;
};

union header {
	struct T b;
	union align a;
};

// <data 96>
static T freechunks = NULL;		/* 空闲内存链表 */
static int nfree = 0;			/* 空闲内存链表中，空闲内存块的数量 */


// <function 93>
/*
 * name: MODULE_FUN_NAME(Arena, new)
 * description: create a new arena
 * return value: return the pointer arena
 * args: void
 */
T MODULE_FUN_NAME(Arena, new)(void)
{
	T arena = NULL;

	arena = (T)malloc(sizeof(*arena));
	if (NULL != arena)
	{
		arena->prev = NULL;
		arena->limit = NULL;
		arena->avail = NULL;
	}

	return arena;
}

/*
 * name: MODULE_FUN_NAME(Arena, dispose)
 * description: destroy an arena and free the memory 
 * 		allocated for arena
 * 	销毁new创建的arena，但是空闲链表中的内存块没有返回给系统
 * return value: return void
 * args: ap: pointer's pointer to arena
 */
void MODULE_FUN_NAME(Arena, dispose)(T *ap)
{
	assert(ap && *ap);

	MODULE_FUN_NAME(Arena, free)(*ap);
	free(*ap);
	*ap = NULL;
}

/*
 * name: MODULE_FUN_NAME(Arena, alloc)
 * description: alloc mem from an arena
 * 申请nbytes内存，首先判断当前在使用的内存块剩下的内存是否满足要求，
 * 满足要求，则在当前内存块申请，
 * 否则在空闲链表中搜索，如果找到的空闲块内存大小适合，
 * 则分配给用户并把新的内存块当为当前分配内存的块；
 * 否则，从系统申请一块内存，然后把用户需要的内存返回给用户,
 * return value: return the pointer to the
 * 		mem allocated 
 * args: arena: arena (mem managed arena)
 * 		 nbytes: total number of bytes to be allocated
 * 		 file: file name
 * 		 line: line number in file
 */
void *MODULE_FUN_NAME(Arena, alloc)(
		T arena, long nbytes, 
		const char *file, int line)
{
	assert(arena);
	assert(nbytes > 0);

	// < round nbytes 95>
	nbytes = ((nbytes + sizeof(union align) -1) / 
					(sizeof(union align))) * (sizeof(union align));
	while (nbytes > arena->limit - arena->avail)
	{
		// < get a new chunk 95>
		T ptr = NULL;
		char *limit = NULL;
		
		if ((ptr = freechunks) != NULL)
		{
			freechunks = freechunks->prev;
			nfree--;
			limit = ptr->limit;
		}
		else
		{
			long m = sizeof(union header) + nbytes + 10 * 1024;
			ptr = malloc(m);
			if (NULL == ptr)
			{
				return NULL;
			}
			limit = (char *)ptr + m;
		}
		*ptr =*arena;
		arena->avail = (char *)((union header *)ptr + 1);
		arena->limit = limit;
		arena->prev = ptr;
	}

	arena->avail += nbytes;
	return arena->avail - nbytes;
}

/*
 * name: MODULE_FUN_NAME(Arena, calloc)
 * description: alloc mem from an arena and init to zero
 * return value: return the pointer to the
 * 		mem allocated 
 * args: arena: arena (mem managed arena)
 * 		 count: number of elements to allocated
 * 		 nbytes: element's size for mem
 * 		 file: file name
 * 		 line: line number in file
 */
void *MODULE_FUN_NAME(Arena, calloc)(
		T arena, long count, long nbytes, 
		const char *file, int line)
{
	void *ptr = NULL;

	assert(count > 0);
	assert(nbytes > 0);

	ptr = MODULE_FUN_NAME(Arena, alloc)(arena, count * nbytes, file, line);
	memset(ptr, '\0', count * nbytes);

	return ptr;
}

/*
 * name: MODULE_FUN_NAME(Arena, free)
 * description: free an arena
 * 释放申请的内存，如果空闲链表中的内存块数目少于阈值，
 * 则将内存放到空闲链表中，否则，释放内存到系统；
 * return value: return void
 * args: arena: arena to free
 */
void MODULE_FUN_NAME(Arena, free)(T arena)
{
	assert(arena);

	while (arena->prev)
	{
		struct T tmp = *arena->prev;

		if (nfree < THRESHOLD)
		{
			arena->prev->prev = freechunks;
			freechunks = arena->prev;
			nfree++;
			freechunks->limit = arena->limit;
		}
		else
		{
			free(arena->prev);
		}
		*arena = tmp;
	}

	assert(arena->limit == NULL);
	assert(arena->avail == NULL);
}




