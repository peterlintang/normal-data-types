
/*
 * 实现图的广度优先遍历
 * 有向图无详图均可
 * date: 2021-11-07
 * author: lin
 */
#ifndef BFS_H__
#define BFS_H__


#include "module.h"
#include "sentinel-linked-list.h"

#define WHITE	0
#define GRAY	1
#define BLACK	2

#define INFINITE (-7777777)

#define NODE bfs_node_t
#define EDGE_EXT bfs_edge_ext_t

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
 * g: 图数据结构
 * k: index为k的节点作为开始节点
 */
int bfs(GraphA_T g, int k, int (*cb)(void *arg, void *priv), void *arg);

/*
 * 打印终点为k的节点的前缀路径
 * g: 图数据结构
 * k: index为k的节点
 */
void G_tree_print(GraphA_T g, int k);

/*
 * 打印s到v的路径，存在的话
 */
void G_tree_print_path(GraphA_T g, int s, int v);

/*
 * 根据文件创建bfs图数据结构
 */
GraphA_T bfs_create_graph(char *filename);

#undef NODE
#undef EDGE_EXT

#endif // end of BFS_H__


