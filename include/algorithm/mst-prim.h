
/*
 * 最小生成树
 * 连通无向图
 */

#ifndef MST_PRIM_H__
#define MST_PRIM_H__

#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"
#include "sentinel-linked-list.h"
#include "set-list.h"
#include "quicksort.h"

#define NODE gve_node_t

/*
 * 计算连通图g的最小生成树
 * 最小生成树的边存放在edges中
 *  algorith for charpter 23-2
 * */
int mst_prim(SenDlink_T edges, GraphA_T g, NODE r);

#undef NODE

#endif // end of MST_PRIM_H__



