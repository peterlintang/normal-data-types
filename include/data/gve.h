
/*
 * 实现图基本数据结构
 */
#ifndef CI_GVE_HEADER__
#define CI_GVE_HEADER__

#include "module.h"
//#include "sentinel-linked-list.h"

//#define WHITE	0
//#define GRAY	1
//#define BLACK	2

//#define INFINITE	77777777

#define V VNode_T
#define E Edge_T
#define G Graph_T

typedef struct V *V;
typedef struct E *E;
typedef struct G *G;


/**********************************VNODE************************************/
/*
 * 创建一个图顶点，成功返回指针，失败返回NULL
 * @v: 用户数据
 */
V MODULE_FUN_NAME(Graph, VnodeCreate)(void *priv);

/*
 * 释放一个图顶点，
 * @vp: 节点数据的指针
 */
void MODULE_FUN_NAME(Graph, VnodeFree)(V *vp);

/*
 * 设置顶点的priv，返回原来的priv
 * @v: 节点数据的指针
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, VnodeSetPriv)(V v, void *priv);

/*
 * 获取顶点的priv，返回原来的priv
 * @v: 节点数据的指针
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, VnodeGetPriv)(V v);


/**********************************EDGE************************************/

/*
 * 创建一条以@v为开始节点，@u为终止节点的边，成功返回指针，失败返回NULL
 * @v:   边节点
 * @u:   边节点
 * @priv: 用户数据, 可以存放与边有关的信息
 */
E MODULE_FUN_NAME(Graph, EdgeCreate)(V v, V u, void *priv);

/*
 * 释放边@ep的内容
 * @ep:   边指针
 */
void MODULE_FUN_NAME(Graph, EdgeFree)(E *ep);

/*
 * 设置边的priv，返回原来的priv
 * @e: 边
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, EdgeSetPriv)(E e, void *priv);

/*
 * 获取边的priv，
 * @e: 边
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(Graph, EdgeGetPriv)(E e);

/*
 * 设置边的两个节点，成功返回0，失败-1，
 * @e: 边
 * @v: 边e的开始节点
 * @u: 边e的终止节点
 * @ovp: 保存边e旧的开始节点
 * @oup: 保存边e旧的终止节点
 */
int MODULE_FUN_NAME(Graph, EdgeSetVnodes)(E e, V v, V u, V *ovp, V *oup);

/*
 * 获取边的两个节点并保存在@vp，@up，成功返回0，失败-1，
 * @e: 边
 * @vp: 保存边e的开始节点
 * @up: 保存边e的终止节点
 */
int MODULE_FUN_NAME(Graph, EdgeGetVnodes)(E e, V *vp, V *up);

/********************************GRAPH******************************/

/*
 * 添加边@e到图@g中,并更新相关节点信息
 * 成功返回0，失败-1
 * 将以@e->v为开始节点，@e->u为终止节点的边@e->v添加到v的链表上
 * 成功返回0，失败返回-1
 * @v:	节点
 * @e:	以@v为开始节点的边 (v->u, 对于有向)
 */
int MODULE_FUN_NAME(Graph, EdgeAdd)(G g, E e);

/*
 * 将边@e从图@g中删除,并更新相关节点的信息
 * 成功返回0，失败-1
 * 将以@e->v为开始节点，@e->u为终止节点的边@e->v的链表上删除
 * 成功返回0，失败返回-1
 * @v:	节点
 * @e:	以@v为开始节点的边 (v->u, 对于有向)
 */
int MODULE_FUN_NAME(Graph, EdgeRemove)(G g, E e);


/*
 * 根据@priv在图@g中查找边，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
E MODULE_FUN_NAME(Graph, EdgeSearch)(G g, int (*cmp)(void *, void *), void *priv);

/*
 * 根据i获取edge，返回其指针
 * g:	图
 * i:   下标
 */
E MODULE_FUN_NAME(Graph, EdgeGet)(G g, int i);

/*
 * 获取图g edges 数组的长度
 * g:	图
 */
int MODULE_FUN_NAME(Graph, EdgesLength)(G g);

/*
 * 将节点@v添加到图@g中，成功返回0，失败-1；
 * g: 图
 * v: 节点
 */
int MODULE_FUN_NAME(Graph, VnodeAdd)(G g, V v);

/*
 * 将节点@v从图@g中删除，成功返回0，失败-1；
 * g: 图
 * v: 节点
 */
int MODULE_FUN_NAME(Graph, VnodeRemove)(G g, V v);

/*
 * 根据@priv在图@g中查找节点，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
V MODULE_FUN_NAME(Graph, VnodeSearch)(G g, int (*cmp)(void *, void *), void *priv);

/*
 * 根据i获取vnode，返回其指针
 * g:	图
 * i:   下标
 */
V MODULE_FUN_NAME(Graph, VnodeGet)(G g, int i);

/*
 * 获取图g vnodes 数组的长度
 * g:	图
 */
int MODULE_FUN_NAME(Graph, VnodesLength)(G g);

/*
 * 创建一个最大包含@size个节点的图,成功返回指针，失败NULL
 * @node_size: 图中最大节点数目
 * @edge_size: 图中最大边数目
 * @priv: 用户数据
 */
G MODULE_FUN_NAME(Graph, GCreate)(int node_size, int edge_size, void *priv);

/*
 * 释放图@gp的内容
 * gp: 图指针
 */
void MODULE_FUN_NAME(Graph, GFree)(G *gp);



void **MODULE_FUN_NAME(Graph, EdgeToArray)(G g, void *priv);

void **MODULE_FUN_NAME(Graph, VnodeToArray)(G g, void *priv);


/***********************************util****************************/
/*
 * 打印节点信息
 */
void MODULE_FUN_NAME(Graph, VnodePrint)(V v, int (*print)(V v, void *priv), void *priv);

/*
 * 打印边信息
 */
void MODULE_FUN_NAME(Graph, EdgePrint)(E e, int (*print)(E e, void *priv), void *priv);

/*
 * 打印图信息
 */
void MODULE_FUN_NAME(Graph, GPrint)(G g, int (*print)(G g, void *priv), void *priv);

#if 0

/*
 * 返回图g的第一个节点
 */
V MODULE_FUN_NAME(Graph, FirstVnode)(G g);

/*
 * 返回图g中v后面的节点，若没有则返回NULL
 */
V MODULE_FUN_NAME(Graph, NextVnode)(G g, V v);

/*
 * 判断v是否为g中最后一个节点，是返回1， 不是返回0
 */
int MODULE_FUN_NAME(Graph, IsLastVnode)(G g, V v);

/*
 * 返回图g的第一条边
 */
E MODULE_FUN_NAME(Graph, FirstEdge)(G g);

/*
 * 返回图g中e后面的边，若没有则返回NULL
 */
E MODULE_FUN_NAME(Graph, NextEdge)(G g, E e);

/*
 * 判断e是否为g中最后一条边，是返回1， 不是返回0
 */
int MODULE_FUN_NAME(Graph, IsLastEdge)(G g, E e);

#endif 


#undef V
#undef E
#undef G

#endif // end of CI_GVE_HEADER__


