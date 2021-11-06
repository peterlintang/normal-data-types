
/*
 * 根据图数据文件生成图数据结构
 * 文件格式
 * 节点数目
 * 边数目
 * 点1信息
 * 点2信息
 * 。。。
 * 边1信息
 * 边2信息
 * 。。。
 */

#ifndef CI_GVE_NORMAL_H
#define CI_GVE_NORMAL_H

#include "gve_array.h"


GraphA_T GA_create(
			char *filename,
			GraphA_T (*get_graph)(FILE *fp),
			int (*get_edge)(FILE *fp, void *g, void *edge), 
			int (*get_node)(FILE *fp, void *g, void *node)
			);

void GA_V_print(int (*print)(void *v, void *arg), void *v, void *arg);

void GA_E_print(int (*print)(void *edge, void *arg), void *edge, void *arg);

void GA_print(GraphA_T g, int (*v_print)(void *, void *), int (*e_print)(void *, void *arg), void *v_arg, void *e_arg);


#endif // end of CI_GVE_NORMAL_H

