/*************************************
 *
 * filename:	stree.h
 * description:	implements the simple tree
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_SIMPLE_TREE_H
#define CI_SIMPLE_TREE_H

#include "module.h"

#define T Stree_T
typedef struct T *T;

#define NODE StreeNode_T
typedef struct NODE *NODE;


NODE MODULE_FUN_NAME(StreeNode, new)(void *data);
void MODULE_FUN_NAME(StreeNode, destroy)(NODE *pnode);


/*
 * name: MODULE_FUN_NAME(Stree, new)
 * description: create a new simple tree, 
 * return value: return the pointer to new tree
 * args: @compare: 
 */
T MODULE_FUN_NAME(Stree, new)(int (*compare)(const void *, const void *));

/*
 * name: MODULE_FUN_NAME(Stree, destroy)
 * description: destroy a simple tree, 
 * return value: 
 * args: @: 
 */
void MODULE_FUN_NAME(Stree, destroy)(T *pstree);

static int __insert_node_to_stree(NODE node, NODE new,  
                int (*cmp)(const void *, const void *));
/*
 * name: MODULE_FUN_NAME(Stree, insert)
 * description: insert new node to a simple tree, 
 * return value: 
 * args: @: 
 */
NODE MODULE_FUN_NAME(Stree, insert)(T stree, NODE new);

static NODE __find_node_in_stree(NODE node, void *data,
                int (*cmp)(const void *, const void *));
/*
 * name: MODULE_FUN_NAME(Stree, find)
 * description: find node from a simple tree, 
 * return value: 
 * args: @: 
 */
NODE MODULE_FUN_NAME(Stree, find)(T stree, void *data);

/*
 * name: MODULE_FUN_NAME(Stree, remove)
 * description: remove node from a simple tree, 
 * return value: 
 * args: @: 
 */
NODE MODULE_FUN_NAME(Stree, remove)(T stree, NODE old);

/*
 * name: MODULE_FUN_NAME(Stree, clear)
 * description: clear a simpel stree
 * return value: 
 * args: @: 
 */
void MODULE_FUN_NAME(Stree, clear)(T stree);

/*
 * name: MODULE_FUN_NAME(Stree, count)
 * description: count a simple stree
 * return value: 
 * args: @: 
 */
unsigned int MODULE_FUN_NAME(Stree, count)(T stree);

/*
 * name: MODULE_FUN_NAME(Stree, size)
 * description: size a simple stree
 * return value: 
 * args: @: 
 */
unsigned int MODULE_FUN_NAME(Stree, size)(T stree);


#undef T

#endif // CI_SIMPLE_TREE_H





