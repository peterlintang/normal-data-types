/*************************************
 *
 * filename:	arrayrep.h
 * description:	implement the arrayrep operations
 * author:
 * date:		2019-09-29
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_ARRAYREP_INCLUDED
#define CI_ARRAYREP_INCLUDED

#include "module.h"

#define T Array_T

struct T {
	int length;
	int size;
	char *array;
};

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
extern void MODULE_FUN_NAME(ArrayRep, init)(T array, int length, int size, void *ary);

#undef T

#endif
