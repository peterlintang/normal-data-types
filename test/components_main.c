
#include <stdio.h>
#include <stdlib.h>

#include "components.h"

static void apply(void **priv, void *arg)
{
	int count = 0;
	gve_node_t p = NULL;
	SetL_T set = (SetL_T)(*priv);

	count = MODULE_FUN_NAME(SetL, count)(set);
	fprintf(stdout, "%s: set: %p, count: %d\n", __func__, set, count);
	for (int i = 0; i< count; i++)
	{
		p = (gve_node_t )MODULE_FUN_NAME(SetL, get)(set, i);
		fprintf(stdout, "i: %d, v: %p, value: %d\n", i, p, p->index);
	}
}

static int v_print(void *priv, void *arg)
{
	gve_node_t node = (gve_node_t )priv;
	fprintf(stdout, "%s: node: %p, v: %d\n", __func__, node, node->index);
	return 0;
}

static int e_print(void *priv, void *arg)
{
	gve_node_t v = NULL;
	gve_node_t u = NULL;
	gve_edge_ext_t ext = NULL;
	EdgeA_T edge = (EdgeA_T)priv;
	MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, &v, &u);
	ext = (gve_edge_ext_t )MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
	fprintf(stdout, "%s: edge: %p, v: %p, u: %p, value: %d\n", 
					__func__,
					edge, 
					v,
					u,
					ext->value);
	return 0;
}

int main(int argc, char *argv[])
{
	GraphA_T g = NULL;
	GraphA_T convert = NULL;
	ListD_T sets = NULL;

	if (argc != 2) return 0;

	g = gve_create_graph(argv[1]);
	fprintf(stdout, "%s: g: %p\n", __func__, g);

	GA_print(g, v_print, e_print, NULL, NULL);

	convert = convert_graphA_edges(g);
	fprintf(stdout, "%s: g convert\n", __func__);
	GA_print(convert, v_print, e_print, NULL, NULL);

	sets = MODULE_FUN_NAME(ListD, new)();
	fprintf(stdout, "%s: %p, %p\n", __func__, sets, g);

	components_connected(sets, g);

	MODULE_FUN_NAME(ListD, map)(sets, apply, NULL);

	return 0;
}
