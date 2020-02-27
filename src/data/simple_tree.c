/*
 * this implements the tree interface which is 
 * very simple.
 *
 */
#include <stdio.h>		/* for stdin stdout stderr and anything else */
#include <stdlib.h>

#include "simple_tree.h"

/*
 * tree_node interfaces
 */
simple_tree_node_t *_new_simple_tree_node(void *data)
{
	simple_tree_node_t *node = calloc(1, sizeof(simple_tree_node_t));

	if (node) {
		node->data = data;
	}
	return node;
}

void _destroy_simple_tree_node(simple_tree_node_t **node_ptr)
{
	if (node_ptr && *node_ptr) {
		if ((*node_ptr)->data) free((*node_ptr)->data);
		free(*node_ptr);
		*node_ptr = NULL;
	}
}

/*
 * tree_root interfaces
 */
simple_tree_t *_new_simple_tree(int32_t (*cmp)(const void *, const void *))
{
	simple_tree_t *root = calloc(1, sizeof(simple_tree_t));
	if (root) {
		root->cmp = cmp;
	}
	return root;
}

simple_tree_node_t *
_insert_node_to_simple_tree(simple_tree_t *tree, simple_tree_node_t *new)
{
	int32_t c = 0;
	if (!tree || !new) {
	//	DEBUGP(DERROR, "_insert_tree_node", "parameters error\n");
		return NULL;
	}

	simple_tree_node_t *n = tree->root;

	if (!n) {
		new->parent 	= NULL;
		new->left 	= NULL;
		new->right 	= NULL;
		tree->root 	= new;
		tree->count	= 1;
	}

	while (n) {
		c = tree->cmp(n->data, new->data);
		if (c < 0) {
			if (!n->right) {
				n->right = new;
				new->parent = n;
				tree->count++;
				return new;
			}
			n = n->right;
		} else if (c > 0) {
			if (!n->left) {
				n->left = new;
				new->parent = n;
				tree->count++;
				return new;
			}
			n = n->left;
		} else {
	//		DEBUGP(DINFO, "_insert_tree_node", 
//				"node already in tree\n");
			return n;
		}
	}
}

simple_tree_node_t *_search_node_simple_tree(simple_tree_t *tree, void *data)
{
	uint32_t 		c  = 0;
	simple_tree_node_t 	*n = NULL;

	if (!tree || !data) {
//		DEBUGP(DERROR, "_search_tree_node", "parameters error\n");
		return NULL;
	}

	n = tree->root;

	while (n) {
		c = tree->cmp(n->data, data);
		if (c < 0) {
			if (!n->right)
				return NULL;
			n = n->right;
		} else if (c > 0) {
			if (!n->left)
				return NULL;
			n = n->left;
		} else {
			return n;
		}
	}
	return NULL;	/*should not be here */
}

/* 
 * caller by inner use , @node must be in @tree 
 * you can use it in your program too.
 * return value : 1 success
 *		  0 no del (fail)
 * 		  -1 error
 */
int _del_node_from_simple_tree(simple_tree_t *tree, simple_tree_node_t *old)
{
	if (!tree || !old)
		return -1;

	if (old == tree->root) {
		return 0; 	/* can not del root ? why  */
	} else if (!old->right || !old->left) { /* single child or nono */
		if (old->parent->left = old) {
			if (old->left)
				old->parent->left = old->left;
			else if (old->right)
				old->parent->left = old->right;
			else
				old->parent->left = NULL;
			tree->count--;
		} else {
			if (old->left)
				old->parent->right = old->left;
			else if (old->right)
				old->parent->right = old->right;
			else
				old->parent->right = NULL;
			tree->count--;
		}
	} else {	/* wow, two children! how? */
		return 0;	/* current dont delete it */
	}
	return 1;
}

/* the caller should free the memory @data & @old(the pointer which returned */
simple_tree_node_t *_del_simple_tree_node(simple_tree_t *tree, void *data)
{
	simple_tree_node_t *old = NULL;

	if (!tree || !data) {
//		DEBUGP(DERROR, "_del_tree_node", "parameters error\n");
		return NULL;
	}

	old = _search_node_simple_tree(tree, data);
	if (!old) {
//		DEBUG(DINFO, "_del_tree_node", "node does not exist\n");
		return NULL;
	}

	if (_del_node_from_simple_tree(tree, old) != 1)
		return NULL;
	return old;
}

simple_tree_node_t *_root_of_simple_tree(simple_tree_t *tree)
{
	return tree ? tree->root : NULL;
}

/* 
 * count the number of elements in the tree.
 * return value  >= 0 the count of elements.
 *		 < 0 error
 */
int32_t _count_of_simple_tree(simple_tree_t *tree)
{
	if (tree)
		return tree->count;
	else
		return -1;
}

/* inner usage */
void __destroy_simple_tree(simple_tree_node_t *root)
{
	if (root) {
		if (root->left)
			__destroy_simple_tree(root->left);
		if (root->right)
			__destroy_simple_tree(root->right);
		_destroy_simple_tree_node(&root);
	}
}

void _destroy_simple_tree(simple_tree_t **tree_ptr)
{
	if (tree_ptr && *tree_ptr) {
		simple_tree_node_t *node = (*tree_ptr)->root;
		__destroy_simple_tree(node);
		free(*tree_ptr);
		*tree_ptr = NULL;
	}
}

