
/*
 * 实现深度优先图遍历
 * 有向图无详图都可以
 * date: 2021-11-07
 * author: lin
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "dfs.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t


/*********************************************************/
void dfs_V_tree_produce(GraphA_T g, NODE v, SetL_T set)
{
	int count = 0;
	NODE node = NULL;

	fprintf(stdout, "index: %d, d: %d, f: %d\n", v->index, v->d, v->f);

	MODULE_FUN_NAME(SetL, add)(set, (void *)v);

	count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < count; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if ((node->d > v->d) && (node->f < v->f))
		{
			MODULE_FUN_NAME(SetL, add)(set, (void *)node);
		}
	}
}

static int set_cmp(void *key, void *arg)
{
	NODE v = (NODE )key;
	NODE u = (NODE )arg;

	if (v->index == u->index)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void dfs_G_trees_produce(GraphA_T g, SenDlink_T l)
{
	int len = 0;
	NODE node = NULL;

	assert(g);

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (node->prev == NULL)
		{
			SetL_T set = NULL;
			set = MODULE_FUN_NAME(SetL, new)(set_cmp);
			dfs_V_tree_produce(g, node, set);
			MODULE_FUN_NAME(SenDlink, insert)(l, (void *)set);
		}
	}
}

/*****************dfs**********************/
static int time = 0;

static int dfs_visit(GraphA_T g, NODE u, int (*cb)(void *arg, void *node), void *arg)
{
	int len = 0;
	EdgeA_T edge = NULL;
	NODE v = NULL;
	NODE unused = NULL;


	time = time + 1;
	u->d = time;
	u->color = GRAY;

	len = MODULE_FUN_NAME(SenDlink, count)(u->l);
	for (int i = 0; i < len; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(u->l, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&unused, (void **)&v);
		if (v->color == WHITE)
		{
			v->prev = u;
			dfs_visit(g, v, cb, arg);
		}
	}

	u->color = BLACK;
	time = time + 1;
	u->f = time;

	if (cb)
	{
		cb(arg, (void *)u);
	}

	return 0;
}

/*
 * you xiang & wu xiang tu jun ke
 */
int dfs(GraphA_T g, int (*cb)(void *arg, void *node), void *arg)
{
	int len = 0;
	NODE u = NULL;

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		u = (NODE )MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		u->color = WHITE;
		u->prev = NULL;
	}

	time = 0;

	for (int i = 0; i < len; i++)
	{
		u = (NODE )MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (u->color == WHITE)
		{
			dfs_visit(g, u, cb, arg);
		}
	}

	return 0;
}

