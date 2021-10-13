
/*
 * 实现图扩展数据结构
 */
#ifndef CI_GVE_EXT_HEADER__
#define CI_GVE_EXT_HEADER__

#include "module.h"
#include "gve.h"

#define WHITE	0
#define GRAY	1
#define BLACK	2

#define INFINITE	77777777

#define V VNodeExt_T
#define E EdgeExt_T

typedef struct V *V;
typedef struct E *E;

struct V {
	int color;		// 节点颜色， 黑白灰
	int d;			// 忘记了是表示什么
	int f;			// 忘记了是表示什么
	VNode_T p;		// 前驱节点
	SenDlink_T l;	// 以v为开始节点的边的终止节点链表
	int e_num;		// l中的节点数目
};

struct E {
	int value;
};


#undef V
#undef E

#endif // end of CI_GVE_EXT_HEADER__


