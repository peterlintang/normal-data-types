
/*
 * 计算图的连通分量
 * 用set-list实现
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dlist.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "set-list.h"
#include "components.h"


static void *get_graphA(FILE *fp)
{
	GraphA_T g = NULL;
	int node_item = 0;
	int edge_item = 0;

	fscanf(fp, "%d", &node_item);
	fscanf(fp, "%d", &edge_item);
	fprintf(stdout, "%s: node: %d, edge: %d\n", __func__, node_item, edge_item);

	g = MODULE_FUN_NAME(GraphA, create)(sizeof(struct node), node_item, sizeof(struct edge_ext), edge_item, NULL);

	return g;
}

static int get_node(FILE *fp, void *g_p, void *n_p)
{
	int v = 0;
	struct node *node = (struct node *)n_p;
	GraphA_T g = (GraphA_T)g_p;

	fscanf(fp, "%d", &v);

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

	fscanf(fp, "%d,%d,%d", &v, &u, &value);

	n_v = MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)v);
	n_u = MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)u);
	if (n_v == NULL || n_u == NULL)
	{
		fprintf(stdout, "search %d, %d failed %p, %p\n", v, u, n_v, n_u);
		return -1;
	}

	MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)n_v, (void *)n_u, NULL, NULL);
	ext = (struct edge_ext *)MODULE_FUN_NAME(GraphA, EdgeGetPriv)((void *)edge);
	ext->value = value;
	return 0;
}

GraphA_T components_create_graph(char *filename)
{
	assert(filename);

	GraphA_T g = NULL;

	g = GA_create(filename, get_graphA, get_edge, get_node);

	return g;
}

/**********************************************************/
static int set_cmp(void *priv, void *arg)
{
	if (priv == arg) return 0;
	else return 1;
}

static SetL_T new_set(void *node)
{
	int ret = 0;
	SetL_T set = NULL;

	set = MODULE_FUN_NAME(SetL, new)(set_cmp);
	if (set == NULL)
	{
		return NULL;
	}

	ret = MODULE_FUN_NAME(SetL, add)(set, node);
	if (ret != 0)
	{
		MODULE_FUN_NAME(SetL, free)(&set);
		return NULL;
	}

	return set;
}

static int insert_set_to_sets(ListD_T sets, SetL_T set)
{
	ListDNode_T node = NULL;

	node = MODULE_FUN_NAME(ListDNode, new)((void *)set);
	if (node == NULL)
	{
		return -1;
	}

	MODULE_FUN_NAME(ListD, insert)(sets, (void *)node);
	return 0;
}

SetL_T set_find(ListD_T sets, void *p)
{
	int count = 0;
	ListDNode_T cur = NULL;

	for (int i = 0; i < count; i++)
	{
		cur = MODULE_FUN_NAME(ListD, get)(sets, i);
		if (MODULE_FUN_NAME(SetL, isMember)((SetL_T)(cur->priv), p))
			return (SetL_T)(cur->priv);
	}

	return NULL;
}

static int listd_cmp(void *priv, void *arg)
{
	if (priv == arg)
		return 0;
	else
		return 1;
}

static int set_union(ListD_T sets, SetL_T set_v, SetL_T set_u)
{
	SetL_T set = NULL;
	ListDNode_T v = NULL;
	ListDNode_T u = NULL;
	ListDNode_T node = NULL;

	set = MODULE_FUN_NAME(SetL, union)(set_v, set_u);

	v = MODULE_FUN_NAME(ListD, search)(sets, listd_cmp, set_v);
	u = MODULE_FUN_NAME(ListD, search)(sets, listd_cmp, set_u);

	MODULE_FUN_NAME(ListD, remove)(sets, v);
	MODULE_FUN_NAME(ListD, remove)(sets, u);

	node = MODULE_FUN_NAME(ListDNode, new)(set);
	MODULE_FUN_NAME(ListD, insert)(sets, node);

	MODULE_FUN_NAME(ListDNode, free)(&v);
	MODULE_FUN_NAME(ListDNode, free)(&u);

	MODULE_FUN_NAME(SetL, free)(&set_v);
	MODULE_FUN_NAME(SetL, free)(&set_u);

	return 0;
}

/*
 * 计算图g的连通分量,图最后的连通分量保存在链表sets中，
 * 链表中每一个元素代表一个连通分量
 * g: 有节点信息，边信息的图
 * sets: 空链表
 */
void components_connected(ListD_T sets, GraphA_T g)
{
	SetL_T set = NULL;
	SetL_T set_v = NULL;
	SetL_T set_u = NULL;
	int node_count = 0;
	int edge_count = 0;
	EdgeA_T edge = NULL;
	void *node = NULL;
	void *v = NULL;
	void *u = NULL;

	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < node_count; i++)
	{
		node = MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);

		set = new_set(node);

		insert_set_to_sets(sets, set);
	}

	edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	for (int i = 0; i < edge_count; i++)
	{
		edge = MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, &v, &u);
		set_v = set_find(sets, v);
		set_u = set_find(sets, u);
		if (set_v == set_u)
		{
			set_union(sets, set_v, set_u);
		}
	}
}

int components_same(ListD_T sets, void *x, void *y)
{
	SetL_T x_set = NULL;
	SetL_T y_set = NULL;

	x_set = set_find(sets, x);
	y_set = set_find(sets, y);

	if (x_set == y_set)
		return 1;
	else
		return 0;
}

