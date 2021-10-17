
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"
#include "bfs.h"


/*
 * you xiang & wu xiang tu jun ke
 */
int bfs(struct G *g, int k)
{
	struct V *u = NULL;
	struct V *v = NULL;
	struct V *s = NULL;
	struct Queue *q = NULL;

	q = (struct Queue *)calloc(1, sizeof(*q));
	if (q == NULL)
	{
		return -1;
	}

	for (int i = 0; i < g->vs_num; i++)
	{
		u = g->vs[i];
		if (u->v != k)
		{
			u->color = WHITE;
			u->d = INFINITE;
			u->p = NULL;
		}
		else
		{
			s = u;
		}
	}

	s->color = GRAY;
	s->d = 0;
	s->p = NULL;

	queue_en(q, s);

	while (queue_empty(q) == 0)
	{
		struct item *item = NULL;
		
		queue_de(q, (void **)&u);
		for (item = u->l.nil.next; item != &(u->l.nil); item = item->next)
		{
			v = (struct V *)item->key;

			if (v->color == WHITE)
			{
				v->color = GRAY;
				v->d = u->d + 1;
				v->p = u;
				queue_en(q, v);
			}
		}
		u->color = BLACK;
	}

	return 0;
}

void G_tree_print(struct G *g, int k)
{
	struct V *v = NULL;

	for (int i = 0; i < g->vs_num; i++)
	{
		v = g->vs[i];
		if (v->v == k)
		{
			break;
		}
	}

	while (v)
	{
		fprintf(stdout, "v: %d, d: %d\n", v->v, v->d);
		v = v->p;
	}
}

static void _G_tree_print_path(struct G *g, 
				struct V *s, struct V *v)
{
	if (s == v)
	{
		fprintf(stdout, "%d ", s->v);
	}
	else if (v->p == NULL)
	{
		fprintf(stdout, "no path from %d to %d exist\n", 
			s->v, v->v);
	}
	else
	{
		_G_tree_print_path(g, s, v->p);
		fprintf(stdout, "%d ", v->v);
	}
}

void G_tree_print_path(struct G *g, int s, int v)
{
	struct V *s_p = NULL;
	struct V *v_p = NULL;
	struct V *u = NULL;

	assert(g);
	assert(s != v);

	for (int i = 0; i < g->vs_num; i++)
	{
		u = g->vs[i];
		if (u->v == s)
			s_p = u;
		if (u->v == v)
			v_p = u;
		if (s_p && v_p)
			break;
	}

	_G_tree_print_path(g, s_p, v_p);
}

/*
 * test code
int main(int argc, char *argv[])
{
	int ret = 0;
	struct G *g = NULL;

	g = G_create();
	bfs(g, 1);
	G_tree_print(g, 3);
//	G_print(g);

	return ret;
}
 */



