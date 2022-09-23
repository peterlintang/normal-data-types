
/*
 * 计算图的连通分量
 * 用set-list实现
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dlist.h"
#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "set-list.h"
#include "components.h"
#include "quicksort.h"
#include "dfs.h"

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t

/**********************************************************/
static int _set_cmp(void *priv, void *arg)
{
//	fprintf(stdout, "%s: priv: %p, arg: %p\n", __func__, priv, arg);
	if (priv == arg) return 0;
	else return 1;
}

static SetL_T new_set(void *node)
{
	int ret = 0;
	SetL_T set = NULL;

	set = MODULE_FUN_NAME(SetL, new)(_set_cmp);
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

SetL_T set_find(ListD_T sets, NODE p)
{
	int i;
	int count = 0;
	ListDNode_T cur = NULL;

	count = MODULE_FUN_NAME(ListD, count)(sets);
	for (i = 0; i < count; i++)
	{
		cur = MODULE_FUN_NAME(ListD, get)(sets, i);
//		fprintf(stdout, "%s: i: %d, cur: %p, priv: %p, p: %p\n",
//						__func__, i, cur, cur->priv, p);
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

//	fprintf(stdout, "%s: union set_v: %p, set_u: %p\n", __func__, set_v, set_u);

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
	int ret = 0;
	EDGE_EXT ext = NULL;
	EdgeA_T edge = NULL;
	void *node = NULL;
	void *v = NULL;
	void *u = NULL;
	int i;

	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (i = 0; i < node_count; i++)
	{
		node = MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);

		set = new_set(node);

		ret = insert_set_to_sets(sets, set);
		if (ret != 0)
		{
			fprintf(stdout, "%s: i: %d, set: %p, ret: %d\n", 
							__func__, i, set, ret);
		}
	//	fprintf(stdout, "%s: i: %d, set: %p, ret: %d\n", 
	//					__func__, i, set, ret);
	}

//	fprintf(stdout, "%s ********************\n", __func__);
	edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	for (i = 0; i < edge_count; i++)
	{
		edge = MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, &v, &u);
		ext = MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
		set_v = set_find(sets, v);
		set_u = set_find(sets, u);
//		fprintf(stdout, "%s i: %d, edge: %p, v: %p, set_v: %p, u: %p, set_u: %p, value: %d\n", 
//						__func__, i, edge, v, set_v, u, set_u, ext->value);
		if (set_v != set_u)
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

/*
 * 反置图g的边
 * 即原来有边u->v变成v->u
 */
GraphA_T convert_graphA_edges(GraphA_T g)
{
	GraphA_T copy = NULL;

	assert(g);

	copy = MODULE_FUN_NAME(GraphA, copy)(g);
	if (copy)
	{
		int node_count = 0;
		int edge_count = 0;
		int i;
		int j;
		NODE node = NULL;
		EdgeA_T edge = NULL;
		NODE v = NULL;
		NODE u = NULL;
		NODE v_c = NULL;
		NODE u_c = NULL;

		node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(copy);
		edge_count = MODULE_FUN_NAME(GraphA, EdgesLength)(copy);

		for (i = 0; i < node_count; i++)
		{
			node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(copy, i);
			node->l = MODULE_FUN_NAME(SenDlink, create)();
			if (node->l == NULL)
			{
				fprintf(stdout, "%s: i: %d failed to create link list\n", __func__, i);
			}
		}

		for (i = 0; i < edge_count; i++)
		{
			edge = MODULE_FUN_NAME(GraphA, EdgeGet)(copy, i);
			MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&v, (void **)&u);

			v_c = NULL;
			u_c = NULL;

			for (j = 0; j < node_count; j++)
			{
				node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(copy, j);
				if (node->index == v->index)
				{
					v_c = node;
				}
				if (node->index == u->index)
				{
					u_c = node;
				}
				if (v_c && u_c)
				{
					break;
				}
			}

//			fprintf(stdout, "%s: u: %d, v: %d, u_c: %d, v_c: %d\n", __func__, u->index, v->index, u_c->index, v_c->index);

			MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)u_c, (void *)v_c, NULL, NULL);
			MODULE_FUN_NAME(SenDlink, insert)(u_c->l, (void *)edge);
		}
	}

	return copy;
}

static int node_cmp(void *arg1, void *arg2)
{
	NODE v = (NODE)arg1;
	NODE u = (NODE)arg2;

//	fprintf(stdout, "%s: v: %d, %d, u: %d, %d\n", __func__, v->index, v->f, u->index, u->f);
	if (v->f > u->f) return 1;
	else if (v->f < u->f) return -1;
	else return 0;
}

static int set_cmp(void *key, void *arg)
{
	NODE v = (NODE)key;
	NODE u = (NODE)arg;

	if (v->index == u->index)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static int graphA_cmp(void *arg, void *priv)
{
	NODE node = (NODE)arg;
	int index = (int)priv;

	if (node->index == index)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/*
 * 计算图g的强连通分量
 * g 有向图
 * sets 连通分量集合
 */
int strongly_components_connected(SenDlink_T sets, GraphA_T g)
{
	assert(g && sets);

	GraphA_T convert = NULL;
	NODE node = NULL;
	SenDlink_T l = NULL;
	void **array = NULL;
	int node_count = 0;
	int i;

	dfs(g, NULL, NULL);

	convert = convert_graphA_edges(g);

	node_count = MODULE_FUN_NAME(GraphA, VnodesLength)(convert);
	array = (void **)calloc(node_count, sizeof(void *));

	for (i = 0; i < node_count; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(convert, i);
		node->color = WHITE;
		node->prev = NULL;
		array[i] = (void *)node;
	}

	/*
	for (int i = 0; i < node_count; i++)
	{
		fprintf(stdout, "%s: i: %d, p: %p\n", __func__, i, array[i]);
	}
	*/

	quicksort(array, node_cmp, 0, node_count - 1);

	for (i = node_count - 1; i >= 0; i--)
	{
		node = (NODE)(array[i]);
		if (node->color == WHITE)
		{
			dfs_visit(convert, node, NULL, NULL);
		}
	}

	l = MODULE_FUN_NAME(SenDlink, create)();
	dfs_G_trees_produce(convert, l);

	int count = MODULE_FUN_NAME(SenDlink, count)(l);
	for (i = 0; i < count; i++)
	{
		SetL_T set = (SetL_T)MODULE_FUN_NAME(SenDlink, get)(l, i);

		int set_count = MODULE_FUN_NAME(SetL, count)(set);
		int j;
		SetL_T new = MODULE_FUN_NAME(SetL, new)(set_cmp);
		NODE v = NULL;

		for (j = 0; j < set_count; j++)
		{
			node = (NODE)MODULE_FUN_NAME(SetL, get)(set, j);
			v = (NODE)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, graphA_cmp, (void *)(node->index));
			if (v == NULL)
			{
				fprintf(stdout, "%s: cant find %d in graph\n", __func__, node->index);
			}
			else
			{
				MODULE_FUN_NAME(SetL, add)(new, v);
			}
		}

		MODULE_FUN_NAME(SenDlink, insert)(sets, new);

		MODULE_FUN_NAME(SetL, free)(&set);
	}

	free(array);
	MODULE_FUN_NAME(SenDlink, destroy)(&l);
	MODULE_FUN_NAME(GraphA, free)(&convert);

	return 0;
}


