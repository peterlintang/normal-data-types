
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"
#include "dfs.h"
#include "strongly-connected-components.h"


/*
 * you xiang tu 
 */
int strongly_connected_components(struct G *g)
{
	int ret = 0;

	dfs(g);

	return ret;
}

/*
 * test code
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	struct G *g = NULL;

	g = G_create();

	ret = strongly_connected_components(g);

	return ret;
}



