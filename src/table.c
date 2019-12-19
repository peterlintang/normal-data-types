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
 * interface: 
 * description:  
 * args: 
 * return value: 
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

	table = (T) calloc(sizeof(*table) + 
			primes[i - 1] * sizeof(table->buckets[0]), sizeof(char));

	table->size = primes[i - 1];
	table->cmp = cmp ? cmp : cmpatom;
	table->hash = hash ? hash : hashatom;
	table->buckets = (struct binding **)(table + 1);
	for (i = 0; i < table->size; i++)
	{
		table->buckets[i] = NULL;
	}
	table->length = 0;
	table->timestamp = 0;

	return table;

}

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

int MODULE_FUN_NAME(Table, length)(T table)
{
	assert(table);

	return table->length;
}

void *MODULE_FUN_NAME(Table, put)
		(T table, const void *key, void *value)
{
	int i;
	struct binding *p = NULL;
	void *prev = NULL;

	assert(table);
	assert(key);

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

void *MODULE_FUN_NAME(Table, get)(T table, const void *key)
{
	int i = -1;
	struct binding *p = NULL;

	assert(table);
	assert(key);

	i = (*table->hash)(key) % table->size;
	for (p = table->buckets[i]; p ; p = p->link)
	{
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}

	return p ? p->value : NULL;
}

void *MODULE_FUN_NAME(Table, remove)(T table, const void *key)
{
	int i;
	struct binding **p = NULL;
	struct binding *tmp = NULL;
	void *value = NULL;

	assert(table);
	assert(key);

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

void MODULE_FUN_NAME(Table, map)(T table, 
		void apply(const void *key, void **value, void *cl),
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
			apply(p->key, &p->value, cl);
			assert(table->timestamp == stamp);
		}
	}
}

void **MODULE_FUN_NAME(Table, toArray)(T table, void *end)
{
	int i;
	int j = 0;
	void **array = NULL;
	struct binding *p = NULL;

	assert(table);

	array = (void **)calloc(2 * table->length + 1 , sizeof(*array));
	for (i = 0; i < table->size; i++)
	{
		for (p = table->buckets[i]; p; p = p->link)
		{
			array[j++] = (void *)p->key;
			array[j++] = p->value;
		}
	}

	array[i] = end;
	return array;
}



