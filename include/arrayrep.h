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
	int length;		// total number of elements in array
	int size;		// element's size (mem space)
	char *array;	// pointer to array TODO should use void * ?
};

/*
 * name: MODULE_FUN_NAME(ArrayRep, init)
 * description: init @array 
 * return value: return 
 * args: @array: array to be inited
 * args: @length: total number of elements in array
 * 		 @size: element's size in array
 * 		 @ary: pointer to store elements
 */
void MODULE_FUN_NAME(ArrayRep, init)(T array, 
				int length, 
				int size, 
				void *ary);

#undef T

#endif
