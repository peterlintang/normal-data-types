
/*
 * 计算图的连通分量
 * 用set-list实现
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "set-list.h"



static void _set_destroy(struct set **setp)
{
	assert(setp);
	assert(*setp);

	free(*setp);
	*setp = NULL;
}

static struct set *_set_make(int x)
{
	struct set *set = NULL;
	struct set_item *item = NULL;

	assert(x);

	set = (struct set *)calloc(1, sizeof(*set));
	if (set)
	{
		item = (struct set_item *)calloc(1, sizeof(*item));
		if (item)
		{
			set->head = item;
			set->tail = item;
			set->next = NULL;
			set->prev = NULL;
			set->n = 1;
			item->p = x;
			item->set = set;
			item->next = NULL;
		}
		else
		{
			free(set);
			set = NULL;
		}
	}

	return set;
}

/*
 * attention: user free @x and @y
 */
static struct set *_set_union(struct set *x, struct set *y)
{
	struct set *z = NULL;
	struct set_item *item = NULL;

	assert(x);
	assert(y);

	/* not empty set */
	assert(x->head);
	assert(y->head);

	z = (struct set *)calloc(1, sizeof(*z));
	if (NULL == z)
	{
		return NULL;
	}

	z->head = x->head;
	z->tail = x->tail;
	z->n = x->n;

	do 
	{
		item = y->head;
		y->head = item->next;
		y->n -= 1;

		z->tail->next = item;
		item->next = NULL;
		z->tail = item;
		z->n += 1;

	} while (y->head != NULL);

	x->head = NULL;
	x->tail = NULL;
	x->n = 0;

	y->head = NULL;
	y->tail = NULL;
	y->n = 0;

	for (item = z->head; item != NULL; item = item->next)
	{
		item->set = z;
	}

	return z;
}

static struct set *_set_find(struct set *set, int x)
{
	struct set_item *item = NULL;

	assert(set);
	assert(x);

	for (item = set->head; item != NULL; item = item->next)
	{
		if (item->p == x)
			return set;
	}

	return NULL;
}


/*****************************/
int set_make(struct sets *sets, int x)
{
	struct set *set = NULL;

	assert(sets);

	set = _set_make(x);
	if (set)
	{
		set->prev = NULL;
		set->next = NULL;

		if (sets->head == NULL)
		{
			sets->head = set;
			sets->tail = set;
			sets->n = 1;
		}
		else
		{
			sets->tail->next = set;
			set->prev = sets->tail;
			sets->tail = set;
			sets->n += 1;
		}

		return x;
	}
	else
	{
		fprintf(stderr, "%s: no mem\n", __func__);
		return SET_ERROR;
	}
}

int set_find(struct sets *sets, int x)
{
	struct set *set = NULL;

	assert(sets);

	for (set = sets->head; set != NULL; set = set->next)
	{
		if (_set_find(set, x))
			return set->head->p;
	}

	return SET_ERROR;
}

/*
 *
 *  union operation, the @sets must have 2 sets in the list
 **/
