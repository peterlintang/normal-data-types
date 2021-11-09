
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../charpter-22/gve.h"
#include "../charpter-21/set-list.h"

struct weight_e {
	int u;
	int v;
	int w;
};

static int sort_weight_e(struct weight_e *wes, int wes_num)
{
	struct weight_e w;
	int i;
	int j;

	for (i = 1; i < wes_num; i++)
	{
		w.u = wes[i].u;
		w.v = wes[i].v;
		w.w = wes[i].w;
		for (j = i; j > 0; j--)
		{
			if (w.w < wes[j - 1].w)
			{
				wes[j].w = wes[j - 1].w;
				wes[j].u = wes[j - 1].u;
				wes[j].v = wes[j - 1].v;
			}
			else
			{
				break;
			}
		}
		wes[j].w = w.w;
		wes[j].u = w.u;
		wes[j].v = w.v;
	}

	return 0;
}


/*
 *
 *  algorith for charpter 23-2
 * */
int mst_kruskal(struct sets *sets, 
				struct G *g, 
				struct weight_e *wes, int wes_num,
				struct G *mst_g)
{
	assert(sets);
	assert(g);
	assert(mst_g);
	assert(wes);

	for (int i = 0; i < g->vs_num; i++)
	{
		set_make(sets, g->vs[i]->v);
	}

	sort_weight_e(wes, wes_num);

	for (int i = 0; i < wes_num; i++)
	{
		if (set_find(sets, wes[i].u) != set_find(sets, wes[i].v))
		{
			E_create(mst_g->vs[wes[i].u - 1], mst_g->vs[wes[i].v - 1]);
			set_union(sets, wes[i].u, wes[i].v);
			/*
			fprintf(stdout, "edge: %d-%d\n", wes[i].u, wes[i].v);
			fprintf(stdout, "edge: %d-%d\n", 
							(mst_g->vs[wes[i].u - 1])->v, 
							(mst_g->vs[wes[i].v - 1])->v);
			*/
		}
	}

	return 0;
}

/*
 *
 *  algorith for charpter 23-3
 * */
int mst_prim(struct sets *sets, 
				struct G *g, 
				struct weight_e *wes, int wes_num,
				struct G *mst_g)
{

	return 0;
}

static int mst_get_edge_weights(struct weight_e **wes, int *wes_num)
{
	int u = 0;
	int v = 0;
	int w = 0;
	struct weight_e *w_p = NULL;

	assert(wes);
	assert(wes_num);

	scanf("%d", wes_num);

	w_p = (struct weight_e *)calloc(*wes_num, sizeof(struct weight_e));
	if (w_p == NULL)
	{
		fprintf(stderr, "%s: no mem\n", __func__);
		exit(0);
	}

	for(int i = 0; i < *wes_num; i++)
	{
		scanf("%d,%d,%d", &u, &v, &w);
		w_p[i].u = u;
		w_p[i].v = v;
		w_p[i].w = w;
	}

	*wes = w_p;

	return 0;
}

static void print_weight_edges(struct weight_e *wes, int num)
{
	for (int i = 0; i < num; i++)
	{
		fprintf(stdout, "%s:(%d: %d %d %d)\n",
						__func__, i, 
						wes[i].u, wes[i].v, wes[i].w);
	}
}

/*
 * test code
 */
int main(int argc, char *argv[])
{
	struct sets sets;
	struct G *g = NULL;
	struct G *mst_g = NULL;
	struct weight_e *wes = NULL;
	int wes_num = 0;

	/*
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s info_file\n", argv[0]);
		exit(0);
	}
	*/

	sets.head = NULL;
	sets.tail = NULL;
	sets.n = 0;

	g = G_create();
	fprintf(stdout, "create g ok\n");
	mst_get_edge_weights(&wes, &wes_num);
	print_weight_edges(wes, wes_num);
	fprintf(stdout, "get edge weights ok\n");
	mst_g = G_create();
	fprintf(stdout, "create mst_g ok\n");

	mst_kruskal(&sets, g, wes, wes_num, mst_g);

	fprintf(stdout, "result: \n");
	G_print(mst_g);

	return 0;
}





