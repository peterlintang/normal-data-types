
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"



struct V *V_create(int p)
{
	struct V *v = NULL;

	v = (struct V *)calloc(1, sizeof(*v));
	if (NULL == v)
	{
		return NULL;
	}

	v->v = p;
	v->color = WHITE;
	v->d = INFINITE;
	v->p = NULL;
	v->e_num = 0;
	v->l.nil.next = v->l.nil.prev = &(v->l.nil);

	return v;
}

int E_create(struct V *v, struct V *u)
{
	assert(v);
	assert(u);

	list_insert(&(v->l), u);
	v->e_num += 1;
}

struct G *G_create(void)
{
	int key= 0;
	int e_num = 0;
	int p1 = 0;
	int p2 = 0;
	struct V *v = NULL;
	struct V *u = NULL;
	struct G *g = NULL;

	g = (struct G *)calloc(1, sizeof(*g));
	if (g == NULL)
	{
		return NULL;
	}

	scanf("%d", &(g->vs_num));

	g->vs = (struct V **)calloc(g->vs_num, sizeof(struct V*));

//	fprintf(stdout, "total vs: %d\n", g->vs_num);
	for (int i = 0; i < g->vs_num; i++)
	{
		scanf("%d", &key);
		g->vs[i] = V_create(key);
	}

	scanf("%d", &e_num);
//	fprintf(stdout, "total es: %d\n", e_num);
	for (int i = 0; i < e_num; i++)
	{
		scanf("%d,%d", &p1, &p2);
		v = g->vs[p1 - 1];
		u = g->vs[p2 - 1];
		E_create(v, u);
//		fprintf(stdout, "es: %d, %d\n", p1, p2);
	}

//	fprintf(stdout, "done\n");

	return g;
}

void V_print(struct V *v)
{
	struct item *cur = NULL;
	struct V *u = NULL;

	assert(v);
	fprintf(stdout, "v: %d, color: %d, p: %p, d: %d, f: %d, es: %d\n",
					v->v, v->color, v->p, v->d, v->f, v->e_num);
	for (cur = v->l.nil.next; cur != &(v->l.nil); cur = cur->next)
	{
		u = (struct V *)cur->key;
		fprintf(stdout, "v: %d, color: %d, p: %p, d: %d, f: %d\n",
					u->v, u->color, u->p, u->d, u->f);
	}
}

void G_print(struct G *g)
{
	assert(g);

	for (int i = 0; i < g->vs_num; i++)
	{
		fprintf(stdout, "\n");
		V_print(g->vs[i]);
		fprintf(stdout, "\n");
	}
}

/*
 * test code

int main(int argc, char *argv[])
{
	struct G *g = NULL;

	g = G_create();
	G_print(g);

	return 0;
}

*/



