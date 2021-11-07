
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
int dfs(GraphA_T g);

GraphA_T dfs_create_graph(char *filename);

void dfs_V_print(NODE v);

void dfs_G_print(GraphA_T g);

#undef NODE
#undef EDGE_EXT

#endif // end of DFS_H__

