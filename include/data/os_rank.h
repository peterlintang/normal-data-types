
/*
 * 顺序统计树
 * 由红黑树扩展而得来的os rank数据结构
 * 主要是多了可以根据数据在树中的排序位置，
 * 快速访问相应元素
 *
 */
/*
 * for charpter 14
 */

#ifndef CI_OS_RANK_HEADER
#define CI_OS_RANK_HEADER

#include "module.h"

#define T OsRank_T
#define NODE OsRank_Node

typedef struct T *T;
typedef struct NODE *NODE;

struct NODE {
	NODE left;
	NODE right;
	NODE parent;
	void *key;
	int size;
#define RED		0x1
#define BLACK	0x2
	int color;
};

struct T {
	NODE root;
	NODE nil;
	int (*cmp)(void *, void *);
};

/*
 * 与红黑树操作相仿
 */
NODE MODULE_FUN_NAME(OsRank, search)(T tree, void *key);

NODE MODULE_FUN_NAME(OsRank, iterative_search)(T tree, void *key);

NODE MODULE_FUN_NAME(OsRank, minimum)(T tree);

NODE MODULE_FUN_NAME(OsRank, maximum)(T tree);

NODE MODULE_FUN_NAME(OsRank, successor)(T tree, NODE x);

NODE MODULE_FUN_NAME(OsRank, predecessor)(T tree, NODE x);

NODE MODULE_FUN_NAME(OsRank, insert)(T tree, NODE node);

NODE MODULE_FUN_NAME(OsRank, delete)(T tree, NODE z);

T MODULE_FUN_NAME(OsRank, new)(int (*cmp)(void *, void *));

void MODULE_FUN_NAME(OsRank, free)(T *treep);

void MODULE_FUN_NAME(OsRank, inorder_walk)(T tree, NODE node, int (*map)(void *, void *), void *arg);

void MODULE_FUN_NAME(OsRank, preorder_walk)(T tree, NODE node, int (*map)(void *, void *), void *arg);

void MODULE_FUN_NAME(OsRank, postorder_walk)(T tree, NODE node, int (*map)(void *, void *), void *arg);

/*
 *  扩展的数据操作
 */
/*
 * 返回节点@x的在tree中元素排序的顺序号，顺序号从1开始，
 * @tree:	顺序树
 * @x:		节点
 */
int MODULE_FUN_NAME(OsRank, rank)(T tree, NODE x);

/*
 * 返回在tree中元素排序的顺序号为@i的节点，顺序号从1开始，
 * 超出范围返回空指针
 * @tree:	顺序树
 * @i:		节点序号
 */
NODE MODULE_FUN_NAME(OsRank, select)(T tree, int i);


#undef T
#undef NODE

#endif // end of CI_OS_RANK_HEADER




