
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "ring.h"

#define T Ring_T

struct T {
	struct node {
		struct node *llink;
		struct node *rlink;
		void *value;
	} *head;
	int length;
};


T MODULE_FUN_NAME(Ring, new)(void)
{
	T ring = NULL;

	ring = (T)calloc(1, sizeof(*ring));
	if (NULL != ring)
	{
		ring->head = NULL;
	}

	return ring;
}

T MODULE_FUN_NAME(Ring, ring)(void *x, ...)
{
	va_list ap;

	T ring = NULL;

	ring = MODULE_FUN_NAME(Ring, new)();
	if (ring)
	{
		va_start(ap, x);
		for (; x; x = va_arg(ap, void *))
		{
			MODULE_FUN_NAME(Ring, addhi)(ring, x);
		}
		va_end(ap);
	}

	return ring;
}

void MODULE_FUN_NAME(Ring, free)(T *ring)
{
	struct node *p = NULL;
	struct node *q = NULL;

	assert(ring && *ring);

	if ((p = (*ring)->head) != NULL)
	{
		int n = (*ring)->length;
		for ( ; n-- > 0; p = q)
		{
			q = p->rlink;
			free(p);
		}
	}

	free(*ring);
	*ring = NULL;
}

int MODULE_FUN_NAME(Ring, length)(T ring)
{
	assert(ring);

	return ring->length;
}


void *MODULE_FUN_NAME(Ring, get)(T ring, int i)
{
	int n = 0;
	struct node *q = NULL;

	assert(ring);
	assert(i >= 0 && i < ring->length);

	q = ring->head;

	if (i <= ring->length / 2)
	{
		for (n = i; n-- > 0; )
		{
			q = q->rlink;
		}
	}
	else
	{
		for (n = ring->length - i; n-- > 0; )
		{
			q = q->llink;
		}
	}

	return q->value;
}

void *MODULE_FUN_NAME(Ring, put)(T ring, int i, void *x)
{
	int n = 0;
	struct node *q = NULL;
	void *prev = NULL;

	assert(ring);
	assert(i >= 0 && i < ring->length);

	q = ring->head;

	if (i <= ring->length / 2)
	{
		for (n = i; n-- > 0; )
		{
			q = q->rlink;
		}
	}
	else
	{
		for (n = ring->length - i; n-- > 0; )
		{
			q = q->llink;
		}
	}

	prev = q->value;
	q->value = x;

	return prev;
}

void *MODULE_FUN_NAME(Ring, addhi)(T ring, void *x)
{
	struct node *p = NULL;
	struct node *q = NULL;

	assert(ring);

	p = (struct node *)calloc(1, sizeof(*p));
	if (p)
	{
		if ((q = ring->head) != NULL)
		{
			p->llink = q->llink;
			q->llink->rlink = p;
			p->rlink = q;
			q->llink = p;
		}
		else
		{
			ring->head = p;
			p->llink = p;
			p->rlink = p;
		}

		ring->length++;
		return p->value = x;
	}
}

void *MODULE_FUN_NAME(Ring, addlo)(T ring, void *x)
{
	assert(ring);

	MODULE_FUN_NAME(Ring, addhi)(ring, x);
	ring->head = ring->head->llink;

	return x;
}


/**********************************************************
 *
 * ring structure:
 * positive position:      1  2  3  4  5 6
 * element index:	        0  1  2  3  4
 * negative position:     -5 -4 -3 -2 -1 0
 *
 **********************************************************/
void *MODULE_FUN_NAME(Ring, add)(T ring, int pos, void *x)
{
	assert(ring);
	assert(pos >= -ring->length && pos <= ring->length + 1);

	if (1 == pos || pos == -ring->length)
	{
		return MODULE_FUN_NAME(Ring, addlo)(ring, x);
	}
	else if (0 == pos || pos == ring->length + 1)
	{
		return MODULE_FUN_NAME(Ring, addhi)(ring, x);
	}
	else
	{
		int n = 0;
		struct node *p = NULL;
		struct node *q = NULL;

		int i = pos < 0 ? pos + ring->length : pos - 1;

		p = (struct node *)calloc(1, sizeof(*p));
		if (p)
		{
			q = ring->head;
			if (i <= ring->length / 2)
			{
				for (n = i; n-- > 0; )
					q = q->rlink;
			}
			else
			{
				for (n = ring->length - i; n-- > 0; )
					q = q->llink;
			}

			p->llink = q->llink;
			q->llink->rlink = p;
			p->rlink = q;
			q->llink = p;

			ring->length++;
			return p->value = x;
		}

		return NULL;
	}
}

void *MODULE_FUN_NAME(Ring, remove)(T ring, int i)
{
	int n;
	void *x = NULL;
	struct node *q = NULL;

	assert(ring);
	assert(ring->length > 0);
	assert(i >= 0 && i < ring->length);

	q = ring->head;
	if (i <= ring->length / 2)
	{
		for (n = i; n-- > 0; )
			q = q->rlink;
	}
	else
	{
		for (n = ring->length - i; n-- > 0; )
			q = q->llink;
	}

	if (0 == i)
	{
		ring->head = ring->head->rlink;
	}

	x = q->value;

	q->llink->rlink = q->rlink;
	q->rlink->llink = q->llink;
	q->llink = q->rlink = NULL;
	free(q);

	ring->length--;
	if (0 == ring->length)
		ring->head = NULL;

	return x;
}

void *MODULE_FUN_NAME(Ring, rmhi)(T ring)
{
	void *x = NULL;
	struct node *q = NULL;

	assert(ring && ring->length > 0);

	q = ring->head->llink;
	x = q->value;

	q->llink->rlink = q->rlink;
	q->rlink->llink = q->llink;
	q->llink = q->rlink = NULL;
	free(q);

	ring->length--;
	if (0 == ring->length)
		ring->head = NULL;

	return x;
}

void *MODULE_FUN_NAME(Ring, rmlo)(T ring)
{
	void *x = NULL;
	struct node *q = NULL;

	assert(ring && ring->length > 0);

	q = ring->head;
	ring->head = ring->head->rlink;

	x = q->value;

	q->llink->rlink = q->rlink;
	q->rlink->llink = q->llink;
	q->llink = q->rlink = NULL;
	free(q);

	ring->length--;
	if (0 == ring->length)
		ring->head = NULL;
	
	return x;
}

/**********************************
 *
 * use element of index n as head
 *
 **********************************/
void MODULE_FUN_NAME(Ring, rotate)(T ring, int n)
{
	struct node *q = NULL;
	int i = 0;

	assert(ring);
	assert(n >= -ring->length && n <= ring->length);

	if (n >= 0)
	{
		i = n % ring->length;
	}
	else
	{
		i = n + ring->length;
	}

	q = ring->head;
	if (i <= ring->length / 2)
	{
		for (n = i; n-- > 0; )
			q = q->rlink;
	}
	else
	{
		for (n = ring->length - i; n-- > 0; )
			q = q->llink;
	}

	ring->head = q;
}




