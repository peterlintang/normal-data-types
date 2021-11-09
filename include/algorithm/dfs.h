
/*
 * 实现深度优先图遍历
 * 有向图无详图都可以
 * date: 2021-11-07
 * author: lin
 */

#ifndef DFS_H__
#define DFS_H__

#include "module.h"
#include "sentinel-linked-list.h"
#include "set-list.h"

#define WHITE	0
#define GRAY	1
#define BLACK	2

#define NODE dfs_node_t
#define EDGE_EXT dfs_edge_ext_t

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


/*
 * you xiang & wu xiang tu jun ke
 */
int dfs(GraphA_T g, int (*cb)(void *arg, void *priv), void *arg);

GraphA_T dfs_create_graph(char *filename);

void dfs_V_print(NODE v);

void dfs_G_print(GraphA_T g);

/*
 * 将由深度优先生成的数据结构中，以v为源节点的所有节点加到集合set中，v的prev为空
 * 深度优先树中的元素都加到集合set
 * g: 将过深度优先算法处理的图
 * v: 根
 * set: 以v为最终根的节点的集合
 */
void dfs_V_tree_produce(GraphA_T g, NODE v, SetL_T set);

/*
 * 将由深度优先生成的深度优先树，全部以一个一个集合的方式放到链表l中，其中每一个集合包括一个深度优先树
 * g: 将过深度优先算法处理的图
 * l: 空链表
 */
void dfs_G_trees_produce(GraphA_T g, SenDlink_T l);

#undef NODE
#undef EDGE_EXT

#endif // end of DFS_H__

