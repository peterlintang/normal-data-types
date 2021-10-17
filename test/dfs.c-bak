
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"
#include "dfs.h"

static int time = 0;

static int dfs_visit(struct G *g, struct V *u)
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
			dfs_visit(g, v);
		}
	}

	u->color = BLACK;
	time = time + 1;
	u->f = time;

	return 0;
}

/*
 * you xiang & wu xiang tu jun ke
 */
int dfs(struct G *g)
{
	struct V *u = NULL;

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
			dfs_visit(g, u);
		}
	}

	return 0;
}


/*
 * test code
int main(int argc, char *argv[])
{
	int ret = 0;
	struct G *g = NULL;

	g = G_create();
	dfs(g);
	G_print(g);

	return ret;
}
 */



