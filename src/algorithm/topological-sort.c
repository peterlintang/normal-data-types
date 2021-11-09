
/*
 * 实现图的拓扑排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "dfs.h"
#include "topological-sort.h"


static int process_node(void *arg, void *priv)
{
	SenDlink_T l = (SenDlink_T)arg;

	MODULE_FUN_NAME(SenDlink, insert)(l, priv);

	return 0;
}

/*
 * 图的拓扑排序
 * you xiang tu 
 */
int topological_sort(GraphA_T g, SenDlink_T l)
{
	assert(g && l);

	dfs(g, process_node, (void *)l);

	return 0;
}

void topological_sort_print(SenDlink_T l)
{
	int count = 0;
	dfs_node_t v = NULL;

	assert(l);

	count = MODULE_FUN_NAME(SenDlink, count)(l);

	for (int i = 0; i < count; i++)
	{
		v = (dfs_node_t)MODULE_FUN_NAME(SenDlink, get)(l, i);
		fprintf(stdout, "%d ", v->index);
	}
	fprintf(stdout, "\n");
}


