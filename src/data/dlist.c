/*************************************
 *
 * filename:	dlist.c
 * description:	implement the new list operations
 * 循环双向链表
 * author:
 * date:		2019-12-19
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dlist.h"

#define T ListD_T

#define NODE ListDNode_T

struct T {
	NODE head;
	int count;
};


/*
 * name: MODULE_FUN_NAME(ListDNode, new)
 * description: create a new node of list
 * return value: return the pointer to NODE
 * args: x: user private data pointer
 */
NODE MODULE_FUN_NAME(ListDNode, new)(void *x)
{
//	assert(x);

	NODE node = (NODE)calloc(1, sizeof(*node));
	if (node)
		node->priv = x;

	return node;
}

/*
 * name: MODULE_FUN_NAME(ListDNode, free)
 * description: free the node $p
 * return value: return void
 * args: @p: pointer to NODE
 */
void MODULE_FUN_NAME(ListDNode, free)(NODE *p)
{
	assert(p);
	assert(*p);

	free(*p);
	*p = NULL;
}

/*
 * name: MODULE_FUN_NAME(ListD, new)
 * description: create a new emtpy list
 * return value: return the pointer to list
 * args: void
 */
T MODULE_FUN_NAME(ListD, new)(void)
{
	T list = (T)calloc(1, sizeof(*list));
	if (list)
	{
		list->head = NULL;
		list->count = 0;
	}

	return list;
}

/*
 * name: MODULE_FUN_NAME(ListD, free)
 * description: free list,
 * return value: void
 * args: the pointer to list
 */
void MODULE_FUN_NAME(ListD, free)(T *list)
{
	NODE node = NULL;

	assert(list);
	assert(*list);

	while (node = MODULE_FUN_NAME(ListD, head)(*list))
	{
		MODULE_FUN_NAME(ListD, remove)(*list, node);
		MODULE_FUN_NAME(ListDNode, free)(&node);
	}
}

/*
 * name: MODULE_FUN_NAME(ListD, insert)
 * description: insert @new to @list's tail,
 * return value: @new
 * args: @list: the pointer to list
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(ListD, insert)(T list, NODE new)
{
	assert(list);
	assert(new);

	if (NULL == list->head)
	{
		list->head = new;
		new->prev = new;
		new->next = new;
		list->count++;
	}
	else
	{
		MODULE_FUN_NAME(ListD, insertBefore)(list, list->head, new);
	}

	return new;
}

/*
 * name: MODULE_FUN_NAME(ListD, insertBefore)
 * description: insert @new before @cur,
 * return value: @new
 * args: @list: the pointer to list
 * 		@cur: current node 
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(ListD, insertBefore)(T list, NODE cur, NODE new)
{
	assert(list);
	assert(cur);
	assert(new);

	new->prev = cur->prev;
	new->next = cur;
	cur->prev->next = new;
	cur->prev = new;

	list->count++;

	return new;
}

/*
 * name: MODULE_FUN_NAME(ListD, insertAfter)
 * description: insert @new after @cur,
 * return value: @new
 * args: @list: the pointer to list
 * 		@cur: current node 
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(ListD, insertAfter)(T list, NODE cur, NODE new)
{
	assert(list);
	assert(cur);
	assert(new);

	new->prev = cur;
	new->next = cur->next;
	cur->next->prev = new;
	cur->next = new;

	list->count++;

	return new;
}

/*
 * name: MODULE_FUN_NAME(ListD, remove)
 * description: remove @old from @list,
 * return value: @old
 * args: @list: the pointer to list
 * 		@old: node to remove 
 */
NODE MODULE_FUN_NAME(ListD, remove)(T list, NODE old)
{
	assert(list);
	assert(list->count > 0);
	assert(old);

	if (1 == list->count)
	{
		assert(list->head == old);
		list->head = NULL;
	}
	else
	{
		if (list->head == old)
			list->head = old->next;

		old->prev->next = old->next;
		old->next->prev = old->prev;
	}

	old->next = NULL;
	old->prev = NULL;

	list->count--;

	return old;
}

/*
 * name: MODULE_FUN_NAME(ListD, prev)
 * description: return the prev node of @x,
 * return value: node prev @x
 * args: @x: the pointer to node
 */
NODE MODULE_FUN_NAME(ListD, prev)(NODE x)
{
	assert(x);

	return x->prev;
}

