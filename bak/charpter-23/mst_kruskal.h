
#ifndef MST_KRUSKAL_H__
#define MST_KRUSKAL_H__

#include "../charpter-22/gve.h"
#include "../charpter-21/set-list.h"

struct weight_e {
	int u;
	int v;
	int w;
};



/*
 *
 *  algorith for charpter 23-2
 * */
int mst_kruskal(struct sets *sets, 
				struct G *g, 
				struct weight_e *wes, int wes_num,
				struct G *mst_g);





#endif // end of MST_KRUSKAL_H__



