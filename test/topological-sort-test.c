
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "dfs.h"
#include "topological-sort.h"


int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;
	SenDlink_T l = NULL;


	l = MODULE_FUN_NAME(SenDlink, create)();
	g = gve_create_graph(argv[1]);

	topological_sort(g, l);

	topological_sort_print(l);

	return ret;
}

