
/*
 * not easy to understand the book's code
 * since dont have a route to find node by x
 */

#define SET_ERROR	-1

struct tree_node {
	struct tree_node *p;
	int rank;
	int v;
};

struct tree_set {
	struct tree_node *root;
	struct tree_set *next;
	struct tree_set *prev;
};

struct tree_sets {
	struct tree_set *head;
	struct tree_set *tail;
	int n;
};

set_make(struct tree_sets *sets, int x)
{
	struct tree_node *node = NULL;
	struct tree_set *set = NULL;

	assert(sets);

	node = (struct tree_node *)calloc(1, sizeof(*node));
	set = (struct tree_set *)calloc(1, sizeof(*set));
	if ((set == NULL) || (node == NULL))
	{
		if (node)
			free(node);
		if (set)
			free(set);
		return SET_ERROR;
	}

	node->v = x;
	node->p = node;
	node->rank = 0;

	set->root = node;
	set->prev = NULL;
	set->next = NULL;
	if (sets->head == NULL)
	{
		sets->head = set;
		sets->tail = set;
	}
	else
	{
		sets->tail->next = set;
		set->prev = sets->tail;
		sets->tail = set;
	}
	sets->n += 1;

	return x;
}

static tree_set *link(struct tree_set *x, struct tree_set *y)
{
	if (x->rank > y->rank)
	{
		y->p = x;
		return x;
	}
	else
	{
		x->p = y;
		if (x->rank == y->rank)
			y->rank = y->rank + 1;
		return y;
	}
}

set_union(struct tree_sets *sets, int x, int y)
{
	struct tree_set *set_x = NULL;
	struct tree_set *set_y = NULL;
	struct tree_set *set_z = NULL;
	struct tree_set *set_old = NULL;

	assert(sets);

	set_x = set_find(x);
	set_y = set_find(y);

	assert(set_x);
	assert(set_y);
	assert(set_x != set_y);

	set_z = link(set_x, set_y);

	if (set_z == set_y)
	{
		set_old = set_x;
	}
	else
	{
		set_old = set_y;
	}

	if (sets->head == set_old)
	{
		sets->head = set_old->next;
		if (sets->head)
			sets->head->prev = NULL;
	}
	else if (sets->tail == set_old)
	{
		sets->tail = set_old->prev;
		if (sets->tail)
			sets->tail->next = NULL;
	}
	else
	{
		set_old->next->prev = set_old->prev;
		set_old->prev->next = set_old->next;
	}

	set_old->next = NULL;
	set_old->prev = NULL;

	sets->n -= 1;
	if (sets->n == 0)
	{
		sets->head = NULL;
		sets->tail = NULL;
	}
}

static struct tree_node *_set_find(struct tree_node *x)
{
	if (x->p != x)
	{
		x->p = _set_find(x->p);
	}

	return x->p;
}

struct tree_set *set_find(struct tree_sets *sets, int x)
{
	struct tree_set *set = NULL;

	for (set = sets->head; set != NULL; set = set->next)
	{

	}

	return NULL;
}







