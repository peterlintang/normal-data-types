
/*
 * 计算图的连通分量
 * 用set-list实现
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dlist.h"
#include "gve_array.h"
#include "set-list.h"
#include "components.h"


static void *get_graphA(FILE *fp)
{
	GraphA_T g = NULL;
	int node_item = 0;
	int edge_item = 0;

	scanf(fp, "%d", &node_item);
	scanf(fp, "%d", &edge_item);
	fprintf(stdout, "%s: node: %d, edge: %d\n", __func__, node_item, edge_item);

	g = MODULE_FUN_NAME(GraphA, create)(sizeof(struct node), node_item, sizeof(struct edge_ext), edge_item, NULL);

	return g;
}

static int get_node(FILE *fp, void *g_p, void *n_p)
{
	int v = 0;
	struct node *node = (struct node *)n_p;
	GraphA_T g = (GraphA_T)g_p;

	scanf(fp, "%d", &v);

	node->v = v;

	return 0;
}

static int cmp(void *arg, void *priv)
{
	struct node *node = (struct node *)arg;
	int num = (int)priv;

	if (num == node->v) return 0;
	else return 1;
}

static int get_edge(FILE *fp, void *g_p, void *e_p)
{
	int v = 0;
	int u = 0;
	int value = 0;
	struct node *n_v = NULL;
	struct node *n_u = NULL;
	struct edge_ext *ext = NULL;
	GraphA_T g = (GraphA_T)g_p;
	EdgeA_T edge = (EdgeA_T)e_p;

	scanf(fp, "%d,%d,%d", &v, &u, &value);

	n_v = MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)v);
	n_u = MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)u);
	if (n_v == NULL || n_u == NULL)
	{
		fprintf(stdout, "search %d, %d failed %p, %p\n", n_v, n_u);
		return -1;
	}

	MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)n_v, (void *)n_u, NULL, NULL);
	ext = (struct edge_ext *)MODULE_FUN_NAME(GraphA, EdgeGetPriv)((void *)edge);
	ext->value = value;
	return 0;
}

GraphA_T components_create_graph(char *fliename)
{
	assert(filename);

	GraphA_T g = NULL;

	g = GA_create(filename, get_graphA, get_edge, get_node);

	return g;
}

/*
 * 计算图g的连通分量,图最后的连通分量保存在链表sets中，
 * 链表中每一个元素代表一个连通分量
 * g: 有节点信息，边信息的图
 * sets: 空链表
 */
void components_connected(ListD_T sets, GraphA_T g)
{
	for (int i = 0; i < g->vs_num; i++)
	{
		set_make(sets, g->vs[i].v);
	}

	for (int i = 0; i < g->es_num; i++)
	{
		if (set_find(sets, g->es[i].u) != set_find(sets, g->es[i].v))
		{
			set_union(sets, g->es[i].u, g->es[i].v);
		}
	}
}

int components_same(struct sets *sets, int x, int y)
{
	int x_r = 0;
	int y_r = 0;

	x_r = set_find(sets, x);
	y_r = set_find(sets, y);

	if ((x_r != SET_ERROR) && (x_r == y_r))
		return 1;
	else
		return 0;
}

