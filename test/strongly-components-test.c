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
#include "components.h"

static int process_node(void *arg, void *priv)
{
	gve_node_t node = (gve_node_t)priv;

	fprintf(stdout, "%s: %d, %p\n", __func__, node->index, node);
	return 0;
}

static int map(void *priv, void *arg)
{
	gve_node_t node = (gve_node_t)priv;
	fprintf(stdout, "%s: node: %d, d: %d, f: %d\n", 
					__func__, node->index,
					node->d, node->f);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int count = 0;
	GraphA_T g = NULL;
	SenDlink_T l = NULL;
	SetL_T set = NULL;

	l = MODULE_FUN_NAME(SenDlink, create)();
	g = gve_create_graph(argv[1]);
	gve_G_print(g);
	fprintf(stdout, "\n\n");
	strongly_components_connected(l, g);

	count = MODULE_FUN_NAME(SenDlink, count)(l);
	for (int i = 0; i < count; i++)
	{
		set = (SetL_T)MODULE_FUN_NAME(SenDlink, get)(l, i);
		fprintf(stdout, "i: %d\n", i);
		MODULE_FUN_NAME(SetL, map)(set, map, NULL);
	}

	return ret;
}