int set_union(struct sets *sets, int x, int y)
{
	struct set *set_x = NULL;
	struct set *set_y = NULL;
	struct set *set = NULL;

	assert(sets);

	for (set = sets->head; set != NULL; set = set->next)
	{
		if (set_x == NULL)
			set_x = _set_find(set, x);
		if (set_y == NULL)
			set_y = _set_find(set, y);

		if ((set_x != NULL) && (set_y != NULL))
			break;
	}

	assert(set_x);
	assert(set_y);
	assert(set_x != set_y);

	set = _set_union(set_x, set_y);
	if (NULL == set)
	{
		fprintf(stderr, "%s: no mem", __func__);
		return SET_ERROR;
	}

	if (set_x == sets->head)
	{
		sets->head = set_x->next;
		if (sets->head)
			sets->head->prev = NULL;
	}
	else if (set_x == sets->tail)
	{
		sets->tail = set_x->prev;
		if (sets->tail)
			sets->tail->next = NULL;
	}
	else
	{
		set_x->prev->next = set_x->next;
		set_x->next->prev = set_x->prev;
	}
	set_x->prev = NULL;
	set_x->next = NULL;
	sets->n -= 1;

	if (set_y == sets->head)
	{
		sets->head = set_y->next;
		if (sets->head)
			sets->head->prev = NULL;
	}
	else if (set_y == sets->tail)
	{
		sets->tail = set_y->prev;
		if (sets->tail)
			sets->tail->next = NULL;
	}
	else
	{
		set_y->prev->next = set_y->next;
		set_y->next->prev = set_y->prev;
	}
	set_y->prev = NULL;
	set_y->next = NULL;
	sets->n -= 1;

	_set_destroy(&set_x);
	_set_destroy(&set_y);

	if (sets->n == 0)
	{
		sets->head = set;
		sets->tail = set;
		set->next = NULL;
		set->prev = NULL;
	}
	else
	{
		sets->tail->next = set;
		set->prev = sets->tail;
		sets->tail = set;
		set->next = NULL;
	}
	sets->n += 1;

	return set->head->p;
}

void set_print(struct sets *sets)
{
	struct set *set = NULL;
	struct set_item *item = NULL;

	assert(sets);

	for (set = sets->head; set != NULL; set = set->next)
	{
		for (item = set->head; item != NULL; item = item->next)
		{
			fprintf(stdout, " %d, %p;", item->p, item->set);
		}
		fprintf(stdout, "\n");
	}
}

/***************************/

struct V {
	int v;
};

struct E {
	int u;
	int v;
};

struct G {
	struct V *vs;
	int vs_num;
	struct E *es;
	int es_num;
};

void connected_components(struct sets *sets, struct G *g)
{
	for (int i = 0; i < g->vs_num; i++)
	{
		set_make(sets, g->vs[i].v);
	}

	for (int i = 0; i < g->es_num; i++)
	{
		if (set_find(sets, g->es[i].u) != set_find(sets, g->es[i].v))
		{
			set_union(sets, g->es[i].u, g->es[i].v);
		}
	}
}

int same_components(struct sets *sets, int x, int y)
{
	int x_r = 0;
	int y_r = 0;

	x_r = set_find(sets, x);
	y_r = set_find(sets, y);

	if ((x_r != SET_ERROR) && (x_r == y_r))
		return 1;
	else
		return 0;
}


/*
 * test code
int main(int argc, char *argv[])
{
	struct sets sets;
	struct G g;
	struct V *vs = NULL;
	struct E *es = NULL;
	int x;
	int y;

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s num1 num2\n", argv[0]);
		exit(0);
	}


	vs = (struct V *)calloc(10, sizeof(*vs));
	es = (struct E *)calloc(7, sizeof(*es));

	for (int i = 0; i < 10; i++)
	{
		vs[i].v = i + 1;
	}

	es[0].u = 1;
	es[0].v = 2;
	es[1].u = 1;
	es[1].v = 3;
	es[2].u = 2;
	es[2].v = 3;
	es[3].u = 2;
	es[3].v = 4;
	es[4].u = 5;
	es[4].v = 6;
	es[5].u = 5;
	es[5].v = 7;
	es[6].u = 8;
	es[6].v = 9;

	g.vs = vs;
	g.vs_num = 10;
	g.es = es;
	g.es_num = 7;

	sets.head = NULL;
	sets.tail = NULL;
	sets.n = 0;

	connected_components(&sets, &g);

	x = atoi(argv[1]);
	y = atoi(argv[2]);
	if (same_components(&sets, x, y))
		fprintf(stdout, "%d, %d is in the same set\n", x, y);
	else
		fprintf(stdout, "%d, %d is not in the same set\n", x, y);

	set_print(&sets);

	return 0;
}
 */




