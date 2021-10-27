
/*
 * 根据数据文件生成图的数据结构
 * 文件格式如下
 * 节点数目
 * 边数目
 * 点1信息
 * 点2信息
 * 。。。
 * 边1信息
 * 边2信息
 * 。。。
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "gve-normal.h"


GraphA_T GA_create(
			char *filename,
			void *(*get_graph)(FILE *fp),
			int (*get_edge)(FILE *fp, void *g, void *edge), 
			int (*get_node)(FILE *fp, void *g, void *node)
			)
{
	FILE *fp = NULL;
	GraphA_T g = NULL;
	EdgeA_T edge = NULL;
	int node_len = 0;
	int edge_len = 0;
	int ret = 0;
	void *node = NULL;

	assert(filename);
	assert(get_graph);
	assert(get_edge);
	assert(get_node);

	fp = fopen(filename, "r");

	g = get_graph(fp);
	if (g == NULL)
	{
		fclose(fp);
		return NULL;
	}


	node_len = MODULE_FUN_NAME(GraphA, VnodesLength)(g); 
	edge_len = MODULE_FUN_NAME(GraphA, EdgesLength)(g);

	for (int i = 0; i < node_len; i++)
	{
		node = MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		ret = get_node(fp, g, node);
		if (ret != 0)
		{
			fprintf(stderr, "get: %i node failed: %d\n", i, ret);
		}
	}

	for (int i = 0; i < edge_len; i++)
	{
		edge = MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		ret = get_edge(fp, g, edge);
		if (ret != 0)
		{
			fprintf(stderr, "get: %i node failed: %d\n", i, ret);
		}
	}

	fclose(fp);
	fprintf(stdout, "%s: nodes: %d, edges: %d, g: %p\n", 
				__func__,
				node_len = MODULE_FUN_NAME(GraphA, VnodesLength)(g), 
				edge_len = MODULE_FUN_NAME(GraphA, EdgesLength)(g),
				g);

	return g;
}	


void GA_V_print(int (*print)(void *v, void *arg), void *v, void *arg)
{
	assert(print);
	assert(v);

	print(v, arg);
}

void GA_E_print(int (*print)(void *edge, void *arg), void *edge, void *arg)
{
	assert(print);
	assert(edge);

	print(edge, arg);
}

void GA_print(GraphA_T g, int (*v_print)(void *, void *), int (*e_print)(void *, void *arg), void *v_arg, void *e_arg)
{
	int len = 0;
	void *priv = NULL;

	assert(g);
	assert(v_print);
	assert(e_print);

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		priv = MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		GA_V_print(v_print, priv, v_arg);
	}

	len = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	for (int i = 0; i < len; i++)
	{
		priv = MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		GA_E_print(e_print, priv, e_arg);
	}
}


