
/*
 * 最小生成树
 * 连通无向图
 */

#ifndef GENERIC_MST_H__
#define GENERIC_MST_H__

#include "gve_array.h"
#include "gve-normal.h"
#include "ve.h"
#include "sentinel-linked-list.h"
#include "set-list.h"
#include "quicksort.h"


/*
 * 计算连通图g的最小生成树
 * 最小生成树的边存放在edges中
 *  algorith for charpter 23-2
 * */
int mst_kruskal(SenDlink_T edges, GraphA_T g);

#endif // end of GENERIC_MST_H__