/*
 * name: MODULE_FUN_NAME(ListD, next)
 * description: return the next node of @x,
 * return value: node next @x
 * args: @x: the pointer to node
 */
NODE MODULE_FUN_NAME(ListD, next)(NODE x)
{
	assert(x);

	return x->next;
}

/*
 * name: MODULE_FUN_NAME(ListD, head)
 * description: return the head node of list ,
 * return value: node of list
 * args: @list: the pointer to list
 */
NODE MODULE_FUN_NAME(ListD, head)(T list)
{
	assert(list);

	return list->head;
}

/*
 * name: MODULE_FUN_NAME(ListD, tail)
 * description: return the tail node of list ,
 * return value: node of list
 * args: @list: the pointer to list
 */
NODE MODULE_FUN_NAME(ListD, tail)(T list)
{
	assert(list);

	return list->head ? list->head->prev : NULL;
}

NODE MODULE_FUN_NAME(ListD, get)(T list, int index)
{
	assert(list);

	NODE node = NULL;

	if (index < 0 || index > list->count - 1)
	{
		return NULL;
	}

	node = MODULE_FUN_NAME(ListD, head)(list);
	for (; index > 0; index--)
	{
		node = MODULE_FUN_NAME(ListD, next)(node);
	}

	return node;
}

NODE MODULE_FUN_NAME(ListD, search)(T list, int (*cmp)(void *priv, void *arg), void *arg)
{
	assert(list && cmp);

	int i;
	NODE node = NULL;

	node = MODULE_FUN_NAME(ListD, head)(list);
	for (i = 0; i < list->count; i++)
	{
		if (cmp(node->priv, arg) == 0)
		{
			break;
		}
		node = MODULE_FUN_NAME(ListD, next)(node);
	}

	/*
	 * 链表为循环链表
	 */
	if (i < list->count)
	{
		return node;
	}
	else
	{
		return NULL;
	}
}

/*
 * name: MODULE_FUN_NAME(ListD, count)
 * description: return the count of list ,
 * return value: count of nodes in list >= 0
 * 				-1  error
 * args: @list: the pointer to list
 */
int MODULE_FUN_NAME(ListD, count)(T list)
{
	assert(list);

	return list->count;
}

/*
 * name: MODULE_FUN_NAME(ListD, isEmpty)
 * description: is list empty,
 * return value: 1: list is empty
 * 				0: list is not empty
 * args: @list: the pointer to list
 */
int MODULE_FUN_NAME(ListD, isEmpty)(T list)
{
	assert(list);

	return list->count == 0 ? 1 : 0;
}

/*
 * name: MODULE_FUN_NAME(ListD, map)
 * description: for each node in @list, call @apply
 * return value: void
 * args: @list: the pointer to list
 * 		@apply: user provided callback, for each node
 * 				will be called, and node's private data
 * 				will be passed as @@x, 
 * 		@cl	:	user's private data for @apply, used as@cl
 */
void MODULE_FUN_NAME(ListD, map)(T list,
				void apply(void **x, void *cl),
				void *cl)
{
	NODE node = NULL;
	int i;

	for (i = MODULE_FUN_NAME(ListD, count)(list), 
				node = MODULE_FUN_NAME(ListD, head)(list); 
				i > 0; 
				i--, node = MODULE_FUN_NAME(ListD, next)(node))
	{
		apply(&node->priv, cl);
	}
}

/*
 * name: MODULE_FUN_NAME(ListD, toArray)
 * description: copy @list node's priv to an array, 
 * 				use @end to indicate end; user should
 *              free the returned array after unuse;
 * return value: the pointer to array; 
 * args: @list: the pointer to list
 * 		@end: indicate end of array
 */
void **MODULE_FUN_NAME(ListD, toArray)(
				T list, void *end)
{
	void **array = NULL;
	NODE node = NULL;
	int count = 0;
	int index = 0;

	count = MODULE_FUN_NAME(ListD, count)(list);
	array = (void **)calloc(1, (count + 1) * sizeof(void *));
	if (array)
	{
		for (node = MODULE_FUN_NAME(ListD, head)(list); 
				count > 0; 
				count--, node = MODULE_FUN_NAME(ListD, next)(node))
		{
			array[index++] = node->priv;
		}

		array[index] = end;
	}

	return array;
}



