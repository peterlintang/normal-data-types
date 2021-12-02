
/*
 * 最小生成树
 * 连通无向图
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"
#include "sentinel-linked-list.h"
#include "set-list.h"
#include "quicksort.h"
#include "mst-kruskal.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t


static int set_cmp(void *priv, void *arg)
{
	if (priv == arg)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

static int edge_cmp(void *arg1, void *arg2)
{
	EdgeA_T edge1 = NULL;
	EDGE_EXT edge_ext1 = NULL;
	EdgeA_T edge2 = NULL;
	EDGE_EXT edge_ext2 = NULL;

	edge1 = (EdgeA_T)arg1;
	edge2 = (EdgeA_T)arg2;

	edge_ext1 = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge1);
	edge_ext2 = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge2);

	if (edge_ext1->value > edge_ext2->value) return 1;
	else if (edge_ext1->value < edge_ext2->value) return -1;
	else return 0;
}

static int sendlink_search(void *priv, void *arg)
{
	SetL_T set = (SetL_T)priv;
	NODE node = (NODE)arg;
	int count = 0;

	count = MODULE_FUN_NAME(SetL, count)(set);
	for (int i = 0; i < count; i++)
	{
		NODE member = MODULE_FUN_NAME(SetL, get)(set, i);
		if (member == node)
			return 0;
	}

	return -1;
}

/*
 *
 *  algorith for charpter 23-2
 * */
int mst_kruskal(SenDlink_T edges, GraphA_T g)
{
	assert(g);
	assert(edges);

	SenDlink_T sets = NULL;
	NODE node = NULL;
	NODE v = NULL;
	NODE u = NULL;
	EdgeA_T edge = NULL;
	EDGE_EXT edge_ext = NULL;
	int node_count = 0;
	int edge_count = 0;
	void **array = NULL;

	sets = MODULE_FUN_NAME(SenDlink, create)();
	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < node_count; i++)
	{
		SetL_T set = NULL;

		set = MODULE_FUN_NAME(SetL, new)(set_cmp);
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		MODULE_FUN_NAME(SetL, add)(set, (void *)node);
		MODULE_FUN_NAME(SenDlink, insert)(sets, (void *)set);
	}

	edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	array = (void **)calloc(edge_count, sizeof(void *));
	for (int i = 0; i < edge_count; i++)
	{
		array[i] = (void *)MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
	}
	quicksort(array, edge_cmp, 0, edge_count - 1);

	for (int i = 0; i < edge_count; i++)
	{
		SetL_T set_v = NULL;
		SetL_T set_u = NULL;
		SetL_T new_set = NULL;

		edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&v, (void **)&u);
		MODULE_FUN_NAME(SenDlink, search)(sets, sendlink_search, v, (void **)(&set_v));
		MODULE_FUN_NAME(SenDlink, search)(sets, sendlink_search, u, (void **)(&set_u));
		if (set_v != set_u)
		{
			MODULE_FUN_NAME(SenDlink, insert)(edges, (void *)edge);
			new_set = MODULE_FUN_NAME(SetL, union)(set_v, set_u);
			MODULE_FUN_NAME(SenDlink, delete)(sets, set_cmp, (void *)set_v);
			MODULE_FUN_NAME(SenDlink, delete)(sets, set_cmp, (void *)set_u);
			MODULE_FUN_NAME(SetL, free)(&set_v);
			MODULE_FUN_NAME(SetL, free)(&set_u);
			MODULE_FUN_NAME(SenDlink, insert)(sets, new_set);
		}
	}

	free(array);
	int count = MODULE_FUN_NAME(SenDlink, count)(sets);
	for (int i = 0; i < count; i++)
	{
		SetL_T set = (SetL_T)MODULE_FUN_NAME(SenDlink, get)(sets, i);
		MODULE_FUN_NAME(SetL, free)(&set);
	}
	MODULE_FUN_NAME(SenDlink, destroy)(&sets);

	return 0;
}

#if 0
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
#endif





