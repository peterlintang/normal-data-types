
/*
 * filename: 		rb_tree.h
 * description:		实现红黑树数据结构
 * date:			2021-10-08
 * author:			lxj
 */

/*
 * for charpter 13
 */


#ifndef CI_RB_TREE_HEADER
#define CI_RB_TREE_HEADER


#include "module.h"

#define T RB_Tree
#define NODE RB_Node

typedef struct T *T;
typedef struct NODE *NODE;

struct NODE {
	NODE left;
	NODE right;
	NODE parent;
	void *priv;
#define RED             0x1
#define BLACK   		0x2
	int color;
};

struct T {
	NODE root;
	NODE nil;
	int (*cmp)(void *, void *);
};


/*
 * 根据关键字key查找元素，找到返回元素指针，找不到返回空(递归查找)
 * @tree:	红黑树
 * @key:	关键字
 */
NODE MODULE_FUN_NAME(RB_Tree, search)(T tree, void *key);

/*
 * 根据关键字key查找元素，找到返回元素指针，找不到返回空 (非递归)
 * @tree:	红黑树
 * @key:	关键字
 */
NODE MODULE_FUN_NAME(RB_Tree, iterative_search)(T tree, void *key);

/*
 * 返回指向树的最小元素,若树空，返回空
 * @tree:	红黑树
 */
NODE MODULE_FUN_NAME(RB_Tree, minimum)(T tree);

/*
 * 返回指向树的最大元素,若树空，返回空
 * @tree:	红黑树
 */
NODE MODULE_FUN_NAME(RB_Tree, maximum)(T tree);

/*
 * 返回指向树中元素@x的后缀元素,若没有返回空
 * @tree:	红黑树
 * @x:		树中元素
 */
NODE MODULE_FUN_NAME(RB_Tree, successor)(T tree, NODE x);

/*
 * 返回指向树中元素@x的前缀元素,若没有返回空
 * @tree:	红黑树
 * @x:		树中元素
 */
NODE MODULE_FUN_NAME(RB_Tree, predecessor)(T tree, NODE x);

/*
 * 将@z插入树@tree中,返回z的指针
 * @tree:	红黑树
 * @z:		新元素
 */
NODE MODULE_FUN_NAME(RB_Tree, insert)(T tree, NODE z);

/*
 * 将@z插入树@tree中,返回z的指针
 * @tree:	红黑树
 * @z:		新元素
 */
NODE MODULE_FUN_NAME(RB_Tree, delete)(T tree, NODE z);

/*
 * 新建一个tree结构，成功返回指针，失败返回NULL
 * @cmp:	比较函数
 */
T MODULE_FUN_NAME(RB_Tree, new)(int (*cmp)(void *, void *));

/*
 * 释放tree结构，
 * @tree:	树的指针
 */
void MODULE_FUN_NAME(RB_Tree, free)(T *tree);

/*
 * 中序遍历从@x开始的树(以x为根)
 * @tree:	红黑树
 * @x:		树中元素
 * @nil:	空元素
 */
void MODULE_FUN_NAME(RB_Tree, inorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg);

/*
 * 前序遍历从@x开始的树(以x为根)
 * @tree:	红黑树
 * @x:		树中元素
 * @nil:	空元素
 */
void MODULE_FUN_NAME(RB_Tree, preorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg);

/*
 * 后序遍历从@x开始的树(以x为根)
 * @tree:	红黑树
 * @x:		树中元素
 * @nil:	空元素
 */
void MODULE_FUN_NAME(RB_Tree, postorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg);

#undef NODE
#undef T

#endif // end of CI_RB_TREE_HEADER
