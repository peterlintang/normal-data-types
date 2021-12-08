/*
 * test
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "mst-prim.h"


int main(int argc, char *argv[])
{
	int ret = 0;
	int count = 0;
	GraphA_T g = NULL;
	gve_node_t v = NULL;
	gve_node_t u = NULL;
	EdgeA_T edge = NULL;
	gve_edge_ext_t edge_ext = NULL;
	SenDlink_T l = NULL;

	l = MODULE_FUN_NAME(SenDlink, create)();
	g = gve_create_graph(argv[1]);
	v = MODULE_FUN_NAME(GraphA, VnodeGet)(g, 0);
	gve_G_print(g);
	fprintf(stdout, "\n\nhello: v: %d\n\n", v->index);
	mst_prim(l, g, v);
	fprintf(stdout, "\n\nhello\n\n");

	count = MODULE_FUN_NAME(SenDlink, count)(l);
	for (int i = 0; i < count; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(l, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)(&v), (void **)(&u));
		edge_ext = (gve_edge_ext_t)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
		fprintf(stdout, "i: %d, v: %d, u: %d, edge: %d\n", i, v->index, u->index, edge_ext->value);
	}

	return ret;
}


