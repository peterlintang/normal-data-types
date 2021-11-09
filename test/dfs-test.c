/*
 * test
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "dfs.h"

static int process_node(void *arg, void *priv)
{
	dfs_node_t node = (dfs_node_t)priv;

	fprintf(stdout, "%s: %d\n", __func__, node->index);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = dfs_create_graph(argv[1]);
	dfs(g, process_node, NULL);
	dfs_G_print(g);

	return ret;
}


