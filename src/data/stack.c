

/*
 * 10-1 
 * implement the stack described by this sector
 */

#include <stdlib.h>
#include <assert.h>

#include "stack.h"

#define T Stack_T

struct Stack_T {
	int size;
	int top;
	void **array;
};


/*
 * 判断堆栈是否为空，是返回1，否返回0
 * sp: 堆栈的指针
 */
int MODULE_FUN_NAME(Stack, isEmpty)(T s)
{
	assert(s);

	if (s->top == 0)
		return 1;
	else
		return 0;
}

/*
 * 往堆栈压入数据, 成功返回0，失败返回-1
 * sp: 堆栈的指针
 * x: 待压入的数据
 */
int MODULE_FUN_NAME(Stack, push)(T s, void *x)
{
	assert(s);

	if (s->top >= s->size)
	{
		return -1;
	}

	s->array[s->top] = x;
	s->top = s->top + 1;

	return 0;
}

/*
 * 从堆栈取出数据, 成功返回0，失败返回-1
 * sp: 堆栈的指针
 * xp: 存放数据指针的地方
 */
int MODULE_FUN_NAME(Stack, pop)(T s, void **xp)
{
	assert(s && xp);

	if (MODULE_FUN_NAME(Stack, isEmpty)(s))
	{
		return -1;
	}

	s->top = s->top - 1;
	*xp = s->array[s->top];
	return 0;
}


/*
 * 初始化一个堆栈,成功返回指针，失败返回空指针
 * size: 堆栈大小
 */
T MODULE_FUN_NAME(Stack, new)(int size)
{
	T s = NULL;

	assert(size > 0);

	s = (T)calloc(1, sizeof(*s));
	if (s)
	{
		s->array = (void **)calloc(1, size * sizeof(void *));
		if (s->array)
		{
			s->size = size;
		}
		else
		{
			free(s);
			s = NULL;
		}
	}

	return s;
}

/*
 * 销毁一个堆栈结构
 * sp: 堆栈的指针
 */
void MODULE_FUN_NAME(Stack, free)(T *sp)
{
	assert(sp && *sp);

	free((*sp)->array);
	free(*sp);
	*sp = NULL;
}



