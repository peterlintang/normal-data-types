/*************************************
 *
 * filename:	list2.h
 * description:	implement the new list operations
 * 循环双向链表
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

#define T ListD_T
typedef struct T *T;

#define NODE ListDNode_T
typedef struct NODE *NODE;

struct NODE {
	NODE prev;
	NODE next;
	void *priv; // user's data
};


/*
 * name: MODULE_FUN_NAME(ListDNode, new)
 * description: create a new node of list
 * return value: return the pointer to NODE
 * args: x: user private data pointer
 */
NODE MODULE_FUN_NAME(ListDNode, new)(void *x);

/*
 * name: MODULE_FUN_NAME(ListDNode, free)
 * description: free the node $p
 * return value: return void
 * args: @p: pointer to NODE
 */
void MODULE_FUN_NAME(ListDNode, free)(NODE *p);

/*
 * name: MODULE_FUN_NAME(ListD, new)
 * description: create a new emtpy list
 * return value: return the pointer to list
 * args: void
 */
T MODULE_FUN_NAME(ListD, new)(void);

/*
 * name: MODULE_FUN_NAME(ListD, free)
 * description: free list,
 * return value: void
 * args: the pointer to list
 */
void MODULE_FUN_NAME(ListD, free)(T *list);

/*
 * name: MODULE_FUN_NAME(ListD, insert)
 * description: insert @new to @list's tail,
 * return value: @new
 * args: @list: the pointer to list
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(ListD, insert)(T list, NODE new);

/*
 * name: MODULE_FUN_NAME(ListD, insertBefore)
 * description: insert @new before @cur,
 * return value: @new
 * args: @list: the pointer to list
 * 		@cur: current node 
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(ListD, insertBefore)(T list, NODE cur, NODE new);

/*
 * name: MODULE_FUN_NAME(ListD, insertAfter)
 * description: insert @new after @cur,
 * return value: @new
 * args: @list: the pointer to list
 * 		@cur: current node 
 * 		@new: new node to insert
 */
NODE MODULE_FUN_NAME(ListD, insertAfter)(T list, NODE cur, NODE new);

/*
 * name: MODULE_FUN_NAME(ListD, remove)
 * description: remove @old from @list,
 * return value: @old
 * args: @list: the pointer to list
 * 		@old: node to remove 
 */
NODE MODULE_FUN_NAME(ListD, remove)(T list, NODE old);

/*
 * name: MODULE_FUN_NAME(ListD, prev)
 * description: return the prev node of @x,
 * return value: node prev @x
 * args: @x: the pointer to node
 */
NODE MODULE_FUN_NAME(ListD, prev)(NODE x);

/*
 * name: MODULE_FUN_NAME(ListD, next)
 * description: return the next node of @x,
 * return value: node next @x
 * args: @x: the pointer to node
 */
NODE MODULE_FUN_NAME(ListD, next)(NODE x);

/*
 * name: MODULE_FUN_NAME(ListD, head)
 * description: return the head node of list ,
 * return value: node of list
 * args: @list: the pointer to list
 */
NODE MODULE_FUN_NAME(ListD, head)(T list);

/*
 * name: MODULE_FUN_NAME(ListD, tail)
 * description: return the tail node of list ,
 * return value: node of list
 * args: @list: the pointer to list
 */
NODE MODULE_FUN_NAME(ListD, tail)(T list);

/*
 * 根据index返回链表中相应序号的元素
 * index超过count，返回NULL
 */
NODE MODULE_FUN_NAME(ListD, get)(T list, int index);

/*
 * 根据key在链表中查找元素，找到返回指针，没有返回NULL
 */
NODE MODULE_FUN_NAME(ListD, search)(T list, int (*cmp)(void *, void *), void *key);

/*
 * name: MODULE_FUN_NAME(ListD, count)
 * description: return the count of list ,
 * return value: count of nodes in list >= 0
 * 				-1  error
 * args: @list: the pointer to list
 */
int MODULE_FUN_NAME(ListD, count)(T list);

/*
 * name: MODULE_FUN_NAME(ListD, isEmpty)
 * description: is list empty,
 * return value: 1: list is empty
 * 				0: list is not empty
 * args: @list: the pointer to list
 */
int MODULE_FUN_NAME(ListD, isEmpty)(T list);

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
				void *cl);
/*
 * name: MODULE_FUN_NAME(ListD, toArray)
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
