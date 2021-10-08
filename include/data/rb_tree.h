
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


NODE MODULE_FUN_NAME(RB_Tree, search)(T tree, void *key);

NODE MODULE_FUN_NAME(RB_Tree, iterative_search)(T tree, void *key);

NODE MODULE_FUN_NAME(RB_Tree, minimum)(T tree);

NODE MODULE_FUN_NAME(RB_Tree, maximum)(T tree);

NODE MODULE_FUN_NAME(RB_Tree, successor)(T tree, NODE x);

NODE MODULE_FUN_NAME(RB_Tree, predecessor)(T tree, NODE x);

NODE MODULE_FUN_NAME(RB_Tree, insert)(T tree, NODE node);

NODE MODULE_FUN_NAME(RB_Tree, delete)(T tree, NODE z);

T MODULE_FUN_NAME(RB_Tree, new)(int (*cmp)(void *, void *));

void MODULE_FUN_NAME(RB_Tree, free)(T *tree);

void MODULE_FUN_NAME(RB_Tree, inorder_walk)(T tree, NODE x, NODE nil);

void MODULE_FUN_NAME(RB_Tree, preorder_walk)(T tree, NODE x, NODE nil);

void MODULE_FUN_NAME(RB_Tree, postorder_walk)(T tree, NODE x, NODE nil);

#undef NODE
#undef T

#endif // end of CI_RB_TREE_HEADER
