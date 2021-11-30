
/*
 * 实现图的点边从文件读取
 * 
 * date: 2021-11-30
 * author: lin
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t


/***************************print****************************/
void gve_V_print(NODE v)
{
	NODE u = NULL;
	int len = 0;
	EdgeA_T edge = NULL;
	NODE unused = NULL;

	assert(v);
	fprintf(stdout, "v: %d, color: %d, p: %p, d: %d, f: %d, es: %d\n",
					v->index, v->color, v->prev, v->d, v->f, MODULE_FUN_NAME(SenDlink, count)(v->l));
	len = MODULE_FUN_NAME(SenDlink, count)(v->l);
	for (int i = 0; i < len; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(v->l, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&unused, (void **)&u);
		fprintf(stdout, "v: %d, color: %d, p: %p, d: %d, f: %d\n",
					u->index, u->color, u->prev, u->d, u->f);
	}

}

void gve_E_print(EDGE_EXT edge)
{
}

void gve_G_print(GraphA_T g)
{
	int len = 0;
	NODE node = NULL;

	assert(g);

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		fprintf(stdout, "\n");
		gve_V_print(node);
		fprintf(stdout, "\n");
	}
}

/*
 * 打印从原点到k的路径
 */
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

/*
 * 打印从s到v的路径
 */
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
static GraphA_T gve_get_graphA(FILE *fp)
{
	GraphA_T g = NULL;
	int node_item = 0;
	int edge_item = 0;

	fscanf(fp, "%d", &node_item);
	fscanf(fp, "%d", &edge_item);

	g = MODULE_FUN_NAME(GraphA, create)(sizeof(struct NODE), node_item, sizeof(struct EDGE_EXT), edge_item, NULL);

	return g;
}

static int gve_get_node(FILE *fp, void *g_p, void *n_p)
{
	int v = 0;
	NODE node = (NODE )n_p;
	GraphA_T g = (GraphA_T)g_p;

	fscanf(fp, "%d", &v);

	node->index = v;
	node->l = MODULE_FUN_NAME(SenDlink, create)();

	return 0;
}

static int gve_get_edge(FILE *fp, void *g_p, void *e_p)
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

GraphA_T gve_create_graph(char *filename)
{
	assert(filename);

	GraphA_T g = NULL;

	g = GA_create(filename, gve_get_graphA, gve_get_edge, gve_get_node);

	return g;
}


