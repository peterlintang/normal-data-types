
/*
 * filename: 		interval_tree.h
 * description:		利用红黑树实现区间树数据结构
 * date:			2022-10-08
 * author:			lxj
 */

/*
 * for charpter 13
 */


#ifndef CI_INTERVAL_TREE_HEADER
#define CI_INTERVAL_TREE_HEADER


#include "module.h"

#define T In_Tree
#define NODE In_Node

typedef struct T *T;
typedef struct NODE *NODE;

struct interval {
	int low;
	int high;
};

struct NODE {
	NODE left;
	NODE right;
	NODE parent;
	struct interval in;
	int max;
//	void *priv;
#define RED             0x1
#define BLACK   		0x2
	int color;
};

struct T {
	NODE root;
	NODE nil;
	int (*cmp)(NODE , struct interval *);
};


/*
 * 根据关键字key查找元素，找到返回元素指针，找不到返回空(递归查找)
 * @tree:	红黑树
 * @key:	关键字
 */
NODE MODULE_FUN_NAME(InTree, search)(T tree, struct interval *in);

/*
 * 根据关键字key查找元素，找到返回元素指针，找不到返回空 (非递归)
 * @tree:	红黑树
 * @key:	关键字
 */
NODE MODULE_FUN_NAME(InTree, iterative_search)(T tree, struct interval *in);

/*
 * 返回指向树的最小元素,若树空，返回空
 * @tree:	红黑树
 */
NODE MODULE_FUN_NAME(InTree, minimum)(T tree);

/*
 * 返回指向树的最大元素,若树空，返回空
 * @tree:	红黑树
 */
NODE MODULE_FUN_NAME(InTree, maximum)(T tree);

/*
 * 返回指向树中元素@x的后缀元素,若没有返回空
 * @tree:	红黑树
 * @x:		树中元素
 */
NODE MODULE_FUN_NAME(InTree, successor)(T tree, NODE x);

/*
 * 返回指向树中元素@x的前缀元素,若没有返回空
 * @tree:	红黑树
 * @x:		树中元素
 */
NODE MODULE_FUN_NAME(InTree, predecessor)(T tree, NODE x);

/*
 * 将@z插入树@tree中,返回z的指针
 * @tree:	红黑树
 * @z:		新元素
 */
NODE MODULE_FUN_NAME(InTree, insert)(T tree, NODE z);

/*
 * 将@z插入树@tree中,返回z的指针
 * @tree:	红黑树
 * @z:		新元素
 */
NODE MODULE_FUN_NAME(InTree, delete)(T tree, NODE z);

/*
 * 新建一个tree结构，成功返回指针，失败返回NULL
 * @cmp:	比较函数
 */
T MODULE_FUN_NAME(InTree, new)(void);

/*
 * 释放tree结构，
 * @tree:	树的指针
 */
void MODULE_FUN_NAME(InTree, free)(T *tree);

/*
 * 中序遍历从@x开始的树(以x为根)
 * @tree:	红黑树
 * @x:		树中元素
 * @nil:	空元素
 */
void MODULE_FUN_NAME(InTree, inorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg);

/*
 * 前序遍历从@x开始的树(以x为根)
 * @tree:	红黑树
 * @x:		树中元素
 * @nil:	空元素
 */
void MODULE_FUN_NAME(InTree, preorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg);

/*
 * 后序遍历从@x开始的树(以x为根)
 * @tree:	红黑树
 * @x:		树中元素
 * @nil:	空元素
 */
void MODULE_FUN_NAME(InTree, postorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg);

#undef NODE
#undef T

#endif // end of CI_INTERVAL_TREE_HEADER
