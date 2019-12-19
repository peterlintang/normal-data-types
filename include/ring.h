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
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
extern T MODULE_FUN_NAME(Ring, new)(void);
extern T MODULE_FUN_NAME(Ring, ring)(void *x, ...);
extern void MODULE_FUN_NAME(Ring, free)(T *ring);

extern int MODULE_FUN_NAME(Ring, length)(T ring);
extern void *MODULE_FUN_NAME(Ring, get)(T ring, int i);
extern void *MODULE_FUN_NAME(Ring, put)
		(T ring, int i, void *x);

extern void *MODULE_FUN_NAME(Ring, add)
		(T ring, int pos, void *x);
extern void *MODULE_FUN_NAME(Ring, addlo)(T ring, void *x);
extern void *MODULE_FUN_NAME(Ring, addhi)(T ring, void *x);
extern void *MODULE_FUN_NAME(Ring, remove)(T ring, int i);
extern void *MODULE_FUN_NAME(Ring, rmlo)(T ring);
extern void *MODULE_FUN_NAME(Ring, rmhi)(T ring);

/* if n < 0, left rotate; if n > 0, right rotate */
extern void MODULE_FUN_NAME(Ring, rotate)(T ring, int n); 


#undef T

#endif
