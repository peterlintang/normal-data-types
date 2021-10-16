
/*
 * 实现图基本数据结构
 */
#include <stdlib.h>
#include <assert.h>

#include "queue.h"
#include "gve.h"
#include "seq.h"

#define V VNode_T
#define E Edge_T
#define G Graph_T

static int iner_key = 0;

struct V {
	int iner_key;			// 内部分配的记录节点，每个节点的iner_key不一样
	void *priv;				// 用户的数据 (信息)
};


struct E {
	V v;		// 开始节点
	V u;		// 终止节点
	void *priv;	//用户数据
};

struct G {
	void *priv;				// 用户数据
	Seq_T vs;
	int vs_num;				// 图中顶点数组大小
	Seq_T es;					// 图中边信息 // can use dlist instead
	int es_num;				// 图中边数组大小
};



/**********************************VNODE************************************/
/*
 * 创建一个图顶点，成功返回指针，失败返回NULL
 * @v: 用户数据
 */
V MODULE_FUN_NAME(Graph, VnodeCreate)(void *priv)
{
	V v = (V)calloc(1, sizeof(*v));
	if (v)
	{
		v->priv = priv;
		v->iner_key = iner_key++;
	}

	return v;
}

/*
 * 释放一个图顶点，
 * @vp: 节点数据的指针
 */
void MODULE_FUN_NAME(Graph, VnodeFree)(V *vp)
{
	assert(vp && *vp);

	free(*vp);
	*vp = NULL;
}

/*
 * 设置顶点的priv，返回原来的priv
 * @v: 节点数据的指针
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, VnodeSetPriv)(V v, void *priv)
{
	void *old_priv = NULL;

	assert(v);

	old_priv = v->priv;
	v->priv = priv;

	return old_priv;
}

/*
 * 获取顶点的priv，返回原来的priv
 * @v: 节点数据的指针
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, VnodeGetPriv)(V v)
{
	assert(v);

	return v->priv;
}


/**********************************EDGE************************************/

/*
 * 创建一条以@v为开始节点，@u为终止节点的边，成功返回指针，失败返回NULL
 * @v:   边节点
 * @u:   边节点
 * @priv: 用户数据, 可以存放与边有关的信息
 */
E MODULE_FUN_NAME(Graph, EdgeCreate)(V v, V u, void *priv)
{
	E e = NULL;

	assert(v && u);

	e = (E)calloc(1, sizeof(*e));
	if (e)
	{
		e->v = v;
		e->u = u;
		e->priv = priv;
	}

	return e;
}

/*
 * 释放边@ep的内容
 * @ep:   边指针
 */
void MODULE_FUN_NAME(Graph, EdgeFree)(E *ep)
{
	assert(ep && *ep);

	free(*ep);
	*ep = NULL;
}

/*
 * 设置边的priv，返回原来的priv
 * @e: 边
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, EdgeSetPriv)(E e, void *priv)
{
	void *old_priv = NULL;

	assert(e);

	old_priv = e->priv;
	e->priv = priv;

	return old_priv;
}

/*
 * 获取边的priv，
 * @e: 边
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, EdgeGetPriv)(E e)
{
	assert(e);

	return e->priv;
}

/*
 * 设置边的两个节点，并返回旧的节点, 成功返回0，失败-1，
 * @e: 边
 * @v: 边e的开始节点
 * @u: 边e的终止节点
 * @ovp: 保存边e旧的开始节点
 * @oup: 保存边e旧的终止节点
 */
