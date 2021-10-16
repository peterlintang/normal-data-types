
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"

#define WHITE	0
#define GRAY	1
#define BLACK	2

struct node_t {
	int index;
	int color;
	int d;
	int f;
	struct node_t *prev;
	SenDlink_T l;
};

struct edge_ext_t {
	int value;
};

static int time = 0;

static int dfs_visit(GraphA_T g, struct node_t *u)
{
	int len = 0;
	EdgeA_T edge = NULL;
	struct node_t *v = NULL;
	struct node_t *unused = NULL;


	time = time + 1;
	u->d = time;
	u->color = GRAY;

	len = MODULE_FUN_NAME(SenDlink, count)(u->l);
	for (int i = 0; i < len; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(u->l, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&unused, (void **)&v);
		if (v->color == WHITE)
		{
			v->prev = u;
			dfs_visit(g, v);
		}
	}

	u->color = BLACK;
	time = time + 1;
	u->f = time;

	return 0;
}

/*
 * you xiang & wu xiang tu jun ke
 */
int dfs(GraphA_T g)
{
	int len = 0;
	struct node_t *u = NULL;

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		u = (struct node_t *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		u->color = WHITE;
		u->prev = NULL;
	}

	time = 0;

	for (int i = 0; i < len; i++)
	{
		u = (struct node_t *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (u->color == WHITE)
		{
			dfs_visit(g, u);
		}
	}

	return 0;
}


static int cmp(void *arg, void *priv)
{
	struct node_t *node = (struct node_t *)arg;
	int *p = (int *)priv;

	if (*p == node->index) return 0;
	else return 1;
}

GraphA_T G_create(void)
{
	int node_len = 0;
	int edge_len = 0;
	int node_index = 0;
	int edge_v = 0;
	int edge_u = 0;
	int edge_value = 0;
	GraphA_T g = NULL;
	EdgeA_T edge = NULL;
	struct node_t *node = NULL;
	struct node_t *v = NULL;
	struct node_t *u = NULL;
	struct edge_ext_t *edge_ext = NULL;

	scanf("%d", &node_len);
	scanf("%d", &edge_len);

	fprintf(stdout, "nodes: %d, edges: %d\n", node_len, edge_len);

	g = MODULE_FUN_NAME(GraphA, create)(sizeof(struct node_t), node_len, sizeof(struct edge_ext_t), edge_len, NULL);

	for (int i = 0; i < node_len; i++)
	{
		scanf("%d", &node_index);
		node = (struct node_t *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		node->index = node_index;
		node->l = MODULE_FUN_NAME(SenDlink, create)();
	}

	for (int i = 0; i < edge_len; i++)
	{
		scanf("%d,%d", &edge_v, &edge_u);
		v = (struct node_t *)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)&edge_v);
		u = (struct node_t *)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)&edge_u);
		edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)v, (void *)u, NULL, NULL);
		edge_ext = (struct edge_ext_t *)MODULE_FUN_NAME(GraphA, EdgeGetPriv)((void *)edge);
		edge_ext->value = (i + 1);
		MODULE_FUN_NAME(SenDlink, insert)(v->l, (void *)edge);
	}

	return g;
}	


void V_print(struct node_t *v)
{
	struct node_t *u = NULL;
	int len = 0;
	EdgeA_T edge = NULL;
	struct node_t *unused = NULL;

	assert(v);
	fprintf(stdout, "v: %d, color: %d, p: %p, d: %d, f: %d, es: %d\n",
					v->index, v->color, v->prev, v->d, v->f, MODULE_FUN_NAME(SenDlink, count)(v->l));
	len = MODULE_FUN_NAME(SenDlink, count)(v->l);
	for (int i = 0; i < len; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(SenDlink, get)(v->l, i);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&unused, (void **)&u);
		fprintf(stdout, "v: %d, color: %d, p: %p, d: %d, f: %d\n",
					u->index, u->color, u->prev, u->d, u->f);
	}

}

void G_print(struct G *g)
{
	int len = 0;
	struct node_t *node = NULL;

	assert(g);

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		node = (struct node_t *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		fprintf(stdout, "\n");
		V_print(node);
		fprintf(stdout, "\n");
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = G_create();
	dfs(g);
	G_print(g);

	return ret;
}



