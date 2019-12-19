/*************************************
 *
 * filename:	array.h
 * description:	implement the array operations
 * author:
 * date:		2019-09-29
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_ARRAY_INCLUDED
#define CI_ARRAY_INCLUDED

#include "module.h"

#define T Array_T
typedef struct T *T;

// < function 162 >

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
extern T MODULE_FUN_NAME(Array, new)(int length, int size);
extern void MODULE_FUN_NAME(Array, free)(T *array);

extern int MODULE_FUN_NAME(Array, length)(T array);
extern int MODULE_FUN_NAME(Array, size)(T array);
extern void *MODULE_FUN_NAME(Array, get)(T array, int i);
extern void *MODULE_FUN_NAME(Array, put)
		(T array, int i, void *elem);
extern void MODULE_FUN_NAME(Array, resize)
		(T array, int length);
extern T MODULE_FUN_NAME(Array, copy)(T array, int length);

#undef T

#endif
