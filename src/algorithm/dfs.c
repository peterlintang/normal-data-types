
/*
 * 实现深度优先图遍历
 * 有向图无详图都可以
 * date: 2021-11-07
 * author: lin
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "gve_array.h"
#include "gve-normal.h"
#include "dfs.h"

#define NODE dfs_node_t
#define EDGE_EXT dfs_edge_ext_t

/*
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
*/




static int cmp(void *arg, void *priv)
{
	NODE node = (NODE )arg;
	int *p = (int *)priv;

	if (*p == node->index) return 0;
	else return 1;
}

/*
 * 根据文件内容创建dfs图
 */
static GraphA_T dfs_get_graphA(FILE *fp)
{
	GraphA_T g = NULL;
	int node_item = 0;
	int edge_item = 0;

	fscanf(fp, "%d", &node_item);
	fscanf(fp, "%d", &edge_item);

	g = MODULE_FUN_NAME(GraphA, create)(sizeof(struct NODE), node_item, sizeof(struct EDGE_EXT), edge_item, NULL);

	return g;
}

static int dfs_get_node(FILE *fp, void *g_p, void *n_p)
{
	int v = 0;
	NODE node = (NODE )n_p;
	GraphA_T g = (GraphA_T)g_p;

	fscanf(fp, "%d", &v);

	node->index = v;
	node->l = MODULE_FUN_NAME(SenDlink, create)();

	return 0;
}

static int dfs_get_edge(FILE *fp, void *g_p, void *e_p)
{
	int v = 0;
	int u = 0;
	int value = 0;
	NODE n_v = NULL;
	NODE n_u = NULL;
	EDGE_EXT ext = NULL;
	GraphA_T g = (GraphA_T)g_p;
	EdgeA_T edge = (EdgeA_T)e_p;

	fscanf(fp, "%d,%d,%d", &v, &u, &value);

	n_v = (NODE)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)&v);
	n_u = (NODE)MODULE_FUN_NAME(GraphA, VnodeSearch)(g, cmp, (void *)&u);
	if (n_v == NULL || n_u == NULL)
	{
		fprintf(stdout, "search %d, %d failed %p, %p\n", v, u, n_v, n_u);
		return -1;
	}

	MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)n_v, (void *)n_u, NULL, NULL);

	ext = (EDGE_EXT)MODULE_FUN_NAME(GraphA, EdgeGetPriv)((void *)edge);
	ext->value = value;

	MODULE_FUN_NAME(SenDlink, insert)(n_v->l, (void *)edge);

	return 0;
}

GraphA_T dfs_create_graph(char *filename)
{
	assert(filename);

	GraphA_T g = NULL;

	g = GA_create(filename, dfs_get_graphA, dfs_get_edge, dfs_get_node);

	return g;
}

/********************dfs print************************/
void dfs_V_print(NODE v)
{
	NODE u = NULL;
	int len = 0;
	EdgeA_T edge = NULL;
	NODE unused = NULL;

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

void dfs_G_print(GraphA_T g)
{
	int len = 0;
	NODE node = NULL;

	assert(g);

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		node = (NODE)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		fprintf(stdout, "\n");
		dfs_V_print(node);
		fprintf(stdout, "\n");
	}
}

/*****************dfs**********************/
static int time = 0;

static int dfs_visit(GraphA_T g, NODE u)
{
	int len = 0;
	EdgeA_T edge = NULL;
	NODE v = NULL;
	NODE unused = NULL;


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
	NODE u = NULL;

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	for (int i = 0; i < len; i++)
	{
		u = (NODE )MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		u->color = WHITE;
		u->prev = NULL;
	}

	time = 0;

	for (int i = 0; i < len; i++)
	{
		u = (NODE )MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
		if (u->color == WHITE)
		{
			dfs_visit(g, u);
		}
	}

	return 0;
}

/*
int main(int argc, char *argv[])
{
	int ret = 0;
	GraphA_T g = NULL;

	g = G_create();
	dfs(g);
	G_print(g);

	return ret;
}
*/

