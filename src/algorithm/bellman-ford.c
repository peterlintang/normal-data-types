
/*
 * bellman ford方法计算单源最短路径
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bellman-ford.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t
#define INFINITE	77777

static int initialize_single_source(GraphA_T g, NODE s)
{
	NODE node = NULL;
	int node_count = 0;

	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < node_count; i++)
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
 * you xiang tu, bian de quan zhong ke yi wei fu zhi 
 *
 * return value: 0: negative loop; 1: ok
 *
 */
int bellman_ford(GraphA_T g, NODE s)
{
	int node_count = 0;
	int edge_count = 0;
	NODE node = NULL;
	NODE u = NULL;
	NODE v = NULL;
	EdgeA_T edge = NULL;
	EDGE_EXT ext = NULL;

	initialize_single_source(g, s);

	edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g) - 1;
	for (int i = 0; i < node_count; i++)
	{
		for (int j = 0; j < edge_count; j++)
		{
			edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, j);
//			ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
			MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)(&u), (void **)(&v));

			relax(u, v, edge);
		}
	}

	for (int j = 0; j < edge_count; j++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, j);
		ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)(&u), (void **)(&v));

		if (v->d > u->d + ext->value)
		{
			return 0;
		}
	}

	return 1;
}


