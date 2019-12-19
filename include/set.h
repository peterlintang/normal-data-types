/*************************************
 *
 * filename:	set.h
 * description:	implement the set operations
 * author:
 * date:		2019-09-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_SET_INCLUDED
#define CI_SET_INCLUDED

#include "module.h"

#define T Set_T
typedef struct T *T;

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
// < functions 138 >
extern T MODULE_FUN_NAME(Set, new)(int hint,
			int cmp(const void *x, const void *y),
			unsigned hash(const void *x));
extern void MODULE_FUN_NAME(Set, free)(T *set);

extern int MODULE_FUN_NAME(Set, length)(T set);
extern int MODULE_FUN_NAME(Set, member)
		(T set, const void *member);
extern int MODULE_FUN_NAME(Set, put)
		(T set, const void *member);
extern void *MODULE_FUN_NAME(Set, remove)
		(T set, const void *member);

extern void MODULE_FUN_NAME(Set, map)(T set, 
				void apply(const void *member, void *cl),
				void *cl);
extern void **MODULE_FUN_NAME(Set, toArray)(T set, void *end);

extern T MODULE_FUN_NAME(Set, union)(T s, T t);	// s + t
extern T MODULE_FUN_NAME(Set, inter)(T s, T t);	// s * t
extern T MODULE_FUN_NAME(Set, minus)(T s, T t);	// s - t 
extern T MODULE_FUN_NAME(Set, diff)(T s, T t);	// s / t 

#undef T

#endif
