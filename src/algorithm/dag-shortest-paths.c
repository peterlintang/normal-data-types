
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dag-shortest-paths.h"
#include "topological-sort.h"

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
 * you xiang tu, bian de quan zhong ke yi wei fu zhi 
 *
 * return value: 0: negative loop; 1: ok
 *
 */
int dag_shortest_paths(GraphA_T g, NODE s)
{
	int node_count = 0;
	int edge_count = 0;
	NODE node = NULL;
	NODE u = NULL;
	NODE v = NULL;
	EdgeA_T edge = NULL;
	EDGE_EXT ext = NULL;
	SenDlink_T l = NULL;

	l = MODULE_FUN_NAME(SenDlink, create)();

	topological_sort(g, l);
	topological_sort_print(l);

	initialize_single_source(g, s);

	edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	node_count = MODULE_FUN_NAME(SenDlink, count)(l);

	for (int i = 0; i < node_count; i++)
	{
		int count = 0;
		node = (NODE)MODULE_FUN_NAME(SenDlink, get)(l, i);

		count = MODULE_FUN_NAME(SenDlink, count)(node->l);
		for (int j = 0; j < count; j++)
		{
			edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(node->l, j);
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

	MODULE_FUN_NAME(SenDlink, destroy)(&l);

	return 1;
}



