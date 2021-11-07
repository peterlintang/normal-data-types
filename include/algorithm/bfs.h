
/*
 * 实现图的广度优先遍历
 * 有向图无详图均可
 */
#ifndef BFS_H__
#define BFS_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"
#include "queue.h"
#include "gve.h"
#include "bfs.h"


/*
 * you xiang & wu xiang tu jun ke
 */
int bfs(struct G *g, int k);

void G_tree_print(struct G *g, int k);

static void _G_tree_print_path(struct G *g, 
				struct V *s, struct V *v);

void G_tree_print_path(struct G *g, int s, int v);

#endif // end of BFS_H__



