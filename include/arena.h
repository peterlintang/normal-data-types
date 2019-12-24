
/*************************************
 *
 * filename:	arena.h
 * description:	implement the memory manager operations
 * author:
 * date:		2019-12-25
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/
#ifndef IC_ARENA_H__
#define IC_ARENA_H__

#include "except.h"

#define T Arena_T
typedef struct T *T;


// < functions 91>
/*
 * name: MODULE_FUN_NAME(Arena, new)
 * description: create a new arena
 * return value: return the pointer arena
 * args: void
 */
T MODULE_FUN_NAME(Arena, new)(void);

/*
 * name: MODULE_FUN_NAME(Arena, dispose)
 * description: destroy an arena and free the memory 
 * 		allocated for arena
 * return value: return void
 * args: ap: pointer's pointer to arena
 */
void MODULE_FUN_NAME(Arena, dispose)(T *ap);

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
void *MODULE_FUN_NAME(Arena, alloc)(
			T arena, long nbytes, 
			const char *file, int line);
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
			const char *file, int line);
/*
 * name: MODULE_FUN_NAME(Arena, free)
 * description: free an arena
 * return value: return void
 * args: arena: arena to free
 */
void MODULE_FUN_NAME(Arena, free)(T arena);

#undef T

#endif // end of IC_ARENA_H__
