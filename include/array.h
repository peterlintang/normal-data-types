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


/*
 * name: MODULE_FUN_NAME(Array, new)
 * description: create a new array, which have @length elements, 
 * 		and eache element has @size byte mem space
 * return value: return the pointer to array
 * args: @length: number of elements in array
 * 		 @size: element's size in array
 */
T MODULE_FUN_NAME(Array, new)(int length, int size);

/*
 * name: MODULE_FUN_NAME(Array, free)
 * description: free the allocated array
 * return value: return void
 * args: @array: pointer's pointer to array
 */
void MODULE_FUN_NAME(Array, free)(T *array);

/*
 * name: MODULE_FUN_NAME(Array, length)
 * description: return the length of elements 
 * 				(how many elemtns)in @array
 * return value: return the length of elements
 * args: @array: pointer to array
 */
int MODULE_FUN_NAME(Array, length)(T array);

/*
 * name: MODULE_FUN_NAME(Array, size)
 * description: return the size of elements 
 * 				(one element's mem space) in @array
 * return value: return element's size
 * args: @array: pointer to array
 */
int MODULE_FUN_NAME(Array, size)(T array);

/*
 * name: MODULE_FUN_NAME(Array, get)
 * description: get the index @i of elements in @array
 * return value: return the pointer of element @i in @array
 * args: @array: pointer to array
 * 		 @i: index in array
 */
void *MODULE_FUN_NAME(Array, get)(T array, int i);

/*
 * name: MODULE_FUN_NAME(Array, put)
 * description: put @elem to the index @i of elements in @array
 * return value: return the pointer of element @i in @array (just @elem)
 * args: @array: pointer to array
 * 		 @i: index in array
 * 		 @elem: data to put into @i element in @array
 */
void *MODULE_FUN_NAME(Array, put)
		(T array, int i, void *elem);

/*
 * name: MODULE_FUN_NAME(Array, resize)
 * description: resize @array to new elements' length to @length
 * return value: return void
 * args: @array: pointer to array
 * 		 @length: new length of @array
 */
void MODULE_FUN_NAME(Array, resize)
		(T array, int length);

/*
 * name: MODULE_FUN_NAME(Array, copy)
 * description: copy @array, if @length is less than @array,
 * 			only copy @length elements
 * return value: return new copied array
 * args: @array: pointer to array
 * 		 @length: elements to copy
 */
T MODULE_FUN_NAME(Array, copy)(T array, int length);

#undef T

#endif
