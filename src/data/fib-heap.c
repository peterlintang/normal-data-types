
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fib-heap.h"

#define TRUE	1
#define FALSE	0

#define T FibHeap_T
#define NODE FibHeap_Node 

struct NODE {
	NODE left;
	NODE right;
	NODE p;
	NODE child;
	void *priv;
	int mark;
//	int flag;
	int degree;
};

struct T {
	int n;
	NODE min;
	int (*cmp)(void *, void *);
	void *infinite;
};

static void print_info(NODE node);

static int default_cmp(void *arg1, void *arg2)
{
	unsigned long key1 = (unsigned long)arg1;
	unsigned long key2 = (unsigned long)arg2;

	if (key1 > key2)
		return 1;
	else if (key1 == key2)
		return 0;
	else
		return -1;
}


static int cut(T h, NODE x, NODE y)
{
	NODE node = NULL;

	node = h->min;

	if (y->child->left == y->child)
	{
		y->child = NULL;
	}
	else
	{
		x->left->right = x->right;
		x->right->left = x->left;
	}
	x->left = NULL;
	x->right = NULL;

	node->left->right = x;
	x->left = node->left;
	x->right = node;
	node->left = x;

	y->degree -= 1;

	x->p = NULL;
	x->mark = FALSE;
}

static void cascading_cut(T h, NODE y)
{
	NODE z = NULL;

	z = y->p;
	if (z != NULL)
	{
		if (y->mark == FALSE)
		{
			y->mark = TRUE;
		}
		else
		{
			cut(h, y, z);
			cascading_cut(h, z);
		}
	}
}

static void fib_heap_link(T h, NODE y, NODE x)
{
	NODE child = NULL;

	/* remove y from the root list */
	/*
	y->left->right = y->right;
	y->right->left = y->left;
	y->left = NULL;
	y->right = NULL;
	*/

	/* add y to the child list of x */
	child = x->child;
	if (child)
	{
		/*
		y->left = child;
		y->right = child->right;
		child->right->left = y;
		child->right = y;
		*/

		child->left->right = y;
		y->left = child->left;
		y->right = child;
		child->left = y;
	}
	else
	{
		y->left = y->right = y;
		x->child = y;
	}
	y->p = x;

	x->degree += 1;

	y->mark = FALSE;
}

static void consolidate(T h)
{
#define D	(h->n + 1)	// how to make it
	NODE A[D];
	NODE x = NULL;
	NODE y = NULL;
	NODE w = NULL;
	NODE tmp = NULL;
	int d = 0;

	assert(h->min);
//			fprintf(stdout, "started: %d\n", D);

	for (int i = 0; i < D; i++)
		A[i] = NULL;

	for (w = h->min; w != NULL; )
	{
		x = w;
		d = x->degree;

//		x->flag = TRUE;
		if (w == w->right)
		{
			w = NULL;
		}
		else
		{
			w->left->right = w->right;
			w->right->left = w->left;
			w = w->right;
		}
		x->left = NULL;
		x->right = NULL;

		while (A[d] != NULL)
		{
//fprintf(stdout, "%s: w->right: %p, h->min: %p\n",
//			__func__, w->right, h->min);
			y = A[d];
			if (h->cmp(x->priv, y->priv) > 0)
			{
				tmp = x;
				x = y;
				y = tmp;
			}
//			y->flag = FALSE;
			fib_heap_link(h, y, x);
//			fprintf(stdout, "d: %d, A[d]: %p\n", d, A[d]);
			A[d] = NULL;
//			fprintf(stdout, "d: %d, A[d]: %p\n", d, A[d]);
			d += 1;
//		fprintf(stdout, "%s node %p, right: %p, left: %p, p: %p, priv: %d\n", 
//						__func__, x, x->right, x->left, x->p, (int)x->priv);
//		fprintf(stdout, "%s node %p, right: %p, left: %p, p: %p, priv: %d\n", 
//						__func__, y, y->right, y->left, y->p, (int)y->priv);
		}

//fprintf(stdout, "%s: w->right: %p, h->min: %p\n",
//			__func__, w->right, h->min);
//		fprintf(stdout, "out: d: %d, A[d]: %p\n", d, A[d]);
		A[d] = x;
//		fprintf(stdout, "out: d: %d, A[d]: %p, x: %p\n", d, A[d], x);

//		if (TRUE == w->flag)
		{
//fprintf(stdout, "%s: w->right: %p, h->min: %p\n",
//			__func__, w->right, h->min);
//			break;
		}
	}
//				fprintf(stdout, "finished\n");

	h->min = NULL;
	for (int i = 0; i < D; i++)
	{
//			fprintf(stdout, "i: %d, A[i]: %p\n", i, A[i]);
		if (A[i])
		{
//			A[i]->flag = FALSE;
			if (h->min == NULL)
			{
				A[i]->left = A[i]->right = A[i];
				h->min = A[i];
			}
			else
			{
				h->min->left->right = A[i];
				A[i]->left = h->min->left;
				A[i]->right = h->min;
				h->min->left = A[i];
				if (h->cmp(A[i]->priv, h->min->priv) < 0)
					h->min = A[i];
			}
		}
	}
}

