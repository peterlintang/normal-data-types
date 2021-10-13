
/*
 * 实现图基本数据结构
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "queue.h"
#include "gve.h"

#define V VNode_T
#define E Edge_T
#define G Graph_T

struct V {
	void *priv;				// 用户的数据 (信息)
	int color;				// 颜色 白 灰 黑三者之一

	int d;					// 不太记得用途了，后面看能否补充注释
	int f;					// 不太记得用途了，后面看能否补充
	V p;					// 前驱节点,有的话

	SenDlink_T l;			// 以此节点为边的一个节点的边,对于有向节点，以@v为开始节点，以边中的另一节点为终止节点
	int e_num;				// 以此节点为边的一个节点的边的数量,
};


struct E {
	V v;		// 开始节点
	V u;		// 终止节点
	void *priv;	//用户数据
};

struct G {
	V *vs;					// 图中顶点
	int vs_num;				// 图中顶点数目
	E *es;					// 图中边信息
	int es_num;				// 图中边数目
};

/*
 * 创建一个图顶点，成功返回指针，失败返回NULL
 * @v: 用户数据
 */
V MODULE_FUN_NAME(Graph, VNodeCreate)(void *priv)
{
}

/*
 * 释放一个图顶点，
 * @vp: 节点数据的指针
 */
void MODULE_FUN_NAME(Graph, VnodeFree)(V *vp)
{
}

/*
 * 创建一条以@v为开始节点，@u为终止节点的边，成功返回指针，失败返回NULL
 * @v:   边节点
 * @u:   边节点
 * @priv: 用户数据, 可以存放与边有关的信息
 */
E MODULE_FUN_NAME(Graph, EdgeCreate)(V v, V u, void *priv)
{
}

/*
 * 释放边@ep的内容
 * @ep:   边指针
 */
void MODULE_FUN_NAME(Graph, EdgeFree)(E *ep)
{
}

/*
 * 创建一个最大包含@size个节点的图,成功返回指针，失败NULL
 * @size: 图中最大节点数目
 */
G MODULE_FUN_NAME(Graph, GCreate)(int size)
{
}

/*
 * 释放图@gp的内容
 * gp: 图指针
 */
void MODULE_FUN_NAME(Graph, GFree)(G *gp)
{
}

/*
 * 打印节点信息
 */
void MODULE_FUN_NAME(Graph, VNodePrint)(V v, int (*print)(V e, void *priv), void *priv)
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
 * 根据@priv在图@g中查找边，找到返回指针，失败返回NULL
 * g:           图
 * cmp:         比较函数，比较成功返回0，不成功-1；
 * priv:        用户数据，会在cmp中回传
 */
E MODULE_FUN_NAME(Graph, EdgeSearch)(G g, int (*cmp)(E e, void *priv), void *priv)
{
}

/*
 * 添加边@e到图@g中,并更新相关节点信息
 * 成功返回0，失败-1
 * 将以@e->v为开始节点，@e->u为终止节点的边@e->v添加到v的链表上
 * 成功返回0，失败返回-1
 * @v:	节点
 * @e:	以@v为开始节点的边 (v->u, 对于有向)
 */
int MODULE_FUN_NAME(Graph, EdgeAdd)(G g, E e)
{
}

/*
 * 将边@e从图@g中删除,并更新相关节点的信息
 * 成功返回0，失败-1
 * 将以@e->v为开始节点，@e->u为终止节点的边@e->v的链表上删除
 * 成功返回0，失败返回-1
 * @v:	节点
 * @e:	以@v为开始节点的边 (v->u, 对于有向)
 */
int MODULE_FUN_NAME(Graph, EdgeRemove)(G g, E e)
{
}

/*
 * 根据@priv在图@g中查找节点，找到返回指针，失败返回NULL
 * g:           图
 * cmp:         比较函数，比较成功返回0，不成功-1；
 * priv:        用户数据，会在cmp中回传
 */
V MODULE_FUN_NAME(Graph, VnodeSearch)(G g, int (*cmp)(V v, void *priv), void *priv)
{
}

/*
 * 将节点@v添加到图@g中，成功返回0，失败-1；
 * g: 图
 * v: 节点
 */
int MODULE_FUN_NAME(Graph, VnodeAdd)(G g, V v)
{
}

/*
 * 将节点@v从图@g中删除，成功返回0，失败-1；
 * g: 图
 * v: 节点
 */
int MODULE_FUN_NAME(Graph, VnodeRemove)(G g, V v)
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




