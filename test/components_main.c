
#include <stdio.h>
#include <stdlib.h>

#include "components.h"

static void apply(void **priv, void *arg)
{
	int count = 0;
	struct node *p = NULL;
	SetL_T set = (SetL_T)(*priv);

	count = MODULE_FUN_NAME(SetL, count)(set);
	fprintf(stdout, "%s: set: %p, count: %d\n", __func__, set, count);
	for (int i = 0; i< count; i++)
	{
		p = (struct node *)MODULE_FUN_NAME(SetL, get)(set, i);
		fprintf(stdout, "i: %d, v: %p, value: %d\n", i, p, p->v);
	}
}

static int v_print(void *priv, void *arg)
{
	struct node *node = (struct node *)priv;
	fprintf(stdout, "%s: node: %p, v: %d\n", __func__, node, node->v);
	return 0;
}

static int e_print(void *priv, void *arg)
{
	struct node *v = NULL;
	struct node *u = NULL;
	struct edge_ext *ext = NULL;
	EdgeA_T edge = (EdgeA_T)priv;
	MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, &v, &u);
	ext = (struct edge_ext *)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
	fprintf(stdout, "%s: edge: %p, v: %p, u: %p, value: %d\n", 
					__func__,
					edge, 
					v,
					u,
					ext->value);
	return 0;
}

int main(int argc, char *argv[])
{
	GraphA_T g = NULL;
	ListD_T sets = NULL;

	if (argc != 2) return 0;

	g = components_create_graph(argv[1]);
	fprintf(stdout, "%s: g: %p\n", __func__, g);

	GA_print(g, v_print, e_print, NULL, NULL);

	sets = MODULE_FUN_NAME(ListD, new)();
	fprintf(stdout, "%s: %p, %p\n", __func__, sets, g);

	components_connected(sets, g);

	MODULE_FUN_NAME(ListD, map)(sets, apply, NULL);

	return 0;
}
