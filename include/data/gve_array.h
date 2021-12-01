
/*
 * 实现图基本数据结构
 */
#ifndef CI_GVE_ARRAY_HEADER__
#define CI_GVE_ARRAY_HEADER__

#include "module.h"

#define E EdgeA_T
#define G GraphA_T

typedef struct E *E;
typedef struct G *G;


/**********************************EDGE************************************/
/*
 * 获取边的priv，
 * @e: 边
 * @priv: 用户数据
 */
void *MODULE_FUN_NAME(GraphA, EdgeGetPriv)(E e);

/*
 * 设置边的两个节点，成功返回0，失败-1，
 * @e: 边
 * @v: 边e的开始节点
 * @u: 边e的终止节点
 * @ovp: 保存边e旧的开始节点
 * @oup: 保存边e旧的终止节点
 */
int MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(E e, void *v, void *u, void **ovp, void **oup);

/*
 * 获取边的两个节点并保存在@vp，@up，成功返回0，失败-1，
 * @e: 边
 * @vp: 保存边e的开始节点
 * @up: 保存边e的终止节点
 */
int MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(E e, void **vp, void **up);




/********************************GRAPH******************************/

/*
 * 根据@priv在图@g中查找边，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
E MODULE_FUN_NAME(GraphA, EdgeSearch)(G g, int (*cmp)(void *, void *), void *priv);

/*
 * 根据i获取edge，返回其指针
 * g:	图
 * i:   下标
 */
E MODULE_FUN_NAME(GraphA, EdgeGet)(G g, int i);

/*
 * 根据i设置edge，返回旧指针
 * g:	图
 * i:   下标
 */
E MODULE_FUN_NAME(GraphA, EdgePut)(G g, int i, E e);

/*
 * 获取图g edges 数组的长度
 * g:	图
 */
int MODULE_FUN_NAME(GraphA, EdgesLength)(G g);

/*
 * 根据@priv在图@g中查找节点，找到返回指针，失败返回NULL
 * g:		图
 * cmp: 	比较函数，比较成功返回0，不成功-1；
 * priv: 	用户数据，会在cmp中回传
 */
void *MODULE_FUN_NAME(GraphA, VnodeSearch)(G g, int (*cmp)(void *, void *), void *priv);

/*
 * 根据i获取vnode，返回其指针
 * g:	图
 * i:   下标
 */
void *MODULE_FUN_NAME(GraphA, VnodeGet)(G g, int i);

/*
 * 根据i设置vnode，返回旧指针
 * g:	图
 * i:   下标
 */
void *MODULE_FUN_NAME(GraphA, VnodePut)(G g, int i, void *v);

/*
 * 获取图g vnodes 数组的长度
 * g:	图
 */
int MODULE_FUN_NAME(GraphA, VnodesLength)(G g);

/*
 * 创建一个最大包含@size个节点的图,成功返回指针，失败NULL
 * @node_size: 节点占用的内存
 * @node_len: 节点数目
 * @edge_ext_size: 边的扩展数据大小,
 * @edge_len: 边数目
 * @priv: 用户数据
 */
G MODULE_FUN_NAME(GraphA, create)(int node_size, int node_len, int edge_ext_size, int edge_len, void *priv);

/*
 * 释放图@gp的内容
 * gp: 图指针
 */
void MODULE_FUN_NAME(GraphA, free)(G *gp);

/*
 * 复制图g生成一个新的图
 */
G MODULE_FUN_NAME(GraphA, copy)(G g);






#undef E
#undef G

#endif // end of CI_GVE_ARRAY_HEADER__


