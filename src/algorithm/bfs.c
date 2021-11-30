
/*
 * 实现图的广度优先遍历
 * 有向图无详图均可
 * date: 2021-11-07
 * author: lin
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "bfs.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t

/*********************************************************/
/*
 * you xiang & wu xiang tu jun ke
 * g: 图数据结构
 * k: 使用k节点作为开始遍历节点, index == k的节点
 */
int bfs(GraphA_T g, int k, int (*cb)(void *arg, void *priv), void *arg)
{
	int count = 0;
	NODE u = NULL;
	NODE v = NULL;
	NODE s = NULL;
	NODE unused = NULL;
	EdgeA_T edge = NULL;
	Queue_T q = NULL;

	count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);

	q = (Queue_T )MODULE_FUN_NAME(Queue, new)(count);
	if (q == NULL)
	{
		return -1;
	}

	for (int i = 0; i < count; i++)
	{
		u = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (u->index != k)
		{
			u->color = WHITE;
			u->d = INFINITE;
			u->prev = NULL;
		}
		else
		{
			s = u;
		}
	}

	s->color = GRAY;
	s->d = 0;
	s->prev = NULL;

	MODULE_FUN_NAME(Queue, en)(q, (void *)s);

	while (MODULE_FUN_NAME(Queue, isEmpty)(q) == 0)
	{
		int count = 0;
		
		MODULE_FUN_NAME(Queue, de)(q, (void **)&u);
		count = MODULE_FUN_NAME(SenDlink, count)(u->l);

		for (int i = 0; i < count; i++)
		{
			edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(u->l, i);
			MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&unused, (void **)&v);

			if (v->color == WHITE)
			{
				v->color = GRAY;
				v->d = u->d + 1;
				v->prev = u;
				MODULE_FUN_NAME(Queue, en)(q, (void *)v);
			}
		}
		u->color = BLACK;

		if (cb)
		{
			cb(arg, (void *)u);
		}
	}

	return 0;
}

