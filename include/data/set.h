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
 * name: MODULE_FUN_NAME(Set, new)
 * description: create a new set, 
 * return value: return the pointer to new set
 * args: @hint: number of elements in set suggested
 * 		 @cmp: function to compare two elements
 * 		 @hash: function do hash
 */
T MODULE_FUN_NAME(Set, new)(int hint,
			int cmp(const void *x, const void *y),
			unsigned hash(const void *x));

/*
 * name: MODULE_FUN_NAME(Set, free)
 * description: free a set
 * return value: return void
 * args: @set: pointer's pointer to set
 */
void MODULE_FUN_NAME(Set, free)(T *set);

/*
 * name: MODULE_FUN_NAME(Set, length)
 * description: return total elements in set
 * return value: return total number of elements in set
 * args: @set: pointer to set
 */
int MODULE_FUN_NAME(Set, length)(T set);

/*
 * name: MODULE_FUN_NAME(Set, member)
 * description: judge @member is in @set
 * return value: 1: is a member of set
 * 				 0: not
 * args: @set: pointer to set
 * 		@member: pointer to member
 */
int MODULE_FUN_NAME(Set, is_member)
		(T set, const void *member);
/*
 * name: MODULE_FUN_NAME(Set, put)
 * description: judge @member is in @set
 * return value: 1: put success
 * 				 0: failed to put in set
 * args: @set: pointer to set
 * 		@member: pointer to member
 */
int MODULE_FUN_NAME(Set, put)
		(T set, const void *member);

/*
 * name: MODULE_FUN_NAME(Set, remove)
 * description: remove @member from @set
 * return value: pointer to member: success
 * 				 NULL: failed 
 * args: @set: pointer to set
 * 		@member: pointer to member
 */
void *MODULE_FUN_NAME(Set, remove)
		(T set, const void *member);

/*
 * name: MODULE_FUN_NAME(Set, map)
 * description: for each member in @set, call @apply
 * return value: void
 * args: @set: pointer to set
 * 		@apply: function to call
 * 		@cl: private data to user
 */
void MODULE_FUN_NAME(Set, map)(T set, 
				void apply(const void *member, void *cl),
				void *cl);

/*
 * name: MODULE_FUN_NAME(Set, toArray)
 * description: put members in @set to array
 * return value: array to members
 * args: @set: pointer to set
 * 		@end: indicator end of array
 */
void **MODULE_FUN_NAME(Set, toArray)(T set, void *end);

/*
 * name: MODULE_FUN_NAME(Set, union)
 * description: put members in @s and @t to a new set
 * return value: return a set, which the member is in t or in s 
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, union)(T s, T t);	// s + t

/*
 * name: MODULE_FUN_NAME(Set, inter)
 * description: put members both in @s and @t to a new set
 * return value: return a set, which the member is in t and in s 
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, inter)(T s, T t);	// s * t

/*
 * name: MODULE_FUN_NAME(Set, minus)
 * description: put members in @s and not in @t to a new set
 * return value: return a set, which the member is in t but not in s
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, minus)(T s, T t);	// s - t 

/*
 * name: MODULE_FUN_NAME(Set, diff)
 * description:  put members which only in @s or in @t to a new set
 * return value: return a set, the member only in s or only in t
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, diff)(T s, T t);	// s / t 

#undef T

#endif
