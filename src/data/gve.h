
#ifndef GVE_H__
#define GVE_H__

#include "sentinel-linked-list.h"

#define WHITE	0
#define GRAY	1
#define BLACK	2

#define INFINITE	77777777

struct V {
	int v;
	int color;

	int d;
	int f;
	struct V *p;	// qian qu jie dian

	struct Linked_list l;
	int e_num;
};

/*
struct E {
	struct V *u;
	struct V *v;
//	int 
};
*/

struct G {
	struct V **vs;
	int vs_num;
	/*
	struct E *es;
	int es_num;
	*/
};

struct V *V_create(int p);
int E_create(struct V *v, struct V *u);
struct G *G_create(void);
void V_print(struct V *v);
void G_print(struct G *g);

#endif // end of GVE_H__


