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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "array.h"

#include "stack_array.h"

#define T Stack_T
#define ARRAY Array_T

struct T {
	ARRAY array;
	int num;
};



/*
 * name: MODULE_FUN_NAME(Stack, new)
 * description: create a new stack
 * return value: return the pointer to stack
 * args: @length: total elements for array
 * 		 @size: element's size for mem space
 */
T MODULE_FUN_NAME(Stack, new)(int length, int size)
{
	T stack = NULL;

	assert(length > 0);
	assert(size > 0);

	stack = (T)calloc(1,sizeof(*stack));
	if (stack)
	{
		stack->array = MODULE_FUN_NAME(Array, new)(length, size);
		if (NULL == stack->array)
		{
			free(stack);
			stack = NULL;
		}
		stack->num = 0;
	}

	return stack;
}

/*
 * name: MODULE_FUN_NAME(Stack, free)
 * description: free the stack
 * return value: void
 * args: @q pointer to stack
 */
void MODULE_FUN_NAME(Stack, free)(T *s)
{
	void *p = NULL;

	assert(s);
	assert(*s);

	while (MODULE_FUN_NAME(Stack, isEmpty)(*s) == 0)
	{
		MODULE_FUN_NAME(Stack, pop)(*s, &p);
	}

	MODULE_FUN_NAME(Array, free)(&(*s)->array);

	free(*s);
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
	assert(s);
	assert(x);

	if (MODULE_FUN_NAME(Array, length)(s->array) > s->num)
	{
		MODULE_FUN_NAME(Array, put)(s->array, s->num, x);
		s->num++;
	}
	else
		return -1;
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
	assert(s);
	assert(x);
	assert(*x);

	if (MODULE_FUN_NAME(Stack, isEmpty)(s))
		return -1;
	
	s->num--;
	*x = MODULE_FUN_NAME(Array, get)(s->array, s->num);
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

	return 0 == s->num;
}



