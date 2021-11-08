
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


int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = bfs_create_graph(argv[1]);
	bfs(g, 1);
	G_tree_print(g, 3);

	return ret;
}

