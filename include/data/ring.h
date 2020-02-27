/*************************************
 *
 * filename:	ring.h
 * description:	implement the ring operations
 * author:
 * date:		2019-09-29
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_RING_INCLUDED
#define CI_RING_INCLUDED

#include "module.h"

#define T Ring_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(Ring, new)
 * description: create a new ring
 * return value: return the pointer to ring
 * args: void
 */
T MODULE_FUN_NAME(Ring, new)(void);

/*
 * name: MODULE_FUN_NAME(Ring, ring)
 * description: create a new ring by args
 * return value: return the pointer to ring
 * args: @x: args
 */
T MODULE_FUN_NAME(Ring, ring)(void *x, ...);

/*
 * name: MODULE_FUN_NAME(Ring, free)
 * description: free a allocated ring
 * return value: return void
 * args: @ring: pointer's pointer to ring
 */
void MODULE_FUN_NAME(Ring, free)(T *ring);

/*
 * name: MODULE_FUN_NAME(Ring, length)
 * description: return the length of ring
 * return value: ring's length
 * args: @ring: pointer to ring
 */
int MODULE_FUN_NAME(Ring, length)(T ring);

/*
 * name: MODULE_FUN_NAME(Ring, get)
 * description: get the @i element of ring
 * return value: return the value of @i element
 * args: @ring: pointer to ring
 * 		 @i: index of element
 */
void *MODULE_FUN_NAME(Ring, get)(T ring, int i);

/*
 * name: MODULE_FUN_NAME(Ring, put)
 * description: put the @i element of ring to be @x
 * return value: return the prev value of @i element
 * args: @ring: pointer to ring
 * 		 @i: index of element
 * 		 @x: value to put
 */
void *MODULE_FUN_NAME(Ring, put)
		(T ring, int i, void *x);

/**********************************************************
 *
 * ring structure:
 * positive position:      1  2  3  4  5 6
 * element index:	        0  1  2  3  4
 * negative position:     -5 -4 -3 -2 -1 0
 *
 **********************************************************/
void *MODULE_FUN_NAME(Ring, add)
		(T ring, int pos, void *x);

/*
 * name: MODULE_FUN_NAME(Ring, addlo)
 * description: put @x at the head of ring
 * return value: return the value of @x
 * args: @ring: pointer to ring
 * 		 @x: value to put
 */
void *MODULE_FUN_NAME(Ring, addlo)(T ring, void *x);

/*
 * name: MODULE_FUN_NAME(Ring, addhi)
 * description: put @x at the tail of ring
 * return value: return the value of @x
 * args: @ring: pointer to ring
 * 		 @x: value to put
 */
void *MODULE_FUN_NAME(Ring, addhi)(T ring, void *x);

/*
 * name: MODULE_FUN_NAME(Ring, remove)
 * description: remove @i element in ring
 * return value: return the value of @i element
 * args: @ring: pointer to ring
 * 		 @i: index of element
 */
void *MODULE_FUN_NAME(Ring, remove)(T ring, int i);

/*
 * name: MODULE_FUN_NAME(Ring, rmlo)
 * description: remove head element in ring
 * return value: return the value of head element
 * args: @ring: pointer to ring
 */
void *MODULE_FUN_NAME(Ring, rmlo)(T ring);

/*
 * name: MODULE_FUN_NAME(Ring, rmhi)
 * description: remove tail element in ring
 * return value: return the value of tail element
 * args: @ring: pointer to ring
 */
void *MODULE_FUN_NAME(Ring, rmhi)(T ring);

/**********************************
 *
 * use element of index n as head
 *
 **********************************/
/* if n < 0, left rotate; if n > 0, right rotate */
void MODULE_FUN_NAME(Ring, rotate)(T ring, int n); 


#undef T

#endif
