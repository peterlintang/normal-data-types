/*************************************
 *
 * filename:	stack_link.h
 * description:	use the double directions link list
 * 		to implement the stack data struture
 * author:
 * date:		2019-12-20
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_STACK_LINK_H__
#define CI_STACK_LINK_H__

#include "module.h"

#define T StackL_T
typedef struct T *T;



/*
 * name: MODULE_FUN_NAME(StackL, new)
 * description: create a new stack
 * return value: return the pointer to stack
 * args: void
 */
T MODULE_FUN_NAME(StackL, new)(void);

/*
 * name: MODULE_FUN_NAME(StackL, free)
 * description: free the stack
 * return value: void
 * args: @q pointer to stack
 * 		@withdata: 1: means free user's data too
 * 					0: menas not free user's data
 */
void MODULE_FUN_NAME(StackL, free)(T *s);

/*
 * name: MODULE_FUN_NAME(StackL, push)
 * description: push @x at the end of stack
 * return value: 0: push success
 * 				-1: no mem
 * args: @q pointer to stack
 * 		@x data to push in stack
 */
int MODULE_FUN_NAME(StackL, push)(T s, void *x);

/*
 * name: MODULE_FUN_NAME(StackL, pop)
 * description: pop the end of the stack
 * return value: 0 : pop success
 * 				-1 : error (maybe stack is empty)
 * args: @q pointer to stack
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(StackL, pop)(T s, void **x);

/*
 * name: MODULE_FUN_NAME(StackL, isEmpty)
 * description: is stack is empty or not
 * return value: 1: stack is empty
 * 				0 : stack is not empty
 * args: @q pointer to stack
 */
int MODULE_FUN_NAME(StackL, isEmpty)(T s);


#undef T

#endif	// end of CI_STACK_LINK_H__
