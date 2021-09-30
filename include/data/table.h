/*************************************
 *
 * filename:	table.h
 * description:	implement the table operations
 * author:
 * date:		2019-09-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_TABLE_INCLUDED
#define CI_TABLE_INCLUDED

#include "module.h"

#define T Table_T
typedef struct T *T;

/*
 * name: MODULE_FUN_NAME(Table, new)
 * description: create a new table
 * return value: return pointer to table
 * args: @hint: suggested size(how many elements have)
 * 		 @cmp: function to compare
 * 		 @hash: function to do hash
 */
T MODULE_FUN_NAME(Table, new)(int hint,
			int cmp(const void *x, const void *y),
			unsigned hash(const void *key));

/*
 * name: MODULE_FUN_NAME(Table, free)
 * description: free a table
 * return value: return void
 * args: @table: pointer's pointer to table
 */
void MODULE_FUN_NAME(Table, free)(T *table);

/*
 * name: MODULE_FUN_NAME(Table, length)
 * description: return the total elements in table
 * return value: return the total elements in table
 * args: @table: pointer to table
 */
int MODULE_FUN_NAME(Table, length)(T table);

/*
 * name: MODULE_FUN_NAME(Table, put)
 * description: put @value @ @key to table
 * return value: return prev value of key or null
 * args: @table: pointer to table
 * 		 @key: key
 * 		 @value: value to put
 */
void *MODULE_FUN_NAME(Table, put)
		(T table, const void *key, void *value);

/*
 * name: MODULE_FUN_NAME(Table, get)
 * description: get value by @key in table
 * return value: return value of key or null
 * args: @table: pointer to table
 * 		 @key: key
 */
void *MODULE_FUN_NAME(Table, get)(T table, const void *key);

/*
 * name: MODULE_FUN_NAME(Table, remove)
 * description: remove element by @key in table
 * return value: return value of key or null
 * args: @table: pointer to table
 * 		 @key: key
 */
void *MODULE_FUN_NAME(Table, remove)(T table, const void *key);

/*
 * name: MODULE_FUN_NAME(Table, map)
 * description: for each elements in table, call @apply
 * return value: return void
 * args: @table: pointer to table
 * 		 @apply: function to call
 * 		 @cl: user's data
 */
void MODULE_FUN_NAME(Table, map)(T table, 
		int apply(const void *key, void **value, void *cl),
		void *cl);
/*
 * name: MODULE_FUN_NAME(Table, toArray)
 * description: put all elements in table to array, which is end of @end
 * return value: return array's pointer
 * args: @table: pointer to table
 * 		 @end: indicator of end of table
 */
void **MODULE_FUN_NAME(Table, toArray)(T table, void *end);

#undef T

#endif
