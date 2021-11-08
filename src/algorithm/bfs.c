
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

#define NODE bfs_node_t
#define EDGE_EXT bfs_edge_ext_t

/*
typedef struct NODE *NODE;
typedef struct EDGE_EXT *EDGE_EXT;

struct NODE {
	int index;
	int color;
	int d;
	int f;
	NODE prev;
	SenDlink_T l;
};

struct EDGE_EXT {
	int value;
};
*/


/*********************************************************/
/*
 * you xiang & wu xiang tu jun ke
 * g: 图数据结构
 * k: 使用k节点作为开始遍历节点, index == k的节点
 */
int bfs(GraphA_T g, int k)
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
	}

	return 0;
}

/***************************print****************************/
void G_tree_print(GraphA_T g, int k)
{
	int count = 0;
	NODE v = NULL;

	count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < count; i++)
	{
		v = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (v->index == k)
		{
			break;
		}
	}

	while (v)
	{
		fprintf(stdout, "v: %d, d: %d\n", v->index, v->d);
		v = v->prev;
	}
}

static void _G_tree_print_path(GraphA_T g, NODE s, NODE v)
{
	if (s == v)
	{
		fprintf(stdout, "%d ", s->index);
	}
	else if (v->prev == NULL)
	{
		fprintf(stdout, "no path from %d to %d exist\n", 
			s->index, v->index);
	}
	else
	{
		_G_tree_print_path(g, s, v->prev);
		fprintf(stdout, "%d ", v->index);
	}
}

void G_tree_print_path(GraphA_T g, int s, int v)
{
	int count = 0;
	NODE s_p = NULL;
	NODE v_p = NULL;
	NODE u = NULL;

	assert(g);
	assert(s != v);

	count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < count; i++)
	{
		u = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (u->index == s)
		{
			s_p = u;
		}
		if (u->index == v)
		{
			v_p = u;
		}
		if (s_p && v_p)
		{
			break;
		}
	}

	_G_tree_print_path(g, s_p, v_p);
}

/**************************************************************/
/*
 * 图生成函数
 */
static int cmp(void *arg, void *priv)
{
	NODE node = (NODE )arg;
	int *p = (int *)priv;

	if (*p == node->index) return 0;
	else return 1;
}

/*
 * 根据文件内容创建dfs图
 */
static GraphA_T bfs_get_graphA(FILE *fp)
{
	GraphA_T g = NULL;
	int node_item = 0;
	int edge_item = 0;

	fscanf(fp, "%d", &node_item);
	fscanf(fp, "%d", &edge_item);

	g = MODULE_FUN_NAME(GraphA, create)(sizeof(struct NODE), node_item, sizeof(struct EDGE_EXT), edge_item, NULL);

	return g;
}

static int bfs_get_node(FILE *fp, void *g_p, void *n_p)
{
	int v = 0;
	NODE node = (NODE )n_p;
	GraphA_T g = (GraphA_T)g_p;

	fscanf(fp, "%d", &v);

	node->index = v;
	node->l = MODULE_FUN_NAME(SenDlink, create)();

	return 0;
}

static int bfs_get_edge(FILE *fp, void *g_p, void *e_p)
{
	int v = 0;
	int u = 0;
	int value = 0;
	NODE n_v = NULL;
	NODE n_u = NULL;
	EDGE_EXT ext = NULL;
	GraphA_T g = (GraphA_T)g_p;
	EdgeA_T edge = (EdgeA_T)e_p;

	fscanf(fp, "%d,%d,%d", &v, &u, &value);

	n_v = (NODE)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)&v);
	n_u = (NODE)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)&u);
	if (n_v == NULL || n_u == NULL)
	{
		fprintf(stdout, "search %d, %d failed %p, %p\n", v, u, n_v, n_u);
		return -1;
	}

	MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)n_v, (void *)n_u, NULL, NULL);

	ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)((void *)edge);
	ext->value = value;

	MODULE_FUN_NAME(SenDlink, insert)(n_v->l, (void *)edge);

	return 0;
}

GraphA_T bfs_create_graph(char *filename)
{
	assert(filename);

	GraphA_T g = NULL;

	g = GA_create(filename, bfs_get_graphA, bfs_get_edge, bfs_get_node);

	return g;
}


 /* test code
int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = bfs_create_graph(argv[1]);
	bfs(g, 1);
	G_tree_print(g, 3);
//	G_print(g);

	return ret;
}
*/

