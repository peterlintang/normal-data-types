
#ifndef DAG_SHORTEST_PATHS_H
#define DAG_SHORTEST_PATHS_H

#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"
#include "sentinel-linked-list.h"
#include "set-list.h"

#define NODE gve_node_t


/*
 *
 * you xiang tu, bian de quan zhong ke yi wei fu zhi 
 * 也可以用来计算pert图，分析关键路径，具体间算法导论383页
 *
 * return value: 0: negative loop; 1: ok
 *
 */
int dag_shortest_paths(GraphA_T g, NODE s);

#undef NODE

#endif // end of DAG_SHORTEST_PATHS_H


