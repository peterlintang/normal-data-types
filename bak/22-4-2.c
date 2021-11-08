
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"
#include "bfs.h"


static int _count_two_points_paths(struct V *u, struct V *v, int *paths)
{
	int ret = 0;
	struct item *item = NULL;
	struct V *cur = NULL;

//	fprintf(stdout, "\n");
//	fprintf(stdout, "u-v: %d, v->v: %d, \n", u->v, v->v);
	for (item = u->l.nil.next; item != &(u->l.nil); item = item->next)
	{
		cur = (struct V *)item->key;
//		fprintf(stdout, "cur: %d\n", cur->v);
		if (cur->v == v->v)
		{
			fprintf(stdout, "%d ", cur->v);
			*paths += 1;
			ret = 1;
		}
		else
			ret |= _count_two_points_paths(cur, v, paths);
	}
//	fprintf(stdout, "u-v: %d, v->v: %d, \n", u->v, v->v);
//	fprintf(stdout, "\n");

	if (ret == 1)
	{
		fprintf(stdout, "%d ", u->v);
	}

	return ret;
}

/*
 * you xiang tu 
 */
int count_two_points_paths(struct G *g, int u_v, int v_v)
{
	struct V *u = NULL;
	struct V *v = NULL;
	struct V *cur = NULL;
	int paths = 0;

	assert(u_v != v_v);

	for (int i = 0; i < g->vs_num; i++)
	{
		cur = g->vs[i];
		if (cur->v == u_v)
			u = cur;
		if (cur->v == v_v)
			v = cur;
		if (u && v)
			break;
	}

	_count_two_points_paths(u, v, &paths);

	return paths;
}


/*
 * test code
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	int u_v = 0;
	int v_v = 0;
	struct G *g = NULL;

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s num1 num2\n", argv[0]);
		exit(0);
	}

	u_v = atoi(argv[1]);
	v_v = atoi(argv[2]);

	fprintf(stdout, "u_v: %d, v_v: %d\n", u_v, v_v);
	g = G_create();
	ret = count_two_points_paths(g, u_v, v_v);
	fprintf(stdout, "\n(%d, %d) paths: %d\n", u_v, v_v, ret);

	return ret;
}



