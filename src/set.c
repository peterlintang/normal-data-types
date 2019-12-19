

#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <assert.h>

#include "set.h"

#define T Set_T

// < types 149 >>
struct T {
	int length;
	unsigned timestamp;
	int (*cmp)(const void *x, const void *y);
	unsigned (*hash)(const void *x);
	int size;
	struct member {
		struct member *link;
		const void *member;
	} **buckets;
};

// <static functions 150>
static int cmpatom(const void *x, const void *y)
{
	return x != y;
}

static unsigned hashatom(const void *x)
{
	return (unsigned long)x >> 2;
}

static T copy(T t, int hint)
{
	int i;
	int j;
	T set = NULL;
	struct member *p = NULL;
	struct member *q = NULL;

	assert(t);

	set = MODULE_FUN_NAME(Set, new)(hint, t->cmp, t->hash);
	if (NULL == set)
		return set;

	for (i = 0; i < t->size; i++)
	{
		for (q = t->buckets[i]; q; q = q->link)
		{
			const void *member = q->member;
			j = (*set->hash)(member) % set->size;

			p = (struct member *)calloc(1, sizeof(*p));
			if (p)
			{
				p->member = member;
				p->link = set->buckets[j];
				set->buckets[j] = p->link;
				set->length++;
			}
			else
			{
				MODULE_FUN_NAME(Set, free)(&set);
				return NULL;
			}
		}
	}

	return set;
}


// < functions 149>
T MODULE_FUN_NAME(Set, new)(int hint, 
				int cmp(const  void *x, const void *y),
				unsigned hash(const void *x))
{
	T set = NULL;
	int i;
	static int primes[] = {
		509, 509, 1021, 2053, 4093,
		8191, 16381, 32771, 65521, INT_MAX
	};

	assert(hint >= 0);

	for (i = 1; primes[i] < hint; i++) 
		;

	set = (T)calloc(1, sizeof(*set) + 
					primes[i - 1] * sizeof(set->buckets[0]));
	if (set)
	{
		set->size = primes[i - 1];
		set->cmp = cmp ? cmp : cmpatom;
		set->hash = hash ? hash : hashatom;
		set->buckets = (struct member **)(set + 1);
		for (i = 0; i < set->size; i++)
			set->buckets[i] = NULL;
		set->length = 0;
		set->timestamp = 0;
	}

	return set;
}

int MODULE_FUN_NAME(Set, member)(T set, const void *member)
{
	int i;
	struct member *p = NULL;

	assert(set);
	assert(member);

	// < search set for member 151>
	i = (*set->hash)(member) % set->size;
	for (p = set->buckets[i]; p; p = p->link)
		if ((*set->cmp)(member, p->member) == 0)
			break;

	return p != NULL;
}

int MODULE_FUN_NAME(Set, put)(T set, const void *member)
{
	int i;
	struct member *p = NULL;

	assert(set);
	assert(member);

	i = (*set->hash)(member) % set->size;
	for (p = set->buckets[i]; p; p = p->link)
		if ((*set->cmp)(member, p->member) == 0)
			break;

	if (NULL == p)
	{
		p = (struct member *)calloc(1, sizeof(*p));
		if (p)
		{
			p->member = member;
			p->link = set->buckets[i];
			set->buckets[i] = p;
			set->length++;
		}
	}
	else
	{
		p->member = member;
	}

	set->timestamp++;

	if (p)
		return 1;
	else
		return 0;
}

void *MODULE_FUN_NAME(Set, remove)(T set, const void *member)
{
	int i;
	struct member *p = NULL;
	struct member **pp = NULL;

	assert(set);
	assert(member);

	set->timestamp++;
	i = (*set->hash)(member) % set->size;
	for (pp = &set->buckets[i]; *pp; pp = &(*pp)->link)
	{
		if ((*set->cmp)(member, (*pp)->member) == 0)
		{
			p = *pp;
			*pp = p->link;
			member = p->member;
			free(p);
			set->length--;
			return (void *)member;
		}
	}

	return NULL;
}

int MODULE_FUN_NAME(Set, length)(T set)
{
	assert(set);

	return set->length;
}

void MODULE_FUN_NAME(Set, free)(T *set)
{
	int i;
	struct member *p = NULL;
	struct member *q = NULL;

	assert(set && *set);

	if ((*set)->length > 0)
	{
		for (i = 0; i < (*set)->length; i++)
		{
			for (p = (*set)->buckets[i]; p; p = q)
			{
				q = p->link;
				free(p);
			}
		}
	}

	free(*set);
	*set = NULL;
}