/*************************************************************/
void *MODULE_FUN_NAME(FibHeap, NodePriv)(NODE node)
{
	assert(node);

	return node->priv;
}

NODE MODULE_FUN_NAME(FibHeap, NodeCreate)(void *priv)
{
	NODE node = NULL;

	assert(priv);

	node = (NODE)calloc(1, sizeof(*node));
	if (node)
	{
		node->priv = priv;
	}

	return node;
}

void MODULE_FUN_NAME(FibHeap, NodeFree)(NODE *nodep)
{
	assert(nodep);
	assert(*nodep);

	if (nodep && *nodep)
	{
		free(*nodep);
		*nodep = NULL;
	}
}


T MODULE_FUN_NAME(FibHeap, new)(int cmp(void *, void *), void *infinite)
{
	T h = NULL;

	h = (T)calloc(1, sizeof(*h));
	if (h)
	{
		h->n = 0;
		h->min = NULL;
		h->infinite = infinite;
		if (cmp)
			h->cmp = cmp;
		else
			h->cmp = default_cmp;
	}
	return h;
}

void MODULE_FUN_NAME(FibHeap, free)(T *hpp)
{
	assert(hpp);
	assert(*hpp);

	if ((*hpp)->min != NULL)
	{
		fprintf(stderr, "fib heap not empty\n");
		return ;
	}

	free(*hpp);
	*hpp = NULL;
}


NODE MODULE_FUN_NAME(FibHeap, insert)(T h, NODE x)
{
	assert(h);
	assert(x);

	x->degree = 0;
	x->mark = FALSE;
	x->child = NULL;
	x->p = NULL;

	if (NULL == h->min)
	{
		x->left = x->right = x;
		h->min = x;
	}
	else
	{
		NODE node = h->min;

		x->left = node;
		x->right = node->right;
		node->right->left = x;
		node->right = x;

		if ((h->cmp) && (h->cmp(h->min->priv, x->priv) > 0))
		{
				h->min = x;
		}
	}

	h->n += 1;

	return x;
}


NODE MODULE_FUN_NAME(FibHeap, minmum)(T h)
{
	assert(h);

	return h->min;
}

static void print_info(NODE node)
{
	NODE start = NULL;
	fprintf(stdout, "%s: %p\n", __func__, node);
	for (start = node; start != NULL; start = start->right)
	{
		fprintf(stdout, "node %p, right: %p, left: %p, p: %p, priv: %d\n", 
						start, start->right, start->left, start->p, (int)start->priv);
		if (start->right == node)
		{
			break;
		}
	}
}

NODE MODULE_FUN_NAME(FibHeap, extractMin)(T h)
{
	assert(h);

	NODE z = NULL;
	NODE x = NULL;

	z = h->min;
	if (z != NULL)
	{
		while (z->child != NULL)
		{
			x = z->child;
#if 0
			fprintf(stdout, "x: %p, z: %p\n", x, z);
			fprintf(stdout, "x right: %p, left: %p, p: %p\n", x->right, x->left, x->p);
			NODE node = NULL;
			NODE min = x;
	for (node = min; ; node = node->right)
	{
	fprintf(stdout, "node: %p, key: %d, child: %p, left: %p, right: %p, p: %p\n", 
					node, (int)node->priv, node->child,
					node->left, node->right, node->p);
		if (node->right == min)
			break;
	}

#endif
			/* remove x from child list of z */
			if (x == x->right)
			{
				z->child = NULL;
			}
			else
			{
				z->child = x->right;

				x->left->right = x->right;
				x->right->left = x->left;
				x->left = NULL;
				x->right = NULL;
			}

			/* add x to root list of h */
			z->left->right = x;
			x->left = z->left;
			x->right = z;
			z->left = x;
			x->p = NULL;
		}
//		MODULE_FUN_NAME(FibHeap, print)(h);
//		print_info(h->min);

		/* remove z from the root list of h */
		z->left->right = z->right;
		z->right->left = z->left;

		if (z == z->right)
		{
//				fprintf(stdout, "last item\n");
			h->min = NULL;
			z->left = NULL;
			z->right = NULL;
		}
		else
		{
			h->min = z->right;
//				fprintf(stdout, "before consolidate\n");
//		MODULE_FUN_NAME(FibHeap, print)(h);
//		print_info(h->min);
			consolidate(h);
//				fprintf(stdout, "after consolidate\n");
//		MODULE_FUN_NAME(FibHeap, print)(h);
//		print_info(h->min);
		//	fprintf(stdout, "hello\n");
		}
		h->n -= 1;
	}

	return z;
}

