
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE	1
#define FALSE	0
#define NEGATIVE_INFINITE	-10000000

struct fib_heap_node {
	struct fib_heap_node *left;
	struct fib_heap_node *right;
	struct fib_heap_node *p;
	struct fib_heap_node *child;
//	void *key;
	int key;
	int mark;
	int flag;
	int degree;
};

struct fib_heap {
	int n;
	struct fib_heap_node *min;
	int (*cmp)(int , int);
};

static int default_cmp(int arg1, int arg2)
{
	int key1 = (int )arg1;
	int key2 = (int )arg2;

	if (key1 > key2)
		return 1;
	else if (key1 == key2)
		return 0;
	else
		return -1;
}


static int cut(struct fib_heap *h, 
		struct fib_heap_node *x, 
		struct fib_heap_node *y)
{
	struct fib_heap_node *node = NULL;

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

static void cascading_cut(struct fib_heap *h, 
		struct fib_heap_node *y)
{
	struct fib_heap_node *z = NULL;

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

static void fib_heap_link(struct fib_heap *h, 
				struct fib_heap_node *y,
				struct fib_heap_node *x)
{
	struct fib_heap_node *child = NULL;

	/* remove y from the root list */
	y->left->right = y->right;
	y->right->left = y->left;
	y->left = NULL;
	y->right = NULL;

	/* add y to the child list of x */
	child = x->child;
	if (child)
	{
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

static void consolidate(struct fib_heap *h)
{
#define D	(h->n)	// how to make it
	struct fib_heap_node *A[D];
	struct fib_heap_node *x = NULL;
	struct fib_heap_node *y = NULL;
	struct fib_heap_node *w = NULL;
	struct fib_heap_node *tmp = NULL;
	int d = 0;

	assert(h->min);

	for (int i = 0; i < D; i++)
		A[i] = NULL;

	for (w = h->min; ; )
	{
		x = w;
		x->flag = TRUE;
		w = w->right;
		d = x->degree;

		while (A[d] != NULL)
		{
			y = A[d];
			if (h->cmp(x->key, y->key) > 0)
			{
				tmp = x;
				x = y;
				y = tmp;
			}
			y->flag = FALSE;
			fib_heap_link(h, y, x);
			A[d] = NULL;
			d += 1;
		}

		A[d] = x;

		if (TRUE == w->flag)
		{
//fprintf(stdout, "%s: w->right: %p, h->min: %p\n",
//			__func__, w->right, h->min);
			break;
		}
	}

	h->min = NULL;
	for (int i = 0; i < D; i++)
	{
		if (A[i])
		{
			A[i]->flag = FALSE;
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
				if (h->cmp(A[i]->key, h->min->key) < 0)
					h->min = A[i];
			}
		}
	}
}

/*************************************************************/

//static struct fib_heap_node *fib_heap_node_create(void *key)
static struct fib_heap_node *fib_heap_node_create(int key)
{
	struct fib_heap_node *node = NULL;

	assert(key);

	node = (struct fib_heap_node *)calloc(1, sizeof(*node));
	if (node)
	{
		node->key = key;
	}

	return node;
}

static void fib_heap_node_destroy(struct fib_heap_node **nodep)
{
	assert(nodep);
	assert(*nodep);

	if (nodep && *nodep)
	{
		free(*nodep);
		*nodep = NULL;
	}
}


struct fib_heap *fib_heap_make(int cmp(int, int))
{
	struct fib_heap *h = NULL;

	h = (struct fib_heap *)calloc(1, sizeof(*h));
	if (h)
	{
		h->n = 0;
		h->min = NULL;
		if (cmp)
			h->cmp = cmp;
		else
			h->cmp = default_cmp;
	}
	return h;
}

void fib_heap_destroy(struct fib_heap **hpp)
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


struct fib_heap_node *fib_heap_insert(
				struct fib_heap *h, 
				struct fib_heap_node *x)
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
		struct fib_heap_node *node = h->min;

		x->left = node;
		x->right = node->right;
		node->right->left = x;
		node->right = x;

		if ((h->cmp) && (h->cmp(h->min->key, x->key) > 0))
		{
				h->min = x;
		}
	}

	h->n += 1;

	return x;
}


struct fib_heap_node *fib_heap_minmum(
				struct fib_heap *h)
{
	assert(h);

	return h->min;
}

struct fib_heap_node *fib_heap_extract_min(
				struct fib_heap *h)
{
	assert(h);

	struct fib_heap_node *z = NULL;
	struct fib_heap_node *x = NULL;

	z = h->min;
	if (z != NULL)
	{
		while (z->child != NULL)
		{
			x = z->child;

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
			}

			/* add x to root list of h */
			z->left->right = x;
			x->left = z->left;
			x->right = z;
			z->left = x;
			x->p = NULL;
		}

		/* remove z from the root list of h */
		z->left->right = z->right;
		z->right->left = z->left;

		if (z == z->right)
		{
			z->left = NULL;
			z->right = NULL;
			h->min = NULL;
		}
		else
		{
			h->min = z->right;
			consolidate(h);
			h->n -= 1;
		}
	}

