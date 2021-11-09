
/*
 * 计算图的连通分量
 * 用set-list实现
 */

#include "dlist.h"
#include "gve_array.h"
#include "set-list.h"

struct node {
	int v;
};

struct edge_ext {
	int value;
};

GraphA_T components_create_graph(char *filename);

/*
 * 计算图g的连通分量,图最后的连通分量保存在链表sets中，
 * 链表中每一个元素代表一个连通分量
 * g: 有节点信息，边信息的图(无向图)
 * sets: 空链表
 */
void components_connected(ListD_T sets, GraphA_T g);

/*
 * 判断x与y是否在同一个连通分量中，即判断x与y是否连通
 * sets: 连通分量链表
 * x:
 * y:
 */
int components_same(ListD_T sets, void *x, void *y);

