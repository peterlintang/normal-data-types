/*************************************
 *
 * filename:	stack_array.h
 * description:	use the array 
 * 		to implement the stack data struture
 * author:
 * date:		2019-12-20
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_STACK_ARRAY_H__
#define CI_STACK_ARRAY_H__

#include "module.h"

#define T StackA_T
typedef struct T *T;



/*
 * name: MODULE_FUN_NAME(StackA, new)
 * description: create a new stack
 * return value: return the pointer to stack
 * args: @length: total elements for array
 *       @size: element's size for mem space
 */
T MODULE_FUN_NAME(StackA, new)(int length, int size);

/*
 * name: MODULE_FUN_NAME(StackA, free)
 * description: free the stack
 * return value: void
 * args: @q pointer to stack
 */
void MODULE_FUN_NAME(StackA, free)(T *s);

/*
 * name: MODULE_FUN_NAME(StackA, push)
 * description: push @x at the end of stack
 * return value: 0: push success
 * 				-1: no mem
 * args: @q pointer to stack
 * 		@x data to push in stack
 */
int MODULE_FUN_NAME(StackA, push)(T s, void *x);

/*
 * name: MODULE_FUN_NAME(StackA, pop)
 * description: pop the end of the stack
 * return value: 0 : pop success
 * 				-1 : error (maybe stack is empty)
 * args: @q pointer to stack
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(StackA, pop)(T s, void **x);

/*
 * name: MODULE_FUN_NAME(StackA, isEmpty)
 * description: is stack is empty or not
 * return value: 1: stack is empty
 * 				0 : stack is not empty
 * args: @q pointer to stack
 */
int MODULE_FUN_NAME(StackA, isEmpty)(T s);


#undef T

#endif	// end of CI_STACK_ARRAY_H__