T MODULE_FUN_NAME(FibHeap, union)(T *h1p, T *h2p)
{
	T h = NULL;
	T h1 = *h1p;
	T h2 = *h2p;

	assert(h1);
	assert(h2);
	assert(h1->cmp == h2->cmp);

	h = MODULE_FUN_NAME(FibHeap, new)(h1->cmp, h1->infinite);
	if (NULL == h)
	{
		return NULL;
	}

	h->min = h1->min;

	/* concatenate h2's root list to h's root list */
	if (h1->min == NULL)
	{
		h->min = h2->min;
	}
	else if (h2->min == NULL)
	{
	}
	else
	{
		NODE node1 = h1->min->left;
		NODE node2 = h2->min->left;

		/*
		node1->right = NULL;
		h1->min->left = NULL;

		node2->right = NULL;
		h2->min->left = NULL;
		*/

		h1->min->left = node2;
		node2->right = h1->min;

		h2->min->left = node1;
		node1->right = h2->min;
	}

	if ((h1->min == NULL) || 
		((h1->min != NULL) && (h->cmp(h2->min->priv, h1->min->priv) < 0)))
	{
		h->min = h2->min;
	}
	h->n = h1->n + h2->n;

	h1->min = NULL;
	MODULE_FUN_NAME(FibHeap, free)(&h1);
	*h1p = NULL;

	h2->min = NULL;
	MODULE_FUN_NAME(FibHeap, free)(&h2);
	*h2p = NULL;

	return h;
}

void *MODULE_FUN_NAME(FibHeap, decreaseKey)(T h, NODE x, void *k)
{
	NODE y = NULL;
	void *prev = NULL;

	assert(h);
	assert(x);

	if (h->cmp(k, x->priv) > 0)
	{
		return NULL;
	}

	prev = x->priv;
	x->priv = k;
	y = x->p;

	if ((y != NULL) && (h->cmp(x->priv, y->priv) < 0))
	{
		cut(h, x, y);
		cascading_cut(h, y);
	}

	if (h->cmp(x->priv, h->min->priv) < 0)
	{
		h->min = x;
	}

	return prev;
}

NODE MODULE_FUN_NAME(FibHeap, delete)(T h, NODE x)
{
	assert(h);
	assert(x);

	MODULE_FUN_NAME(FibHeap, decreaseKey)(h, x, h->infinite);
	return MODULE_FUN_NAME(FibHeap, extractMin)(h);
}

/********************************************************/
static void fibHeap_print(NODE node)
{
	NODE x = NULL;

	for (x = node; ; x = x->right)
	{
		fprintf(stdout, "node: %p, key: %d, child: %p, left: %p, right: %p, parent: %p\n", 
					x, (int)x->priv, x->child,
					x->left, x->right, x->p);
		if (x->child) fibHeap_print(node->child);
		if (x->right == node)
			break;
	}
}

void MODULE_FUN_NAME(FibHeap, print)(T h)
{
	assert(h);

	if (h->min)
	{
		fibHeap_print(h->min);
	}
}


#if 0
int main(int argc, char *argv[])
{
	int ret = 0;
	T h = NULL;
	T h2 = NULL;
	T h3 = NULL;
	NODE node = NULL;


	h = MODULE_FUN_NAME(FibHeap, new)(NULL, (void *)(-100000));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)(((void *)1)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(2)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(3)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(4)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(5)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(6)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(7)));
	node = MODULE_FUN_NAME(FibHeap, extractMin)(h);

	h2 = MODULE_FUN_NAME(FibHeap, new)(NULL, (void *)(-100000));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(8)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(9)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(10)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(11)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(12)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(13)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(14)));
	node = MODULE_FUN_NAME(FibHeap, insert)(h2, MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(15)));
	node = MODULE_FUN_NAME(FibHeap, extractMin)(h2);

	fprintf(stdout, "heap 1:\n");
	MODULE_FUN_NAME(FibHeap, print)(h->min);
	fprintf(stdout, "heap 2:\n");
	MODULE_FUN_NAME(FibHeap, print)(h2->min);
	h3 = MODULE_FUN_NAME(FibHeap, union)(&h, &h2);
	fprintf(stdout, "heap union: %p\n", h3);
	MODULE_FUN_NAME(FibHeap, print)(h3->min);

	node = MODULE_FUN_NAME(FibHeap, extractMin)(h3);
	fprintf(stdout, "value: %d\n", (int)node->priv);
	MODULE_FUN_NAME(FibHeap, NodeFree)(&node);
	MODULE_FUN_NAME(FibHeap, print)(h3->min);

	node = MODULE_FUN_NAME(FibHeap, minmum)(h3);
	fprintf(stdout, "value: %d\n", (int)node->priv);
	MODULE_FUN_NAME(FibHeap, delete)(h3, node);
	MODULE_FUN_NAME(FibHeap, NodeFree)(&node);

	node = MODULE_FUN_NAME(FibHeap, minmum)(h3);
	fprintf(stdout, "value: %d\n", (int)node->priv);
	MODULE_FUN_NAME(FibHeap, delete)(h3, node);
	MODULE_FUN_NAME(FibHeap, NodeFree)(&node);

	MODULE_FUN_NAME(FibHeap, print)(h3->min);

	return ret;
}

#endif







