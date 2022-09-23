
/*
 * dijkstra 方法计算单源最短路径
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "min-heapify.h"
#include "dijkstra.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t
#define INFINITE	77777

static int min_heap_cmp(void *arg1, void *arg2)
{
	NODE node1 = (NODE)arg1;
	NODE node2 = (NODE)arg2;

	if (node1->d > node2->d) return 1;
	else if (node1->d < node2->d) return -1;
	else return 0;
}

static int min_heap_decrease(void **priv, void *arg)
{
	int *parg = (int *)arg;
	NODE node = (NODE)(*priv);

	node->d = *parg;
	return 0;
}

static int min_heap_search(void *priv, void *arg)
{
	if (priv == arg) return 0;
	else return -1;
}

static int initialize_single_source(GraphA_T g, NODE s)
{
	NODE node = NULL;
	int node_count = 0;
	int i;

	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (i = 0; i < node_count; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		node->d = INFINITE;
		node->prev = NULL;
	}

	s->d = 0;

	return 0;
}

/*
 * @w: weight of edge of u and v
 */
static int relax(NODE u, NODE v, EdgeA_T edge)
{
	EDGE_EXT ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
	if (v->d > u->d + ext->value)
	{
		v->d = u->d + ext->value;
		v->prev = u;
	}

	return 0;
}

/*
 *
 * 计算有向图g中的节点到s的最短路径,计算得到的路径值存放在node的d域
 * 其中边的权重大于等于0
 * you xiang tu, bian de quan zhong ke yi wei fu zhi 
 *
 * return value: 0: negative loop; 1: ok
 *
 */
int dijkstra(GraphA_T g, NODE s)
{
	int node_count = 0;
	int edge_count = 0;
	void **array = NULL;
	MinHeap_T min_heap = NULL;
	NODE node = NULL;
	NODE v = NULL;
	NODE u = NULL;
	EdgeA_T edge = NULL;
	EDGE_EXT edge_ext = NULL;
	int i;

	/*
	 * init g
	 */
	initialize_single_source(g, s);

	/*
	 * create min heap
	 */
	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);

	array = (void **)calloc(node_count, sizeof(void *));
	if (array == NULL)
	{
		fprintf(stdout, "%s: no mem for array: %d\n", __func__, node_count);
		return -1;
	}

	for (i = 0; i < node_count; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		array[i] = node;
	}

	min_heap = MODULE_FUN_NAME(MinHeap, new)(array, node_count, min_heap_cmp, min_heap_decrease);

	/*
	 * do the job
	 */
	while ((node = (NODE)MODULE_FUN_NAME(MinHeap, extract_min)(min_heap)) != NULL)
	{
		int index = -1;
		int count = MODULE_FUN_NAME(SenDlink, count)(node->l);
		for (i = 0; i < count; i++)
		{
			int index = -1;
			edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(node->l, i);
			MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&u, (void **)&v);
			edge_ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
			relax(u, v, edge);
			index = MODULE_FUN_NAME(MinHeap, search)(min_heap, min_heap_search, v);
			MODULE_FUN_NAME(MinHeap, decrease_key)(min_heap, index, (void *)(&(v->d)));
		}
	}

	/*
	 * release resouse
	 */
	MODULE_FUN_NAME(MinHeap, free)(&min_heap, 0);
	free(array);

#if 0
	/*
	 * check
	 */
	for (int j = 0; j < edge_count; j++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, j);
		edge_ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)(&u), (void **)(&v));

		if (v->d > u->d + edge_ext->value)
		{
			return 0;
		}
	}
#endif

	return 1;
}


