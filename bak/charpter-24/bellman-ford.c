
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


static int initialize_single_source(struct G *g, int s)
{
	for (int i = 0; i < g->vs_num; i++)
	{
		g->vs[i]->d = INFINITE;
		g->vs[i]->p = NULL;
	}

	g->vs[s]->d = 0;
}

/*
 * @w: weight of edge of u and v
 */
static int relax(struct V *u, struct V *v, struct weight_e *w)
{
	if (v->d > u->d + w->w)
	{
		v->d = u->d + w->w;
		v->p = u;
	}

	return 0;
}

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


static struct weight_e *find_weight(
				struct weight_e *ws, 
				int ws_num,
				int u, int v)
{
	for (int i = 0; i < ws_num; i++)
	{
		if ((ws[i].u == u) && (ws[i].v == v))
			return &(ws[i]);
	}

	return NULL;
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
 *
 * you xiang tu, bian de quan zhong ke yi wei fu zhi 
 *
 * return value: 0: negative loop; 1: ok
 *
 */
int bellman_ford(struct G *g, struct weight_e *ws, int ws_num, int s)
{
	initialize_single_source(g, s);

	for (int i = 1; i < g->vs_num - 1; i++)
	{
		for (int j = 0; j < g->vs_num; j++)
		{
			struct item *cur = NULL;
			struct V *u = g->vs[j];
			struct V *v = NULL;
			struct weight_e *w = NULL;

		    for (cur = u->l.nil.next; cur != &(u->l.nil); cur = cur->next)
			{
				v = (struct V *)cur->key;
				w = find_weight(ws, ws_num, u->v, v->v);
				if (w == NULL)
				{
					fprintf(stderr, "%s: edge(%d,%d) not found\n",
									__func__,
									u->v,
									v->v);
					exit(-1);
				}
				relax(u, v, w);
			}
		}
	}

	for (int j = 0; j < g->vs_num; j++)
	{
		struct item *cur = NULL;
		struct V *u = g->vs[j];
		struct V *v = NULL;
		struct weight_e *w = NULL;

	    for (cur = u->l.nil.next; cur != &(u->l.nil); cur = cur->next)
		{
			v = (struct V *)cur->key;
			w = find_weight(ws, ws_num, u->v, v->v);
			if (v->d > u->d + w->w)
				return 0;
		}
	}

	return 1;
}



int main(int argc, char *argv[])
{
	int ret = 0;
	struct G *g = NULL;
	struct weight_e *ws = NULL;
	int ws_num = 0;
	int s = 0;

	g = G_create();
	mst_get_edge_weights(&ws, &ws_num);
	scanf("%d", &s);

	s = s - 1;
	G_print(g);
	print_weight_edges(ws, ws_num);

	fprintf(stdout, "call bellman for: %d\n", s);
	
	ret = bellman_ford(g, ws, ws_num, s);
	if (0 == ret)
	{
		fprintf(stdout, "g has a negative loop\n");
	}
	G_print(g);

	return ret;
}









