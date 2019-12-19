/*************************************
 *
 * filename:	list2.h
 * description:	implement the new list operations
 * author:
 * date:		2019-12-19
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_NEW_LIST_H__
#define CI_NEW_LIST_H__

#include "module.h"

#define T List_T
typedef struct T *T;

#define NODE ListNode_T
typedef struct NODE *NODE;

struct NODE {
	NODE prev;
	NODE next;
	void *priv;
};

struct T {
	NODE head;
	int count;
};

/*
 * name: MODULE_FUN_NAME(ListNode, new)
 * description: create a new node of list
 * return value: return the pointer to NODE
 * args: x: user private data pointer
 */
NODE MODULE_FUN_NAME(ListNode, new)(void *x);

/*
 * name: MODULE_FUN_NAME(ListNode, free)
 * description: free the node $p
 * return value: return void
 * args: @p: pointer to NODE
 */
void MODULE_FUN_NAME(ListNode, free)(NODE *p);

/*
 * name: MODULE_FUN_NAME(List, new)
 * description: create a new emtpy list
 * return value: return the pointer to list
 * args: void
 */
T MODULE_FUN_NAME(List, new)(void);

/*
 * name: MODULE_FUN_NAME(List, free)
 * description: free list,
 * return value: void
 * args: the pointer to list
 */
void MODULE_FUN_NAME(List, free)(T *list);

/*
 * name: MODULE_FUN_NAME(List, insert)
 * description: insert @new to @list's tail,
 * return value: @new
 * args: @list: the pointer to list
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(List, insert)(T list, NODE new);

/*
 * name: MODULE_FUN_NAME(List, insertBefore)
 * description: insert @new before @cur,
 * return value: @new
 * args: @list: the pointer to list
 * 		@cur: current node 
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(List, insertBefore)(T list, NODE cur, NODE new);

/*
 * name: MODULE_FUN_NAME(List, insertAfter)
 * description: insert @new after @cur,
 * return value: @new
 * args: @list: the pointer to list
 * 		@cur: current node 
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(List, insertAfter)(T list, NODE cur, NODE new);

/*
 * name: MODULE_FUN_NAME(List, remove)
 * description: remove @old from @list,
 * return value: @old
 * args: @list: the pointer to list
 * 		@old: node to remove 
 */
NODE MODULE_FUN_NAME(List, remove)(T list, NODE old);

/*
 * name: MODULE_FUN_NAME(List, prev)
 * description: return the prev node of @x,
 * return value: node prev @x
 * args: @x: the pointer to node
 */
NODE MODULE_FUN_NAME(List, prev)(NODE x);

/*
 * name: MODULE_FUN_NAME(List, next)
 * description: return the next node of @x,
 * return value: node next @x
 * args: @x: the pointer to node
 */
NODE MODULE_FUN_NAME(List, next)(NODE x);

/*
 * name: MODULE_FUN_NAME(List, head)
 * description: return the head node of list ,
 * return value: node of list
 * args: @list: the pointer to list
 */
NODE MODULE_FUN_NAME(List, head)(T list);

/*
 * name: MODULE_FUN_NAME(List, tail)
 * description: return the tail node of list ,
 * return value: node of list
 * args: @list: the pointer to list
 */
NODE MODULE_FUN_NAME(List, tail)(T list);

/*
 * name: MODULE_FUN_NAME(List, count)
 * description: return the count of list ,
 * return value: count of nodes in list >= 0
 * 				-1  error
 * args: @list: the pointer to list
 */
int MODULE_FUN_NAME(List, count)(T list);

/*
 * name: MODULE_FUN_NAME(List, isEmpty)
 * description: is list empty,
 * return value: 1: list is empty
 * 				0: list is not empty
 * args: @list: the pointer to list
 */
int MODULE_FUN_NAME(List, isEmpty)(T list);

/*
 * name: MODULE_FUN_NAME(List, map)
 * description: for each node in @list, call @apply
 * return value: void
 * args: @list: the pointer to list
 * 		@apply: user provided callback, for each node
 * 				will be called, and node's private data
 * 				will be passed as @@x, 
 * 		@cl	:	user's private data for @apply, used as@cl
 */
void MODULE_FUN_NAME(List, map)(T list,
				void apply(void **x, void *cl),
				void *cl);
/*
 * name: MODULE_FUN_NAME(List, toArray)
 * description: copy @list node's priv to an array, 
 * 				use @end to indicate end; user should
 *              free the returned array after unuse;
 * return value: the pointer to array; 
 * args: @list: the pointer to list
 * 		@end: indicate end of array
 */
void **MODULE_FUN_NAME(T, toArray)(
				T list, void *end);


#undef NODE
#undef T

#endif	// end of CI_NEW_LIST_H__
