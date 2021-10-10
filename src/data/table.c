/*************************************
 *
 * filename:	table.c
 * description:	implement the table operations
 * author:
 * date:		2019-09-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <limits.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#include "table.h"

#define T Table_T

struct T {
	int length;
	unsigned timestamp;
	int size;
	int (*cmp)(const void *x, const void *y);
	unsigned (*hash)(const void *key);
	struct binding {
		struct binding *link;
		const void *key;
		void *value;
	} **buckets;
};


static int cmpatom(const void *x, const void *y)
{
	return x != y;
}

static unsigned hashatom(const void *key)
{
	return (unsigned long)key >> 2;
}

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
			unsigned hash(const void *key))
{
	T table;
	int i;
	static int primes[] = { 
			509, 509, 1021, 2053,
			4093, 8191, 16381, 32771,
			65521, INT_MAX};

	assert(hint >= 0);

	for (i = 1; primes[i] < hint; i++)
		;

	table = (T) calloc(1, sizeof(*table));
	if (table == NULL)
	{
		return NULL;
	}

	table->buckets = (struct binding **) calloc(1, (primes[i - 1] * sizeof(table->buckets[0])) * sizeof(char));
	if (table->buckets == NULL)
	{
		free(table);
		return NULL;
	}

	table->size = primes[i - 1];
	table->cmp = cmp ? cmp : cmpatom;
	table->hash = hash ? hash : hashatom;
//	table->buckets = (struct binding **)(table + 1);
	for (i = 0; i < table->size; i++)
	{
		table->buckets[i] = NULL;
	}
	table->length = 0;
	table->timestamp = 0;

	return table;

}

/*
 * name: MODULE_FUN_NAME(Table, free)
 * description: free a table
 * return value: return void
 * args: @table: pointer's pointer to table
 */
void MODULE_FUN_NAME(Table, free)(T *table)
{
	assert(table);
	assert(*table);

	if ((*table)->length > 0)
	{
		int i;
		struct binding *p = NULL;
		struct binding *q = NULL;
		for (i = 0; i < (*table)->size; i++)
		{
			for (p = (*table)->buckets[i]; p; p = q)
			{
				q = p->link;
				free(p);
			}
		}
	}

	free(*table);
	*table = NULL;
}

/*
 * name: MODULE_FUN_NAME(Table, length)
 * description: return the total elements in table
 * return value: return the total elements in table
 * args: @table: pointer to table
 */
int MODULE_FUN_NAME(Table, length)(T table)
{
	assert(table);

	return table->length;
}

/*
 * name: MODULE_FUN_NAME(Table, put)
 * description: put @value @ @key to table
 * return value: return prev value of key or null
 * args: @table: pointer to table
 * 		 @key: key
 * 		 @value: value to put
 */
void *MODULE_FUN_NAME(Table, put)
		(T table, const void *key, void *value)
{
	int i;
	struct binding *p = NULL;
	void *prev = NULL;

	assert(table);
//	assert(key);

	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p ; p = p->link)
	{
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}

	if (NULL == p)
	{
		p = (struct binding *)calloc(1, sizeof(*p));
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length++;
	}
	else
	{
		prev = p->value;
	}

	p->value = value;
	table->timestamp++;
	return prev;
}

/*
 * name: MODULE_FUN_NAME(Table, get)
 * description: get value by @key in table
 * return value: return value of key or null
 * args: @table: pointer to table
 * 		 @key: key
 */
void *MODULE_FUN_NAME(Table, get)(T table, const void *key)
{
	int i = -1;
	struct binding *p = NULL;

	assert(table);
//	assert(key);

	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p ; p = p->link)
	{
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}

	return p ? p->value : NULL;
}

/*
 * name: MODULE_FUN_NAME(Table, remove)
 * description: remove element by @key in table
 * return value: return value of key or null
 * args: @table: pointer to table
 * 		 @key: key
 */
void *MODULE_FUN_NAME(Table, remove)(T table, const void *key)
{
	int i;
	struct binding **p = NULL;
	struct binding *tmp = NULL;
	void *value = NULL;

	assert(table);
//	assert(key);

	table->timestamp++;

	i = ((*table->hash)(key)) % (table->size);
	for (p = &table->buckets[i]; *p ; p = &(*p)->link)
	{
		if ((*table->cmp)(key, (*p)->key) == 0)
		{

			tmp = *p;
			value = tmp->value;
			*p = tmp->link;
			free(tmp);
			table->length--;
			return value;
		}
	}

	return NULL;
}

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
		void *cl)
{
	int i;
	unsigned stamp;
	struct binding *p = NULL;

	assert(table);
	assert(apply);

	stamp = table->timestamp;

	for(i = 0; i < table->size; i++)
	{
		for (p = table->buckets[i]; p; p = p->link)
		{
			if (apply(p->key, &p->value, cl) != 0) 
			{
				return ;
			}
			assert(table->timestamp == stamp);
		}
	}
}

/*
 * name: MODULE_FUN_NAME(Table, toArray)
 * description: put all elements in table to array, which is end of @end
 * return value: return array's pointer
 * args: @table: pointer to table
 * 		 @end: indicator of end of table
 */
void **MODULE_FUN_NAME(Table, toArray)(T table, void *end)
{
	int i;
	int j = 0;
	void **array = NULL;
	struct binding *p = NULL;

	assert(table);

	array = (void **)calloc(1, (2 * table->length + 1) * sizeof(*array));
	for (i = 0; i < table->size; i++)
	{
		for (p = table->buckets[i]; p; p = p->link)
		{
			array[j++] = (void *)p->key;
			array[j++] = p->value;
		}
	}

	array[j] = end;
	return array;
}



