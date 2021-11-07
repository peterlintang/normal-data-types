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


int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = dfs_create_graph(argv[1]);
	dfs(g);
	dfs_G_print(g);

	return ret;
}


