
/*
 * 实现图的广度优先遍历
 * 有向图无详图均可
 * date: 2021-11-07
 * author: lin
 * 测试用例
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "bfs.h"

static int process_node(void *arg, void *priv)
{
	gve_node_t node = (gve_node_t)priv;

	fprintf(stdout, "%s: %d\n", __func__, node->index);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = gve_create_graph(argv[1]);
	bfs(g, 1, process_node, NULL);
	G_tree_print(g, 3);

	return ret;
}