int MODULE_FUN_NAME(Graph, EdgeSetVnodes)(E e, V v, V u, V *ovp, V *oup)
{
	assert(e);
	assert(v && u);
	assert(ovp && oup);

	*ovp = e->v;
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
int MODULE_FUN_NAME(Graph, EdgeGetVnodes)(E e, V *vp, V *up)
{
	assert(e);
	assert(vp && up);

	*vp = e->v;
	*up = e->u;

	return 0;
}

/********************************GRAPH******************************/

/*
 * 添加边@e到图@g中
 * 成功返回0，失败-1
 * 将以@e->v为开始节点，@e->u为终止节点的边@e->v添加到v的链表上
 * 成功返回0，失败返回-1
 * @v:	节点
 * @e:	以@v为开始节点的边 (v->u, 对于有向)
 */
int MODULE_FUN_NAME(Graph, EdgeAdd)(G g, E e)
{
	assert(g && e);
	assert(e->v && e->u);

	MODULE_FUN_NAME(Seq, addhi)(g->es, (void *)e);
//	g->es_num++;

	return 0;
}

/*
 * 将边@e从图@g中删除,
 * 成功返回0，失败-1
 * 将以@e->v为开始节点，@e->u为终止节点的边@e->v的链表上删除
 * 成功返回0，失败返回-1
 * @v:	节点
 * @e:	以@v为开始节点的边 (v->u, 对于有向)
 */
int MODULE_FUN_NAME(Graph, EdgeRemove)(G g, E e)
{
	int len = 0;
	E edge = NULL;

	assert(g && e);
	assert(e->v && e->u);

	len = MODULE_FUN_NAME(Seq, length)(g->es);

	for (int i = 0; i < len; i++)
	{
		edge = (E)MODULE_FUN_NAME(Seq, get)(g->es, i);
		if ((edge) && (edge->v == e->v) && (edge->u == e->u))
		{
			MODULE_FUN_NAME(Seq, put)(g->es, i, NULL);
//			g->es_num--;
			return 0;
		}
	}

	return -1;
}


/*
 * 根据@priv在图@g中查找边，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
E MODULE_FUN_NAME(Graph, EdgeSearch)(G g, int (*cmp)(void *arg, void *priv), void *priv)
{
	E e = NULL;
	int len = 0;

	assert(g && cmp);

	len = MODULE_FUN_NAME(Seq, length)(g->es);

	for (int i = 0; i < len; i++)
	{
		e = (E)MODULE_FUN_NAME(Seq, get)(g->es, i);
		if ((e) && (cmp(e->priv, priv) == 0))
		{
			return e;
		}
	}

	return NULL;
}

E MODULE_FUN_NAME(Graph, EdgeGet)(G g, int i)
{
	assert(g);
	return (E)MODULE_FUN_NAME(Seq, get)(g->es, i);
}

int MODULE_FUN_NAME(Graph, EdgesLength)(G g)
{
	assert(g);
	return g->es_num;
}

/*
 * 将节点@v添加到图@g中，成功返回0，失败-1；
 * g: 图
 * v: 节点
 */
int MODULE_FUN_NAME(Graph, VnodeAdd)(G g, V v)
{
	V node = NULL;
	
	assert(g && v);

	MODULE_FUN_NAME(Seq, addhi)(g->vs, (void *)v);
//	g->vs_num++;

	return 0;
}

/*
 * 将节点@v从图@g中删除，成功返回0，失败-1；
 * g: 图
 * v: 节点
 */
int MODULE_FUN_NAME(Graph, VnodeRemove)(G g, V v)
{
	V node = NULL;
	int len = 0;

	assert(g && v);

	len = MODULE_FUN_NAME(Seq, length)(g->vs);

	for (int i = 0; i < len; i++)
	{
		node = (V)MODULE_FUN_NAME(Seq, get)(g->vs, i);
		if ((node) && (node->iner_key == v->iner_key))
		{
			MODULE_FUN_NAME(Seq, put)(g->vs, i, NULL);
//			g->vs_num--;
			return 0;
		}
	}

	return -1;
}

/*
 * 根据@priv在图@g中查找节点，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
V MODULE_FUN_NAME(Graph, VnodeSearch)(G g, int (*cmp)(void *arg, void *priv), void *priv)
{
	V node = NULL;
	int len = 0;

	assert(g && cmp);

	len = MODULE_FUN_NAME(Seq, length)(g->vs);

	for (int i = 0; i < len; i++)
	{
		node = (V)MODULE_FUN_NAME(Seq, get)(g->vs, i);
		if ((node) && (cmp(node->priv, priv) == 0))
			return node;
	}

	return NULL;
}

V MODULE_FUN_NAME(Graph, VnodeGet)(G g, int i)
{
	assert(g);
	return (E)MODULE_FUN_NAME(Seq, get)(g->vs, i);
}

int MODULE_FUN_NAME(Graph, VnodesLength)(G g)
{
	assert(g);
	return g->vs_num;
}

/*
 * 创建一个最大包含@size个节点的图,成功返回指针，失败NULL
 * @size: 图中最大节点数目
 */
G MODULE_FUN_NAME(Graph, GCreate)(int node_size, int edge_size, void *priv)
{
	G g = NULL;

	assert(node_size >= 0);
	assert(edge_size >= 0);

	g = (G)calloc(1, sizeof(*g)); 
	if (g)
	{
		g->vs = (Seq_T)MODULE_FUN_NAME(Seq, new)(node_size);
		g->es = (Seq_T)MODULE_FUN_NAME(Seq, new)(edge_size);
		if (g->vs == NULL || g->es == NULL)
		{
			if (g->vs) MODULE_FUN_NAME(Seq, free)(&(g->vs));
			if (g->es) MODULE_FUN_NAME(Seq, free)(&(g->es));
			free(g);
		}
		else
		{
			g->priv = priv;
			g->vs_num = node_size;
			g->es_num = edge_size;
		}
	}

	return g;
}

/*
 * 释放图@gp的内容
 * gp: 图指针
 */
void MODULE_FUN_NAME(Graph, GFree)(G *gp)
{
	assert(gp && *gp);

	MODULE_FUN_NAME(Array, free)(&((*gp)->vs));
	MODULE_FUN_NAME(Array, free)(&((*gp)->es));
	free(*gp);
	*gp = NULL;
}



/***********************************util****************************/
/*
 * 打印节点信息
 */
void MODULE_FUN_NAME(Graph, VnodePrint)(V v, int (*print)(V v, void *priv), void *priv)
{
}

/*
 * 打印边信息
 */
void MODULE_FUN_NAME(Graph, EdgePrint)(E e, int (*print)(E e, void *priv), void *priv)
{
}

/*
 * 打印图信息
 */
void MODULE_FUN_NAME(Graph, GPrint)(G g, int (*print)(G g, void *priv), void *priv)
{
}

/*
 * test code

int main(int argc, char *argv[])
{
	struct G *g = NULL;

	g = G_create();
	G_print(g);

	return 0;
}

*/



#if 0
/*
 * 返回图g的第一个节点;
 */
V MODULE_FUN_NAME(Graph, FirstVnode)(G g)
{
	assert(g);

	for (int i = 0; i < g->vs_size; i++)
	{
		if (g->vs[i] != NULL)
		{
			return g->vs[i];
		}
	}

	return NULL;
}

/*
 * 返回图g中v后面的节点,若v后面没有节点，返回NULL
 */
V MODULE_FUN_NAME(Graph, NextVnode)(G g, V v)
{
	int i = 0;
	assert(g && v);

	for (i = 0; i < g->vs_size; i++)
	{
		if ((g->vs[i] != NULL) && (g->vs[i]->iner_key == v->iner_key))
		{
			break;
		}
	}

	for (i = i + 1; i < g->vs_size; i++)
	{
		if (g->vs[i])
		{
			return g->vs[i];
		}
	}

	return NULL;
}

/*
 * 判断v是否时g中最后的一个节点，是返回1，不是返回0
 */
int MODULE_FUN_NAME(Graph, IsLastVnode)(G g, V v)
{
	int i = 0;
	assert(g && v);

	for (i = 0; i < g->vs_size; i++)
	{
		if ((g->vs[i] != NULL) && (g->vs[i]->iner_key == v->iner_key))
		{
			break;
		}
	}

	if (i >= g->vs_size)
	{
		return 1;
	}

	for (i = i + 1; i < g->vs_size; i++)
	{
		if (g->vs[i])
		{
			return 0;
		}
	}

	return 1;

}

/*
 * 返回图g的第一条边;
 */
E MODULE_FUN_NAME(Graph, FirstEdge)(G g)
{
	assert(g);

	for (int i = 0; i < g->es_size; i++)
	{
		if (g->es[i] != NULL)
		{
			return g->es[i];
		}
	}

	return NULL;
}

/*
 * 返回图g中e后面的边,若e后面没有边，返回NULL
 */
E MODULE_FUN_NAME(Graph, NextEdge)(G g, E e)
{
	int i = 0;
	assert(g && e);

	for (i = 0; i < g->es_size; i++)
	{
		if ((g->es[i]== e))
		{
			break;
		}
	}

	for (i = i + 1; i < g->es_size; i++)
	{
		if (g->es[i])
		{
			return g->es[i];
		}
	}

	return NULL;
}

/*
 * 判断e是否时g中最后的一条边，是返回1，不是返回0
 */
int MODULE_FUN_NAME(Graph, IsLastEdge)(G g, E e)
{
	int i = 0;
	assert(g && e);

	for (i = 0; i < g->es_size; i++)
	{
		if ((g->es[i] == e))
		{
			break;
		}
	}

	if (i >= g->es_size)
	{
		return 1;
	}

	for (i = i + 1; i < g->es_size; i++)
	{
		if (g->es[i])
		{
			return 0;
		}
	}

	return 1;

}

/*
 * 后面扩展
 */
void **MODULE_FUN_NAME(Graph, EdgeToArray)(G g, void *priv)
{
}

void **MODULE_FUN_NAME(Graph, VnodeToArray)(G g, void *priv)
{
}

#endif



