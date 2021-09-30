

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
/*
 * name: MODULE_FUN_NAME(Set, new)
 * description: create a new set, 
 * return value: return the pointer to new set
 * args: @hint: number of elements in set suggested
 * 		 @cmp: function to compare two elements
 * 		 @hash: function do hash
 */
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

/*
 * name: MODULE_FUN_NAME(Set, member)
 * description: judge @member is in @set
 * return value: 1: is a member of set
 * 				 0: not
 * args: @set: pointer to set
 * 		@member: pointer to member
 */
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

/*
 * name: MODULE_FUN_NAME(Set, put)
 * description: judge @member is in @set
 * return value: 1: put success
 * 				 0: failed to put in set
 * args: @set: pointer to set
 * 		@member: pointer to member
 */
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

/*
 * name: MODULE_FUN_NAME(Set, remove)
 * description: remove @member from @set
 * return value: pointer to member: success
 * 				 NULL: failed 
 * args: @set: pointer to set
 * 		@member: pointer to member
 */
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

/*
 * name: MODULE_FUN_NAME(Set, length)
 * description: return total elements in set
 * return value: return total number of elements in set
 * args: @set: pointer to set
 */
int MODULE_FUN_NAME(Set, length)(T set)
{
	assert(set);

	return set->length;
}

/*
 * name: MODULE_FUN_NAME(Set, free)
 * description: free a set
 * return value: return void
 * args: @set: pointer's pointer to set
 */
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

/*
 *
 * 返回set中找到的第一个元素
 */
void *MODULE_FUN_NAME(Set, first)(T set)
{
	int len = 0;
	struct member *p = NULL;

	assert(set);

	len = MODULE_FUN_NAME(Set, length)(set);
	if (0 == len)
	{
		return NULL;
	}

	for (int i = 0; i < set->size; i++)
	{
		p = set->buckets[i];
		if (p != NULL)
		{
			return (void *)(p->member);
		}
	}

}

/*
 * 返回set中member之后的下一个元素
 */
void *MODULE_FUN_NAME(Set, next)(T set, void *member)
{
	int i = 0;
	int len = 0;
	struct member *p = NULL;

	assert(set);
	assert(member);

	i = (*set->hash)(member) % set->size;
	for (p = set->buckets[i]; p; p = p->link)
		if ((*set->cmp)(member, p->member) == 0)
			break;

	p = p->link;

	/*需在接下来的buckets中查找第一个元素*/
	if (p == NULL)
	{
		for (i = i + 1; i < set->size; i++)
		{
			p = set->buckets[i];
			if (p != NULL)
			{
				return (void *)(p->member);
			}
		}
		return NULL;
	}
	else
	{
		return (void *)p->member;
	}
}

/*
 *   是否最后一个元素
 *   */
int MODULE_FUN_NAME(Set, end)(T set, void *member)
{
	void *p = NULL;

	assert(set);
	assert(member);

	p = MODULE_FUN_NAME(Set, next)(set, member);
	if (p == NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * name: MODULE_FUN_NAME(Set, map)
 * description: for each member in @set, call @apply
 * return value: void
 * args: @set: pointer to set
 * 		@apply: function to call, 0 success, !0 failed, will return immediately
 * 		@cl: private data to user
 */
void MODULE_FUN_NAME(Set, map)
	(T set, int apply(const void *member, void *cl), void *cl)
{
	int i;
	int ret = 0;
	unsigned stamp = 0;
	struct member *p = NULL;

	assert(set);
	assert(apply);

	stamp = set->timestamp;
	for (i = 0; i < set->size; i++)
	{
		for (p = set->buckets[i]; p; p = p->link)
		{
			ret = apply(p->member, cl);
			if (ret != 0) return ;
			assert(set->timestamp == stamp);
		}
	}
}

/*
 * name: MODULE_FUN_NAME(Set, toArray)
 * description: put members in @set to array
 * return value: array to members
 * args: @set: pointer to set
 * 		@end: indicator end of array
 */
void **MODULE_FUN_NAME(Set, toArray)(T set, void *end)
{
	int i;
	int j = 0;
	void **array = NULL;
	struct member *p = NULL;

	assert(set);

	array = (void **)calloc(1, (set->length + 1) * sizeof(*array));
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
/*
 * name: MODULE_FUN_NAME(Set, union)
 * description: put members in @s and @t to a new set
 * return value: return a set, which the member is in t or in s 
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, union)(T s, T t)
{
	int i;
	struct member *p = NULL;
	T set = NULL;

	assert(s != NULL || t != NULL);

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
/*
 * name: MODULE_FUN_NAME(Set, inter)
 * description: put members both in @s and @t to a new set
 * return value: return a set, which the member is in t and in s 
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, inter)(T s, T t)
{
	int i;
	struct member *q = NULL;
	T set = NULL;

	assert(s != NULL || t != NULL);

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
/*
 * name: MODULE_FUN_NAME(Set, minus)
 * description: put members in @s and not in @t to a new set
 * return value: return a set, which the member is in t but not in s
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, minus)(T t, T s)
{
	int i;
	struct member *q = NULL;
	T set = NULL;

	assert(s != NULL || t != NULL);

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
/*
 * name: MODULE_FUN_NAME(Set, diff)
 * description:  put members which only in @s or in @t to a new set
 * return value: return a set, the member only in s or only in t
 * args: @s: pointer to set @s
 * 		 @t: pointer to set @t
 */
T MODULE_FUN_NAME(Set, diff)(T s, T t)
{
	int i;
	struct member *q = NULL;
	T set = NULL;

	assert(s != NULL || t != NULL);

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




