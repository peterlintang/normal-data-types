/*************************************
 *
 * filename:	stack_link.c
 * description:	use the double directions link list
 * 		to implement the stack data struture
 * author:
 * date:		2019-12-20
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list2.h"

#include "stack_link.h"

#define T Stack_T
#define LIST List_T
#define NODE ListNode_T

struct T {
	LIST stack;
};


/*
 * name: MODULE_FUN_NAME(Stack, new)
 * description: create a new stack
 * return value: return the pointer to stack
 * args: void
 */
T MODULE_FUN_NAME(Stack, new)(void)
{
	T s = (T)calloc(1, sizeof(*s));
	if (s)
	{
		s->stack = MODULE_FUN_NAME(List, new)();
		if (NULL == s->stack)
		{
			free(s);
			s = NULL;
		}
	}

	return s;
}

/*
 * name: MODULE_FUN_NAME(Stack, free)
 * description: free the stack
 * return value: void
 * args: @q pointer to stack
 * 		@withdata: 1: means free user's data too
 * 					0: menas not free user's data
 */
void MODULE_FUN_NAME(Stack, free)(T *s, int withdata)
{
	void *p = NULL;

	assert(s);
	assert(*s);
	assert(1 == withdata || 0 == withdata);

	while (MODULE_FUN_NAME(Stack, pop)(*s, &p) == 0)
	{
		if (withdata)
			free(p);
	}

	*s = NULL;
}

/*
 * name: MODULE_FUN_NAME(Stack, push)
 * description: push @x at the end of stack
 * return value: 0: push success
 * 				-1: no mem
 * args: @q pointer to stack
 * 		@x data to push in stack
 */
int MODULE_FUN_NAME(Stack, push)(T s, void *x)
{
	NODE node = NULL;

	assert(s);

	node = MODULE_FUN_NAME(ListNode, new)(x);
	if (NULL == node)
		return -1;

	MODULE_FUN_NAME(List, insert)(s->stack, node);
	return 0;
}

/*
 * name: MODULE_FUN_NAME(Stack, pop)
 * description: pop the end of the stack
 * return value: 0 : pop success
 * 				-1 : error (maybe stack is empty)
 * args: @q pointer to stack
 * 		@x pointer's pointer to store the return value
 */
int MODULE_FUN_NAME(Stack, pop)(T s, void **x)
{
	NODE node = NULL;

	assert(s);
	assert(x);

	node = MODULE_FUN_NAME(List, tail)(s->stack);
	if (NULL == node)
		return -1;

	MODULE_FUN_NAME(List, remove)(s->stack, node);
	*x = node->priv;
	MODULE_FUN_NAME(ListNode, free)(&node);

	return 0;
}

/*
 * name: MODULE_FUN_NAME(Stack, isEmpty)
 * description: is stack is empty or not
 * return value: 1: stack is empty
 * 				0 : stack is not empty
 * args: @q pointer to stack
 */
int MODULE_FUN_NAME(Stack, isEmpty)(T s)
{
	assert(s);

	return MODULE_FUN_NAME(List, isEmpty)(s->stack);
}



