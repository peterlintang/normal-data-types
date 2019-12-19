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
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
T MODULE_FUN_NAME(Table, new)(int hint,
			int cmp(const void *x, const void *y),
			unsigned hash(const void *key));
void MODULE_FUN_NAME(Table, free)(T *table);
int MODULE_FUN_NAME(Table, length)(T table);
void *MODULE_FUN_NAME(Table, put)
		(T table, const void *key, void *value);
void *MODULE_FUN_NAME(Table, get)(T table, const void *key);
void *MODULE_FUN_NAME(Table, remove)(T table, const void *key);
void MODULE_FUN_NAME(Table, map)(T table, 
		void apply(const void *key, void **value, void *cl),
		void *cl);
void **MODULE_FUN_NAME(Table, toArray)(T table, void *end);

#undef T

#endif
