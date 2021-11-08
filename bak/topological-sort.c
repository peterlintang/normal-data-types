
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"
#include "topological-sort.h"

static int time = 0;

static int dfs_visit(struct G *g, struct V *u, struct Linked_list *l)
{
	struct item *cur = NULL;
	struct V *v = NULL;

	time = time + 1;
	u->d = time;
	u->color = GRAY;

	for (cur = u->l.nil.next; cur != &(u->l.nil); cur = cur->next)
	{
		v = (struct V *)cur->key;
		if (v->color == WHITE)
		{
			v->p = u;
			dfs_visit(g, v, l);
		}
	}

	u->color = BLACK;
	time = time + 1;
	u->f = time;

	list_insert(l, u);

	return 0;
}

/*
 * you xiang tu 
 */
int topological_sort(struct G *g, struct Linked_list *l)
{
	struct V *u = NULL;

	assert(g);
	assert(l);

	for (int i = 0; i < g->vs_num; i++)
	{
		u = g->vs[i];
		u->color = WHITE;
		u->p = NULL;
	}

	time = 0;

	for (int i = 0; i < g->vs_num; i++)
	{
		u = g->vs[i];
		if (u->color == WHITE)
		{
			dfs_visit(g, u, l);
		}
	}

	return 0;
}

void topological_sort_print(struct Linked_list *l)
{
	struct item *cur = NULL;
	struct V *v = NULL;

	assert(l);

	for (cur = l->nil.next; cur != &(l->nil); cur = cur->next)
	{
		v = (struct V *)cur->key;
		fprintf(stdout, "%d ", v->v);
	}
	fprintf(stdout, "\n");
}


/*
 * test code
int main(int argc, char *argv[])
{
	int ret = 0;
	struct G *g = NULL;
	struct Linked_list l;

	l.nil.key = NULL;
	l.nil.next = &(l.nil);
	l.nil.prev = &(l.nil);

	g = G_create();
	topological_sort(g, &l);
//	G_print(g);
	topological_sort_print(&l);

	return ret;
}
 */