void MODULE_FUN_NAME(Set, map)
	(T set, void apply(const void *member, void *cl), void *cl)
{
	int i;
	unsigned stamp = 0;
	struct member *p = NULL;

	assert(set);
	assert(apply);

	stamp = set->timestamp;
	for (i = 0; i < set->size; i++)
	{
		for (p = set->buckets[i]; p; p = p->link)
		{
			apply(p->member, cl);
			assert(set->timestamp == stamp);
		}
	}
}

void **MODULE_FUN_NAME(Set, toArray)(T set, void *end)
{
	int i;
	int j = 0;
	void **array = NULL;
	struct member *p = NULL;

	assert(set);

	array = (void **)calloc(set->length + 1 , sizeof(*array));
	for (i = 0; i < set->size; i++)
	{
		for (p = set->buckets[i]; p; p = p->link)
		{
			array[j++] = (void *)p->member;
		}
	}
	array[j] = end;

	return array;
}

/* return a set, which the member is in t or in s */
T MODULE_FUN_NAME(Set, union)(T s, T t)
{
	int i;
	struct member *p = NULL;
	T set = NULL;
	if (NULL == s)
	{
		assert(t);

		return copy(t, t->size);
	}
	else if (NULL == t)
	{
		return copy(s, s->size);
	}
	else
	{
		set = copy(s, s->size > t->size ?  s->size : t->size);

		assert(s->cmp == t->cmp && s->hash == t->hash);

		for (i = 0; i < t->size; i++)
		{
			for (p = t->buckets[i]; p; p = p->link)
			{
				MODULE_FUN_NAME(Set, put)(set, p->member);
			}
		}

		return set;
	}
}

/* return a set, which the member is in t and in s */
T MODULE_FUN_NAME(Set, inter)(T s, T t)
{
	int i;
	struct member *q = NULL;
	T set = NULL;

	if (NULL == s)
	{
		assert(t);

		return MODULE_FUN_NAME(Set, new)
				(t->size, t->cmp, t->hash);
	}
	else if (NULL == t)
	{
		return MODULE_FUN_NAME(Set, new)
				(s->size, s->cmp, s->hash);
	}
	else if (s->length < t->length)
	{
		return MODULE_FUN_NAME(Set, inter)(t, s);
	}
	else
	{
		set = MODULE_FUN_NAME(Set, new)
				(s->size < t->size ? s->size : t->size, 
						s->cmp, s->hash);
		if (NULL == set)
			return NULL;

		assert(s->cmp == t->cmp && s->hash == t->hash);

		for (i = 0; i < t->size; i++)
		{
			for (q = t->buckets[i]; q; q = q->link)
			{
				if (MODULE_FUN_NAME(Set, member)(s, q->member))
				{
					MODULE_FUN_NAME(Set, put)(set, q->member);
				}
			}
		}

		return set;
	}
}

/* return a set, which the member is in t but not in s */
T MODULE_FUN_NAME(Set, minus)(T t, T s)
{
	int i;
	struct member *q = NULL;
	T set = NULL;

	if (NULL == t)
	{
		assert(s);
		return MODULE_FUN_NAME(Set, new)(s->size, s->cmp, s->hash);
	}
	else if (NULL == s)
	{
		return copy(t, t->size);
	}
	else
	{
		set = MODULE_FUN_NAME(Set, new)
				(s->size < t->size ? s->size : t->size,
						s->cmp, s->hash);
		if (NULL == set)
			return NULL;

		assert(s->cmp == t->cmp && s->hash == t->hash);

		for (i = 0; i < t->size; i++)
		{
			for (q = t->buckets[i]; q; q = q->link)
			{
				if (!MODULE_FUN_NAME(Set, member)(s, q->member))
				{
					MODULE_FUN_NAME(Set, put)(set, q->member);
				}
			}
		}

		return set;
	}
}

/* return a set, the member only in s or only in t */
T MODULE_FUN_NAME(Set, diff)(T s, T t)
{
	int i;
	struct member *q = NULL;
	T set = NULL;

	if (NULL == s)
	{
		assert(t);
		return copy(t, t->size);
	}
	else if (NULL == t)
	{
		return copy(s, s->size);
	}
	else
	{
		set = MODULE_FUN_NAME(Set, new)
				(s->size < t->size ? s->size : t->size,
						s->cmp, s->hash);
		if (NULL == set)
			return NULL;

		assert(s->cmp == t->cmp && s->hash == t->hash);

		for (i = 0; i < t->size; i++)
		{
			for (q = t->buckets[i]; q; q = q->link)
			{
				if (!MODULE_FUN_NAME(Set, member)(s, q->member))
					MODULE_FUN_NAME(Set, put)(set, q->member);
			}
		}

		for (i = 0; i < s->size; i++)
		{
			for (q = s->buckets[i]; q; q = q->link)
			{
				if (!MODULE_FUN_NAME(Set, member)(t, q->member))
					MODULE_FUN_NAME(Set, put)(set, q->member);
			}
		}

		return set;
	}
}




