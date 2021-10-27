
#include <stdio.h>
#include <stdlib.h>

#include "components.h"

static void apply(void **priv, void *arg)
{
	int count = 0;
	struct node *p = NULL;
	SetL_T set = (SetL_T)(*priv);

	count = MODULE_FUN_NAME(SetL, count)(set);
	for (int i = 0; i< count; i++)
	{
		p = (struct node *)MODULE_FUN_NAME(SetL, get)(set, i);
		fprintf(stdout, "i: %d, value: %d\n", i, p->v);
	}
}

static int v_print(void *priv, void *arg)
{
	struct node *node = (struct node *)priv;
	fprintf(stdout, "%s: v: %d\n", __func__, node->v);
	return 0;
}

static int e_print(void *priv, void *arg)
{
	return 0;
}

int main(int argc, char *argv[])
{
	GraphA_T g = NULL;
	ListD_T sets = NULL;

	if (argc != 2) return 0;

	g = components_create_graph(argv[1]);
	sets = MODULE_FUN_NAME(ListD, new)();

	fprintf(stdout, "%s: %p, %p\n", __func__, sets, g);
	GA_print(g, v_print, e_print, NULL, NULL);

	components_connected(sets, g);

	MODULE_FUN_NAME(ListD, map)(sets, apply, NULL);

	return 0;
}
