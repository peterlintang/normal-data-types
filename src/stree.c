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
/*
 * implements the stree structure
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "stree.h"

#define T Stree_T

#define NODE StreeNode_T

struct NODE {
	void	*data;

	NODE parent;
	NODE left;
	NODE right;

	unsigned char status;
#define STATUS_UNDEF	0x00000000
#define STATUS_DELETED	0x00000001
#define STATUS_NORMAL	0x00000002

};

struct T {
	NODE root;
	int (*compare)(const void *, const void *);

	unsigned int count;	// why need two these two 
	unsigned int size;	//

	pthread_mutex_t lock;	// should have thread support	// will use in the future , hei hei 
};

/*
 * interfaces of stree_node_t
 */

NODE MODULE_FUN_NAME(StreeNode, new)(void *data)
{
	NODE node = calloc(1, sizeof(*node));

	if (node) {
		node->data = data;
		node->status = STATUS_NORMAL;
	}

	return node;
}

void MODULE_FUN_NAME(StreeNode, destroy)(NODE *pnode)
{
	assert(pnode != NULL);
	assert(*pnode != NULL);

	if ((*pnode)->data) free((*pnode)->data);
	free(*pnode);
	*pnode = NULL;
}

/*
 * the stree interface 
 */

/*
 * name: MODULE_FUN_NAME(Stree, new)
 * description: create a new simple tree, 
 * return value: return the pointer to new tree
 * args: @compare: 
 */
T MODULE_FUN_NAME(Stree, new)(int (*compare)(const void *, const void *))
{
	T tree = calloc(1, sizeof(*tree));

	if (tree) {
		tree->compare = compare;
		pthread_mutex_init(&tree->lock, NULL);
	}

	return tree;
}

/*
 * name: MODULE_FUN_NAME(Stree, destroy)
 * description: destroy a simple tree, 
 * return value: 
 * args: @compare: 
 */
void MODULE_FUN_NAME(Stree, destroy)(T *pptree)
{
}

/*
 * shi yong le di gui ;
 * this function needs test whether is correct or not 
 */
static int __insert_node_to_stree(NODE node, NODE new, 
		int (*cmp)(const void *, const void *))
{
	int	ret = 0;
	if ((!node) || (!new) || (!(ret = cmp(node->data, new->data)))) return ret;

	if (ret < 0) {
		if (node->left) {
			return __insert_node_to_stree(node->left, new, cmp);
		} else {
			node->left = new;
			new->parent = node;
		}
	} else {
		if (node->right) {
			return __insert_node_to_stree(node->right, new, cmp);
		} else {
			node->right = new;
			new->parent = node;
		}
	}

	return ret;
}

/*
 * name: MODULE_FUN_NAME(Stree, insert)
 * description: insert new node to a simple tree, 
 * return value: 
 * args: @: 
 */
NODE MODULE_FUN_NAME(Stree, insert)(T stree, NODE new)
{

	if (!stree || !new) return NULL;

	if (!stree->root) 
		stree->root = new;
	if (!__insert_node_to_stree(stree->root, new, stree->compare))
		goto exit;;

	stree->count++;
	stree->size++;
exit:
	return new;
}

static NODE __find_node_in_stree(NODE node, void *data, 
		int (*cmp)(const void *, const void *))
{
	int	ret;

	if (!node) return NULL;

	if (!(ret = cmp(node->data, data))) 
		return node;
	else if (ret < 0) 
		return __find_node_in_stree(node->left, data, cmp);
	else 
		return __find_node_in_stree(node->right, data, cmp);
}

/*
 * name: MODULE_FUN_NAME(Stree, find)
 * description: find node from a simple tree, 
 * return value: 
 * args: @: 
 */
NODE MODULE_FUN_NAME(Stree, find)(T stree, void *data)
{
	return __find_node_in_stree(stree->root, data, stree->compare);
}

static NODE __prepare_remove_node(NODE old)
{
}

/*
 * name: MODULE_FUN_NAME(Stree, remove)
 * description: remove node from a simple tree, 
 * return value: 
 * args: @: 
 */
NODE MODULE_FUN_NAME(Stree, remove)(T stree, NODE old)
{
	if (!MODULE_FUN_NAME(Stree, find)(stree, old)) return NULL;
}

/*
 * for what ?
 */
/*
 * name: MODULE_FUN_NAME(Stree, clear)
 * description: clear a simpel stree
 * return value: 
 * args: @: 
 */
void MODULE_FUN_NAME(Stree, clear)(T stree)
{
}

/*
 * name: MODULE_FUN_NAME(Stree, count)
 * description: count a simple stree
 * return value: 
 * args: @: 
 */
unsigned int MODULE_FUN_NAME(Stree, count)(T stree)
{
	unsigned int ret;

	pthread_mutex_lock(&stree->lock);
	ret = stree->count;
	pthread_mutex_unlock(&stree->lock);

	return ret;
}

/*
 * name: MODULE_FUN_NAME(Stree, size)
 * description: size a simple stree
 * return value: 
 * args: @: 
 */
unsigned int MODULE_FUN_NAME(Stree, size)(T stree)
{
	unsigned int ret;

	pthread_mutex_lock(&stree->lock);
        ret = stree->size;
        pthread_mutex_unlock(&stree->lock);

	return ret;
}









