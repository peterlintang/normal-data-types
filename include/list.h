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

/*
#define T List_T
typedef struct T *T;

struct T {
	T rest;
	void *first;
};
*/

struct List_T {
	struct List_T *rest;
	void *first;
};

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
struct List_T *MODULE_FUN_NAME(List, append)(
			struct List_T *list, 
			struct List_T *tail);
//struct List_T *List_append(struct List_T *list, struct List_T *tail);

/* 
 * interface: 
 * description: 
 * args:	
 * return value: 
 */
struct List_T *MODULE_FUN_NAME(List, copy)(
				struct List_T *list);
//struct List_T *List_copy(struct List_T *list);

/* 
 * interface: 
 * description: 
 * args: 
 * return value: 
 */
struct List_T *MODULE_FUN_NAME(List, list)(void *x, ...);
//struct List_T *List_list(void *x, ...);

/* 
 * interface: 
 * description: 
 * args:	
 * return value: 
 */
struct List_T *MODULE_FUN_NAME(List, pop)(
				struct List_T *list, void **x);
//struct List_T *List_pop(struct List_T *list, void **x);

struct List_T *MODULE_FUN_NAME(List, push)(
				struct List_T *list, void *x);
//struct List_T *List_push(struct List_T *list, void *x);

struct List_T *MODULE_FUN_NAME(List, reverse)(
				struct List_T *list);
//struct List_T *List_reverse(struct List_T *list);

int MODULE_FUN_NAME(List, length)(
				struct List_T *list);
//int List_length(struct List_T *list);

void MODULE_FUN_NAME(List, free)(
				struct List_T **list);
//struct List_T *List_free(struct List_T *list);

void MODULE_FUN_NAME(List, map)(
				struct List_T *list, 
			void apply(void **x, void *cl), void *cl);
//void *List_map(struct List_T *list, 
//			void apply(void **x, void *cl), void *cl);

void **MODULE_FUN_NAME(List, toArray)(
				struct List_T *list, void *end);

//void **List_toArray(struct List_T *list, void *end);

/*
#undef T
*/

#endif
