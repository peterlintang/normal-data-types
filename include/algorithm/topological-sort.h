
/*
 * 实现图的拓扑排序
 */
#ifndef TOPOLOGICAL_SORT_H__
#define TOPOLOGICAL_SORT_H__

#include "sentinel-linked-list.h"
#include "gve_array.h"


/*
 * 图的拓扑排序
 * you xiang tu 
 */
int topological_sort(GraphA_T g, SenDlink_T l);

void topological_sort_print(SenDlink_T l);

#endif // end of TOPOLOGICAL_SORT_H__

