
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "arena.h"

#define T Arena_T


// <macro 98>
#define THRESHOLD	10


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
static T freechunks = NULL;
static int nfree = 0;


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
 * return value: return void
 * args: ap: pointer's pointer to arena
 */
void MODULE_FUN_NAME(Arena, dispose)(T *ap)
{
	assert(ap && *ap);

	Arena_free(*ap);
	free(*ap);
	*ap = NULL;
}

/*
 * name: MODULE_FUN_NAME(Arena, alloc)
 * description: alloc mem from an arena
 * return value: return the pointer to the
 * 		mem allocated 
 * args: arena: arena (mem managed arena)
 * 		 nbytes: total number of bytes to be allocated
 * 		 file: file name
 * 		 line: line number in file
 */
void *MODULE_FUN_NAME(Arena, alloc)(T arena, long nbytes, const char *file, int line)
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
void *MODULE_FUN_NAME(Arena, calloc)(T arena, long count, long nbytes, 
				const char *file, int line)
{
	void *ptr = NULL;

	assert(count > 0);
	assert(nbytes > 0);

	ptr = Arena_alloc(arena, count * nbytes, file, line);
	memset(ptr, '\0', count * nbytes);

	return ptr;
}

/*
 * name: MODULE_FUN_NAME(Arena, free)
 * description: free an arena
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




