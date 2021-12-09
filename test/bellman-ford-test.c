
/*
 * test
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "bellman-ford.h"


int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;
	gve_node_t v = NULL;

	g = gve_create_graph(argv[1]);
	v = MODULE_FUN_NAME(GraphA, VnodeGet)(g, 0);
	gve_G_print(g);
	fprintf(stdout, "\n\nhello: v: %d\n\n", v->index);
	ret = bellman_ford(g, v);
	fprintf(stdout, "\n\nhello ret: %d\n\n", ret);
	gve_G_print(g);

	return ret;
}


