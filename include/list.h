/*************************************
 *
 * filename:	list.h
 * description:	implement the list operations
 * author:
 * date:		2019-09-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_LIST_INCLUDED
#define CI_LIST_INCLUDED

#include "module.h"


#define T List_T
typedef struct T *T;

struct T {
	T rest;
	void *first;
};


/*
 * name: MODULE_FUN_NAME(List, append)
 * description: append @tail to @list
 * return value: return a  new list
 * args: @list: pointer to list
 * 		 @tail: pointer to list
 */
T MODULE_FUN_NAME(List, append)(
			T list, 
			T tail);

/*
 * name: MODULE_FUN_NAME(List, copy)
 * description: copy list @list
 * return value: return a copied list
 * args: @list: pointer to list
 */
T MODULE_FUN_NAME(List, copy)(
				T list);

/*
 * name: MODULE_FUN_NAME(List, list)
 * description: create a new list by args
 * return value: return a new list
 * args: @x: args
 */
T MODULE_FUN_NAME(List, list)(void *x, ...);

/*
 * name: MODULE_FUN_NAME(List, pop)
 * description: pop list's head element from list
 * return value: return a poped list
 * args: @list: pointer to list
 * 		 @x: pointer's pointer to store the poped elements
 */
T MODULE_FUN_NAME(List, pop)(
				T list, void **x);

/*
 * name: MODULE_FUN_NAME(List, push)
 * description: push @x to the head of list
 * return value: return a pushed list
 * args: @list: pointer to list
 * 		 @x: value to push
 */
T MODULE_FUN_NAME(List, push)(
				T list, void *x);

/*
 * name: MODULE_FUN_NAME(List, reverse)
 * description: reverse the list
 * return value: return a reversed list
 * args: @list: pointer to list
 */
T MODULE_FUN_NAME(List, reverse)(
				T list);

/*
 * name: MODULE_FUN_NAME(List, length)
 * description: return the total elements in list
 * return value: return the total elements in list
 * args: @list: pointer to list
 */
int MODULE_FUN_NAME(List, length)(
				T list);

/*
 * name: MODULE_FUN_NAME(List, free)
 * description: free list
 * return value: return void
 * args: @list: pointer to list
 */
void MODULE_FUN_NAME(List, free)(
				T *list);

/*
 * name: MODULE_FUN_NAME(List, map)
 * description: for each elements in list, call @apply
 * return value: return void
 * args: @list: pointer to list
 * 		 @apply: function to call
 */
void MODULE_FUN_NAME(List, map)(
				T list, 
			void apply(void **x, void *cl), void *cl);

/*
 * name: MODULE_FUN_NAME(List, toArray)
 * description: put all elements in list to array
 * return value: return array
 * args: @list: pointer to list
 * 		 @end: indicator of end of array
 */
void **MODULE_FUN_NAME(List, toArray)(
				T list, void *end);


#undef T

#endif
