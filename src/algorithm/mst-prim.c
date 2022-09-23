
/*
 * 最小生成树
 * 连通无向图
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mst-prim.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"
#include "sentinel-linked-list.h"
#include "set-list.h"
#include "quicksort.h"
#include "min-heapify.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t

#define POSITIVE_INFINITE	777777
#define NEGTIVE_INFINITE	(-777777)

static int min_heap_cmp(void *arg1, void *arg2)
{
	NODE node1 = (NODE)arg1;
	NODE node2 = (NODE)arg2;

	if (node1->key > node2->key) return 1;
	else if (node1->key < node2->key) return -1;
	else return 0;
}

static int min_heap_decrease(void **priv, void *arg)
{
	int *parg = (int *)arg;
	NODE node = (NODE)(*priv);

	node->key = *parg;
//	fprintf(stdout, "%s: set key: %d, %d\n", __func__, node->index, node->key);
	return 0;
}

static int min_heap_search(void *priv, void *arg)
{
	if (priv == arg) return 0;
	else return -1;
}

/*
 * 计算连通图g的最小生成树
 * 最小生成树的边存放在edges中
 *  algorith for charpter 23-2
 *  最后得到的边集合是以r为根的一颗边树
 *  无向图的边u v需要用两条来表示一个为u,v;另一个为v,u
 *  这是由于node->l中的边表示的是有向的，为从节点node出发
 * */
int mst_prim(SenDlink_T edges, GraphA_T g, NODE r)
{
	int node_count = 0;
	NODE node = NULL;
	MinHeap_T min_heap = NULL;
	void **array = NULL;
	NODE v = NULL;
	NODE u = NULL;
	EdgeA_T edge = NULL;
	EDGE_EXT edge_ext = NULL;
	int i;

	/*
	 * initialize
	 */
	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (i = 0; i < node_count; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		node->key = POSITIVE_INFINITE;
		node->prev = NULL;
	}

	r->key = 0;

	/*
	 * create min priority queue
	 */
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

//	fprintf(stdout, "%s: create min heap\n", __func__);
	min_heap = MODULE_FUN_NAME(MinHeap, new)(array, node_count, min_heap_cmp, min_heap_decrease);
//	fprintf(stdout, "%s: create min heap done\n", __func__);

	/*
	 * do the job
	 */
	while ((node = (NODE)MODULE_FUN_NAME(MinHeap, extract_min)(min_heap)) != NULL)
	{
//		fprintf(stdout, "%s: min: %d, key: %d\n", __func__, node->index, node->key);
		/*
		 * if node->prev is not NULL, then add edge which nodes are node and node->prev to edges
		 */
		if (node->prev)
		{
			int edge_count = 0;

			edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
			for (i = 0; i < edge_count; i++)
			{
				edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
//				edge_ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
				MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&u, (void **)&v);
			//	if ((u == node && v == node->prev) || (u == node->prev && v == node))
				if ((u == node && v == node->prev))
				{
//					fprintf(stdout, "%s: add edge: %d\n", __func__, edge_ext->value);
					MODULE_FUN_NAME(SenDlink, insert)(edges, (void *)edge);
				}
			}
		}

		/*
		 * update nodes
		 */
		int count = MODULE_FUN_NAME(SenDlink, count)(node->l);
		for (i = 0; i < count; i++)
		{
			int index = -1;
			edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(node->l, i);
			MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&u, (void **)&v);
//			fprintf(stdout, "%s: node: %d, u: %d, v: %d\n", __func__, node->index, u->index, v->index);
			edge_ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
			if ((index = MODULE_FUN_NAME(MinHeap, search)(min_heap, min_heap_search, v)) >= 0 && edge_ext->value < v->key)
			{
				MODULE_FUN_NAME(MinHeap, decrease_key)(min_heap, index, (void *)(&(edge_ext->value)));
//				v->key = edge_ext->value;
				v->prev = u;
			}
		}
	}


	/*
	 * release resouce we allocated
	 */
	MODULE_FUN_NAME(MinHeap, free)(&min_heap, 0);
	free(array);

	return 0;
}