	return z;
}

struct fib_heap *fib_heap_union(
				struct fib_heap *h1,
				struct fib_heap *h2)
{
	struct fib_heap *h = NULL;

	assert(h1);
	assert(h2);
	assert(h1->cmp == h2->cmp);

	h = fib_heap_make(h1->cmp);
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
		struct fib_heap_node *node1 = h1->min->left;
		struct fib_heap_node *node2 = h2->min->left;

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
		((h1->min != NULL) && (h->cmp(h2->min->key, h1->min->key) < 0)))
	{
		h->min = h2->min;
	}
	h->n = h1->n + h2->n;

	h1->min = NULL;
	fib_heap_destroy(&h1);
	h2->min = NULL;
	fib_heap_destroy(&h2);

	return h;
}

struct fib_heap_node *fib_heap_decrease_key(
				struct fib_heap *h, 
				struct fib_heap_node *x,
				int k)
{
	struct fib_heap_node *y = NULL;

	assert(h);
	assert(x);

	if (h->cmp(k, x->key) > 0)
	{
		fprintf(stderr, "new key(%d) value is"
						" greater than current's value(%d)\n",
						k, x->key);
		return NULL;
	}

	x->key = k;
	y = x->p;

	if ((y != NULL) && (h->cmp(x->key, y->key) < 0))
	{
		cut(h, x, y);
		cascading_cut(h, y);
	}

	if (h->cmp(x->key, h->min->key) < 0)
	{
		h->min = x;
	}

	return x;
}

struct fib_heap_node *fib_heap_delete(
				struct fib_heap *h,
				struct fib_heap_node *x)
{
	assert(h);
	assert(x);

	fib_heap_decrease_key(h, x, NEGATIVE_INFINITE);
	return fib_heap_extract_min(h);
}

/********************************************************/

void fib_heap_print(struct fib_heap_node *min)
{
	static int level = 0;
	struct fib_heap_node *node = NULL;

	if (NULL == min)
	{
		return ;
	}
	
	fprintf(stdout, "level: %d\n", level++);
	for (node = min; ; node = node->right)
	{
	fprintf(stdout, "node: %p, key: %d, child: %p, left: %p, right: %p\n", 
					node, node->key, node->child,
					node->left, node->right);
		if (node->child) fib_heap_print(node->child);
		if (node->right == min)
			break;
	}
}


int main(int argc, char *argv[])
{
	int ret = 0;
	struct fib_heap *h = NULL;
	struct fib_heap *h2 = NULL;
	struct fib_heap *h3 = NULL;
	struct fib_heap_node *node = NULL;


	h = fib_heap_make(NULL);
	h2 = fib_heap_make(NULL);
	node = fib_heap_insert(h, fib_heap_node_create(1));
	node = fib_heap_insert(h, fib_heap_node_create(2));
	node = fib_heap_insert(h, fib_heap_node_create(3));
	node = fib_heap_insert(h, fib_heap_node_create(4));
//	fib_heap_print(h->min);
	node = fib_heap_insert(h, fib_heap_node_create(5));
	node = fib_heap_insert(h, fib_heap_node_create(6));
	node = fib_heap_insert(h, fib_heap_node_create(7));
	node = fib_heap_extract_min(h);
	node = fib_heap_insert(h2, fib_heap_node_create(8));
	node = fib_heap_insert(h2, fib_heap_node_create(9));
	node = fib_heap_insert(h2, fib_heap_node_create(10));
	node = fib_heap_insert(h2, fib_heap_node_create(11));
	node = fib_heap_insert(h2, fib_heap_node_create(12));
	node = fib_heap_insert(h2, fib_heap_node_create(13));
	node = fib_heap_insert(h2, fib_heap_node_create(14));
	node = fib_heap_insert(h2, fib_heap_node_create(15));
	node = fib_heap_extract_min(h2);
	fprintf(stdout, "heap 1:\n");
	fib_heap_print(h->min);
	fprintf(stdout, "heap 2:\n");
	fib_heap_print(h2->min);
	h3 = fib_heap_union(h, h2);
	fprintf(stdout, "heap union: %p\n", h3);
	fib_heap_print(h3->min);
//	exit(0);

//	fib_heap_print(h->min);
	node = fib_heap_extract_min(h3);
	fprintf(stdout, "value: %d\n", node->key);
	fib_heap_node_destroy(&node);
	fib_heap_print(h3->min);

	/*
	node = fib_heap_extract_min(h);
	fprintf(stdout, "value: %d\n", node->key);
	fib_heap_node_destroy(&node);
	fib_heap_print(h->min);
	*/
	node = fib_heap_minmum(h3);
	fprintf(stdout, "value: %d\n", node->key);
	fib_heap_delete(h3, node);
	fib_heap_node_destroy(&node);

	node = fib_heap_minmum(h3);
	fprintf(stdout, "value: %d\n", node->key);
	fib_heap_delete(h3, node);
	fib_heap_node_destroy(&node);

	fib_heap_print(h3->min);

	return ret;
}








