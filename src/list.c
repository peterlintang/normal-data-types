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

struct List_T *MODULE_FUN_NAME(List, append)
		(struct List_T *list, struct List_T *tail)
//struct List_T *List_append(struct List_T *list, struct List_T *tail)
{
	struct List_T **p = &list;

	while (*p)
	{
		p = &(*p)->rest;
	}

	*p = tail;

	return list;
}

struct List_T *MODULE_FUN_NAME(List, copy)
		(struct List_T *list)
//struct List_T *List_copy(struct List_T *list)
{
	struct List_T *head = NULL;
	struct List_T **p = NULL;

	p = &head;
	for (; list; list = list->rest)
	{
		*p = (struct List_T *)calloc(1, sizeof(**p));
		(*p)->first = list->first;
		p = &(*p)->rest;
	}

	*p = NULL;
	return head;
}

struct List_T *MODULE_FUN_NAME(List, list)
		(void *x, ...)
//struct List_T *List_list(void *x, ...)
{
	va_list ap;
	struct List_T *list = NULL;
	struct List_T **p = NULL;	// a good example to use pointer's pointer
	struct List_T *item = NULL;

	p = &list;
	va_start(ap, x);
	for (; x; x = va_arg(ap, void *))
	{
		*p = (struct List_T *)calloc(1, sizeof(**p));
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

struct List_T *MODULE_FUN_NAME(List, pop)
			(struct List_T *list, void **x)
//struct List_T *List_pop(struct List_T *list, void **x)
{
	struct List_T *head = NULL;
	
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

struct List_T *MODULE_FUN_NAME(List, push)
			(struct List_T *list, void *x)
//struct List_T *List_push(struct List_T *list, void *x)
{
	struct List_T *p = NULL;

	assert(list);
	assert(x);

	p = (struct List_T *)calloc(1, sizeof(*p));
	if (NULL != p)
	{
		p->first = x;
		p->rest = list;
	}
	
	return p;
}

struct List_T *MODULE_FUN_NAME(List, reverse)
	(struct List_T *list)
//struct List_T *List_reverse(struct List_T *list)
{
	struct List_T *head = NULL;
	struct List_T *next = NULL;

	for (; list; list = next)
	{
		next = list->rest;
		list->rest = head;
		head = list;
	}

	return head;
}

int MODULE_FUN_NAME(List, length)
		(struct List_T *list)
//int List_length(struct List_T *list)
{
	int n;

	assert(list);

	for (n = 0; list; list = list->rest)
		n++;
	return n;
}

void MODULE_FUN_NAME(List, free)
		(struct List_T **list)
//void List_free(struct List_T **list)
{
	struct List_T *next = NULL;

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
			(struct List_T *list, 
				void apply(void **x, void *cl), void *cl)
//void *List_map(struct List_T *list, 
//			void apply(void **x, void *cl), void *cl)
{
	assert(apply);

	for (; list; list = list->rest)
	{
		apply(&list->first, cl);
	}
}

void **MODULE_FUN_NAME(List, toArray)
		(struct List_T *list, void *end)
//void **List_toArray(struct List_T *list, void *end)
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


