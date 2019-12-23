/*************************************
 *
 * filename:	list.c
 * description:	implement the list operations
 * author:
 * date:		2019-09-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include "module.h"
#include "list.h"

#define T List_T

T MODULE_FUN_NAME(List, append)
		(T list, T tail)
//T List_append(T list, T tail)
{
	T *p = &list;

	while (*p)
	{
		p = &(*p)->rest;
	}

	*p = tail;

	return list;
}

T MODULE_FUN_NAME(List, copy)
		(T list)
//T List_copy(T list)
{
	T head = NULL;
	T *p = NULL;

	p = &head;
	for (; list; list = list->rest)
	{
		*p = (T )calloc(1, sizeof(**p));
		(*p)->first = list->first;
		p = &(*p)->rest;
	}

	*p = NULL;
	return head;
}

T MODULE_FUN_NAME(List, list)
		(void *x, ...)
//T List_list(void *x, ...)
{
	va_list ap;
	T list = NULL;
	T *p = NULL;	// a good example to use pointer's pointer
	T item = NULL;

	p = &list;
	va_start(ap, x);
	for (; x; x = va_arg(ap, void *))
	{
		*p = (T )calloc(1, sizeof(**p));
		if (NULL == *p)
		{
			goto error;
		}
		(*p)->first = x;
		p = &(*p)->rest;
	}

	*p = NULL;
	va_end(ap);
	return list;

error:

	while (list)
	{
		item = list;
		list = item->rest;
		free(item);
	}

	va_end(ap);
	return NULL;
}

T MODULE_FUN_NAME(List, pop)
			(T list, void **x)
//T List_pop(T list, void **x)
{
	T head = NULL;
	
	if (list)
	{
		head = list->rest;
		if (x)
			*x = list->first;
		free(list);
		return head;
	}
	else
	{
		return list;
	}
}

T MODULE_FUN_NAME(List, push)
			(T list, void *x)
//T List_push(T list, void *x)
{
	T p = NULL;

	assert(list);
	assert(x);

	p = (T )calloc(1, sizeof(*p));
	if (NULL != p)
	{
		p->first = x;
		p->rest = list;
	}
	
	return p;
}

T MODULE_FUN_NAME(List, reverse)
	(T list)
//T List_reverse(T list)
{
	T head = NULL;
	T next = NULL;

	for (; list; list = next)
	{
		next = list->rest;
		list->rest = head;
		head = list;
	}

	return head;
}

int MODULE_FUN_NAME(List, length)
		(T list)
//int List_length(T list)
{
	int n;

	assert(list);

	for (n = 0; list; list = list->rest)
		n++;
	return n;
}

void MODULE_FUN_NAME(List, free)
		(T *list)
//void List_free(T *list)
{
	T next = NULL;

	assert(list);

	while (*list)
	{
		next = (*list)->rest;
		free(*list);
		*list = next;
	}

	*list = NULL;
}

void MODULE_FUN_NAME(List, map)
			(T list, 
				void apply(void **x, void *cl), void *cl)
//void *List_map(T list, 
//			void apply(void **x, void *cl), void *cl)
{
	assert(apply);

	for (; list; list = list->rest)
	{
		apply(&list->first, cl);
	}
}

void **MODULE_FUN_NAME(List, toArray)
		(T list, void *end)
//void **List_toArray(T list, void *end)
{
	int i;
	int n;
	void **array = NULL;

	assert(list);

	n = MODULE_FUN_NAME(List, length)(list);
	array = (void **)calloc(sizeof(*array), (n + 1));

	for (i = 0; i < n; i++)
	{
		array[i] = list->first;
		list = list->rest;
	}

	array[i] = end;

	return array;
}


