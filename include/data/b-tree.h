
/*
 * 普通的二叉树
 *
 */
#ifndef CI_B_TREE_HEADER__
#define CI_B_TREE_HEADER__


#include "module.h"

#define T BTree_T
#define NODE BNode_T

typedef struct T *T;
typedef struct NODE *NODE;

/*
 * 新建一个二叉树的节点，成功返回指针，失败NULL
 */
NODE MODULE_FUN_NAME(BTree, NodeNew)(void *priv);

/*
 * 释放一个二叉树的节点内容
 */
void MODULE_FUN_NAME(BTree, NodeFree)(NODE *nodep);

/*
 * 获取二叉树节点的私有数据
 */
void *MODULE_FUN_NAME(BTree, NodeGetPriv)(NODE node);

/*
 * 设置二叉树节点的私有数据,并返回旧的数据
 */
void *MODULE_FUN_NAME(BTree, NodeSetPriv)(NODE node, void *priv);

/*
 * 遍历树，
 * map: 成功返回0，失败非0，并停止遍历
 */
void MODULE_FUN_NAME(BTree, inorder_walk)(T tree, int (*map)(void *priv, void *arg), void *arg);

void MODULE_FUN_NAME(BTree, preorder_walk)(T tree, int (*map)(void *priv, void *arg), void *arg);

void MODULE_FUN_NAME(BTree, postorder_walk)(T tree, int (*map)(void *priv, void *arg), void *arg);

/*
 * 在树tree中查询节点，其值与key相等，找到返回节点，没有返回NULL
 */
NODE MODULE_FUN_NAME(BTree, search)(T tree, void *key);

/*
 * 在树tree中查询节点，其值与key相等，找到返回节点，没有返回NULL
 * 非递归实现
 */
NODE MODULE_FUN_NAME(BTree, iterative_search)(T tree, void *key);

/*
 * 返回tree中以node为跟的最小节点，
 * node为空，以tree的根为开始
 * 如果树为空，直接返回NULL
 */
NODE MODULE_FUN_NAME(BTree, minimum)(T tree, NODE node);

/*
 * 返回tree中以node为跟的最大节点，
 * node为空，以tree的根为开始
 * 如果树为空，直接返回NULL
 */
NODE MODULE_FUN_NAME(BTree, maximum)(T tree, NODE node);

/*
 * 返回x的后继
 */
NODE MODULE_FUN_NAME(BTree, successor)(T tree, NODE x);

/*
 * 返回x的前驱
 */
NODE MODULE_FUN_NAME(BTree, predecessor)(T tree, NODE x);

/*
 * 将节点new插入二叉树中,
 * 返回new节点
 */
NODE MODULE_FUN_NAME(BTree, insert)(T tree, NODE new);

/*
 * 将节点old从树中删除
 * 返回old节点
 */
NODE MODULE_FUN_NAME(BTree, delete)(T tree, NODE old);

/*
 * 新建一棵二叉树
 * 成功返回指针,失败返回NULL
 * cmp: 比较函数,第一个参数大于第二个参数返回1，等于返回0，小于返回-1
 */
T MODULE_FUN_NAME(BTree, new)(int (*cmp)(void *, void *));

/*
 * 是否tree的内容
 */
void MODULE_FUN_NAME(BTree, free)(T *treep);

#undef T
#undef NODE

#endif // end of CI_B_TREE_HEADER__


