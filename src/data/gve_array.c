
/*
 * 实现图基本数据结构
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gve_array.h"
#include "array.h"
#include "arrayrep.h"

#define E EdgeA_T
#define G GraphA_T


struct E {
	void *v;		// 开始节点
	void *u;		// 终止节点
};

struct G {
	void *priv;				// 用户数据
	Array_T vs;
	Array_T es;					// 图中边信息 // can use dlist instead
};




/**********************************EDGE************************************/

/*
 * 获取边的priv，
 * @e: 边
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(GraphA, EdgeGetPriv)(E e)
{
	assert(e);

	return (void *)(e + 1);
}

/*
 * 设置边的两个节点，并返回旧的节点, 成功返回0，失败-1，
 * @e: 边
 * @v: 边e的开始节点
 * @u: 边e的终止节点
 * @ovp: 保存边e旧的开始节点
 * @oup: 保存边e旧的终止节点
 */
int MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(E e, void *v, void *u, void **ovp, void **oup)
{
	assert(e);
	assert(v && u);
//	assert(ovp && oup);

	if (ovp)
		*ovp = e->v;
	if (oup)
		*oup = e->u;

	e->v = v;
	e->u = u;

	return 0;
}

/*
 * 获取边的两个节点并保存在@vp，@up，成功返回0，失败-1，
 * @e: 边
 * @vp: 保存边e的开始节点
 * @up: 保存边e的终止节点
 */
int MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(E e, void **vp, void **up)
{
	assert(e);
	assert(vp && up);

	*vp = e->v;
	*up = e->u;

	return 0;
}





/********************************GRAPH******************************/
/*
 * 根据@priv在图@g中查找边，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
E MODULE_FUN_NAME(GraphA, EdgeSearch)(G g, int (*cmp)(void *arg, void *priv), void *priv)
{
	E e = NULL;
	int len = 0;
	int i;
	void *edge_priv = NULL;

	assert(g && cmp);

	len = MODULE_FUN_NAME(Array, length)(g->es);

	for (i = 0; i < len; i++)
	{
		e = (E)MODULE_FUN_NAME(Array, get)(g->es, i);
		edge_priv = MODULE_FUN_NAME(GraphA, EdgeGetPriv)(e);
		if ((e) && (cmp(edge_priv, priv) == 0))
		{
			return e;
		}
	}

	return NULL;
}

E MODULE_FUN_NAME(GraphA, EdgeGet)(G g, int i)
{
	assert(g);
	return (E)MODULE_FUN_NAME(Array, get)(g->es, i);
}

E MODULE_FUN_NAME(GraphA, EdgePut)(G g, int i, E e)
{
	assert(g);
	return (E)MODULE_FUN_NAME(Array, put)(g->es, i, (void *)e);
}

int MODULE_FUN_NAME(GraphA, EdgesLength)(G g)
{
	assert(g);
	return MODULE_FUN_NAME(Array, length)(g->es);
}


/*
 * 根据@priv在图@g中查找节点，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
void *MODULE_FUN_NAME(GraphA, VnodeSearch)(G g, int (*cmp)(void *arg, void *priv), void *priv)
{
	void *node = NULL;
	int len = 0;
	int i;

	assert(g && cmp);

	len = MODULE_FUN_NAME(Array, length)(g->vs);

	for (i = 0; i < len; i++)
	{
		node = MODULE_FUN_NAME(Array, get)(g->vs, i);
		if ((node) && (cmp(node, priv) == 0))
			return node;
	}

	return NULL;
}

void *MODULE_FUN_NAME(GraphA, VnodeGet)(G g, int i)
{
	assert(g);
	return MODULE_FUN_NAME(Array, get)(g->vs, i);
}

void *MODULE_FUN_NAME(GraphA, VnodePut)(G g, int i, void *v)
{
	assert(g);
	return MODULE_FUN_NAME(Array, put)(g->vs, i, v);
}

int MODULE_FUN_NAME(GraphA, VnodesLength)(G g)
{
	assert(g);
	return MODULE_FUN_NAME(Array, length)(g->vs);
}

/*
 * 创建一个最大包含@size个节点的图,成功返回指针，失败NULL
 * @node_size: 节点占内存大小
 * @node_len:  图中节点个数
 * @edge_size： 边占内存大小
 * @edge_len:  图中边个数
 * @priv:   私有数据
 */
G MODULE_FUN_NAME(GraphA, create)(int node_size, int node_len, int edge_size, int edge_len, void *priv)
{
	G g = NULL;
	void *nodes_p = NULL;
	void *edges_p = NULL;

	assert(node_size > 0);
	assert(edge_size > 0);

	assert(node_len >= 0);
	assert(edge_len >= 0);

	g = (G)calloc(1, sizeof(*g)); 
	if (g)
	{
		g->vs = (Array_T)calloc(1, sizeof(*(g->vs)));
		if (g->vs == NULL)
		{
			free(g);
			return NULL;
		}
		
		g->es = (Array_T)calloc(1, sizeof(*(g->es)));
		if (g->es == NULL)
		{
			free(g->vs);
			free(g);
			return NULL;
		}

		nodes_p = (void *)calloc(node_len, node_size);
		edges_p = (void *)calloc(edge_len, sizeof(struct E) + edge_size);
		if (nodes_p == NULL || edges_p == NULL)
		{
			if (nodes_p) free(nodes_p);
			if (edges_p) free(edges_p);
			free(g->vs);
			free(g->es);
			free(g);
			return NULL;
		}

		MODULE_FUN_NAME(ArrayRep, init)(g->vs, node_len, node_size, nodes_p);
		MODULE_FUN_NAME(ArrayRep, init)(g->es, edge_len, sizeof(struct E) + edge_size, edges_p);
		g->priv = priv;
	}

	return g;
}

/*
 * 释放图@gp的内容
 * gp: 图指针
 */
void MODULE_FUN_NAME(GraphA, free)(G *gp)
{
	assert(gp && *gp);

	MODULE_FUN_NAME(Array, free)(&((*gp)->vs));
	MODULE_FUN_NAME(Array, free)(&((*gp)->es));
	free(*gp);
	*gp = NULL;
}

G MODULE_FUN_NAME(GraphA, copy)(G g)
{
	G copy = NULL;

	assert(g);

	copy = (G)calloc(1, sizeof(*copy)); 
	if (copy)
	{
		copy->priv = g->priv;
		copy->vs = MODULE_FUN_NAME(Array, copy)(g->vs, g->vs->length);
		if (copy->vs == NULL)
		{
			free(copy);
			return NULL;
		}

		copy->es = MODULE_FUN_NAME(Array, copy)(g->es, g->es->length);
		if (copy->es == NULL)
		{
			MODULE_FUN_NAME(Array, free)(&(copy->vs));
			free(copy);
			return NULL;
		}
	}

	return copy;
}


