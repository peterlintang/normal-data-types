
/*
 * dijkstra 方法计算单源最短路径
 */
#ifndef DIJKSTRA_H__
#define DIJKSTRA_H__

#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"
#include "sentinel-linked-list.h"
#include "set-list.h"

#define NODE gve_node_t

/*
 *
 * 计算有向图g中的节点到s的最短路径,计算得到的路径值存放在node的d域
 * 其中边的权重大于等于0
 * you xiang tu, bian de quan zhong ke yi wei fu zhi 
 *
 * return value: 0: negative loop; 1: ok
 *
 */
int dijkstra(GraphA_T g, NODE s);

#undef NODE

#endif // end of DIJKSTRA_H__


