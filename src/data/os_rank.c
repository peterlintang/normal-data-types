
/*
 * 由红黑树扩展而得来的os rank数据结构
 * 主要是多了可以根据数据在树中的排序位置，
 * 快速访问相应元素
 *
 */
/*
 * for charpter 14
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "os_rank.h"

#define T OsRank_T
#define NODE OsRank_Node

static int os_map(void *x, void *priv);
static void os_delete_fix(T tree, NODE x);

static NODE tree_search(T tree, NODE x, void *key)
{
	if (x == tree->nil)
		return NULL;
	if (x == NULL || tree->cmp(x->key, key) == 0)
		return x;
	if (tree->cmp(x->key, key) > 0)
		return tree_search(tree, x->left, key);
	else
		return tree_search(tree, x->right, key);
}

NODE MODULE_FUN_NAME(OsRank, search)(T tree, void *key)
{
	return tree_search(tree, tree->root, key);
}

NODE MODULE_FUN_NAME(OsRank, iterative_search)(T tree, void *key)
{
	NODE x = tree->root;

	while ((x != tree->nil) && (tree->cmp(x->key, key) != 0))
	{
		if (tree->cmp(x->key, key) > 0)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

static NODE tree_minimum(T tree, NODE x)
{
	if (x == tree->nil)
	{
//		fprintf(stdout, "%s: nil: %p\n", __func__, x);
		return x;
	}

	while (x->left != tree->nil)
		x = x->left;

	return x;
}

NODE MODULE_FUN_NAME(OsRank, minimum)(T tree)
{
	NODE x = tree->root;

	if (x == tree->nil)
		return NULL;

	return tree_minimum(tree, x);
}

static NODE tree_maximum(T tree, NODE x)
{
	while (x->right != tree->nil)
		x = x->right;

	return x;
}

NODE MODULE_FUN_NAME(OsRank, maximum)(T tree)
{
	NODE x = tree->root;

	if (x == tree->nil)
		return NULL;

	return tree_maximum(tree, x);
}

NODE MODULE_FUN_NAME(OsRank, successor)(T tree, NODE x)
{
	NODE y = NULL;
	NODE p = NULL;

	if (x->right != tree->nil)
		return tree_minimum(tree, x->right);

	y = x->parent;
	while ((y != tree->nil) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

NODE MODULE_FUN_NAME(OsRank, predecessor)(T tree, NODE x)
{
	NODE y = NULL;
	NODE p = NULL;

	if (x->left != tree->nil)
		return tree_maximum(tree, x->left);

	y = x->parent;
	while ((y != tree->nil) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

static void left_rotate(T tree, NODE x)
{
	NODE y = NULL;

	assert(x != tree->nil);
	assert(x->right != tree->nil);

	y = x->right;

	x->right = y->left;
	if (y->left != tree->nil)
		y->left->parent = x;

	y->parent = x->parent;
	if (x->parent == tree->nil)
		tree->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;

	x->size = x->left->size + x->right->size + 1;
	y->size = y->left->size + y->right->size + 1;
}

/*
 * 13.2-1 answer
 */
static void right_rotate(T tree, NODE y)
{
	NODE x = NULL;

	assert(y != tree->nil);
	assert(y->left != tree->nil);

	x = y->left;

	y->left = x->right;
	if (x->right != tree->nil)
		x->right->parent = y;

	x->parent = y->parent;
	if (y->parent == tree->nil)
		tree->root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	y->parent = x;
	x->right = y;

	y->size = y->left->size + y->right->size + 1;
	x->size = x->left->size + x->right->size + 1;
}

static void rb_insert_fixup(T tree, NODE z)
{
	NODE y = NULL;

	while (z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;
			if (y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else 
			{
				if (z == z->parent->right)
				{
					z = z->parent;
					left_rotate(tree, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				right_rotate(tree, z->parent->parent);
			}
		}
		else
		{
			y = z->parent->parent->left;
			if (y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else 
			{
				if (z == z->parent->left)
				{
					z = z->parent;
					right_rotate(tree, z);// ?
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				left_rotate(tree, z->parent->parent);//?
			}
		}
	}

	tree->root->color = BLACK;
}

NODE MODULE_FUN_NAME(OsRank, insert)(T tree, NODE node)
{
	NODE y = NULL;
	NODE x = NULL;

	node->size = 1;

	y = tree->nil;
	x = tree->root;

	while (x != tree->nil)
	{
		x->size += 1;
		y = x;
		if (x->key > node->key)
			x = x->left;
		else
			x = x->right;
	}

	node->parent = y;
	if (y == tree->nil)
		tree->root = node;
	else if (node->key < y->key)
		y->left = node;
	else
		y->right = node;

	node->left = tree->nil;
	node->right = tree->nil;
	node->color = RED;

	rb_insert_fixup(tree, node);

	return node;
}

static void rb_transplant(T tree, NODE u, NODE v)
{
	if (u->parent == tree->nil)
	{
		tree->root = v;
	}
	else if (u == u->parent->left)
	{
		u->parent->left = v;
	}
	else
	{
		u->parent->right = v;
	}

	v->parent = u->parent;
}

static void	rb_delete_fixup(T tree, NODE x)
{
	NODE w = NULL;

	while ((x != tree->root) && (x->color == BLACK))
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				left_rotate(tree, x->parent);
				w = x->parent->right;
			}
			if ((w->left->color == BLACK) && (w->right->color == BLACK))
			{
				w->color = RED;
				x = x->parent;
			}
			else if (w->right->color == BLACK)
			{
				w->left->color = BLACK;
				w->color = RED;
				right_rotate(tree, w);
				w = x->parent->right;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				left_rotate(tree, x->parent);
				x = tree->root;
			}
		}
		else
		{
			w = x->parent->left;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				right_rotate(tree, x->parent);
				w = x->parent->left;
			}
			if ((w->right->color == BLACK) && (w->left->color == BLACK))
			{
				w->color = RED;
				x = x->parent;
			}
			else if (w->left->color == BLACK)
			{
				w->right->color = BLACK;
				w->color = RED;
				left_rotate(tree, w);
				w = x->parent->left;
			}
			else
			{
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				right_rotate(tree, x->parent);
				x = tree->root;
			}
		}
	}

	x->color = BLACK;
}

NODE MODULE_FUN_NAME(OsRank, delete)(T tree, NODE z)
{
	int y_origin_color = RED;
	NODE y = NULL;
	NODE x = NULL;

	y = z;
	y_origin_color = y->color;

	if (z->left == tree->nil)
	{
		x = z->right;
		rb_transplant(tree, z, z->right);
	}
	else if (z->right == tree->nil)
	{
		x = z->left;
		rb_transplant(tree, z, z->left);
	}
	else
	{
		y = tree_minimum(tree, z->right);
		y_origin_color = y->color;

		x = y->right;
		if (y->parent == z)
		{
			x->parent = y; // bu tai li jie zhe de yi si
		}
		else
		{
			rb_transplant(tree, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rb_transplant(tree, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
		y->size = z->size;
	}

	os_delete_fix(tree, x);
	if (y_origin_color == BLACK)
	{
		rb_delete_fixup(tree, x);
	}

	return z;
}

/*
 *
 *  kuo zhang shu ju cao zuo 
 *
 *  */
int MODULE_FUN_NAME(OsRank, rank)(T tree, NODE x)
{
	int r = 0;
	NODE y = NULL;

	r = x->left->size + 1;
	y = x;

	while (y != tree->nil)
	{
		if (y == y->parent->right)
			r = r + y->parent->left->size + 1;
		y = y->parent;
	}

	return r;
}

NODE MODULE_FUN_NAME(OsRank, select)(T tree, int i)
{
	int org_i = i;
	int r = 0;
	NODE x = NULL;

	assert(tree);

	x = tree->root;

	assert(i <= x->size && i > 0);

	if (i > x->size || i <= 0)
		return NULL;

	while (x != tree->nil)
	{
		r = x->left->size + 1;
		if (i == r)
			return x;
		if (i < r)
			x = x->left;
		else
		{
			i = i - r;
			x = x->right;
		}
	}

	return NULL;
}

static void os_delete_fix(T tree, NODE x)
{
	if (x == tree->nil)
	{
		return ;
	}
	fprintf(stdout, "%s: node: %p, parent: %p, left: %p, right: %p, priv: %d, size: %d, color: %s, \n", 
					__func__, 
					x, x->parent, x->left, x->right, (int)x->key, x->size, x->color == RED ? "red" : "black");

	x = x->parent;
	x->size = x->left->size + x->right->size + 1;

	while (x->parent != tree->nil)
	{
		x->parent->size -= 1;
		x = x->parent;
	}
}

/**********************************/


T MODULE_FUN_NAME(OsRank, new)(int (*cmp)(void *, void *))
{
	T tree = NULL;

	tree = (T)calloc(1, sizeof(*tree));
	if (tree == NULL) 
	{
		return NULL;
	}

	tree->root = tree->nil = (NODE)calloc(1, sizeof(struct NODE));
	if (tree->nil == NULL) 
	{
		free(tree);
		return NULL;
	}
	tree->nil->color = BLACK;
	tree->cmp = cmp;

	return tree;
}

void MODULE_FUN_NAME(OsRank, free)(T *treep)
{
	T tree = NULL;
	NODE node = NULL;

	assert(treep && *treep);

	tree = *treep;

	while ((node = tree->root) != tree->nil)
	{
		MODULE_FUN_NAME(OsRank, delete)(tree, node);
		MODULE_FUN_NAME(OsRank, inorder_walk)(tree, tree->root, os_map, NULL);
		free(node);
	}

	*treep = NULL;
}


void MODULE_FUN_NAME(OsRank, inorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg)
{
	assert(tree && x);

	if (x != tree->nil)
	{
		MODULE_FUN_NAME(OsRank, inorder_walk)(tree, x->left, map, arg);
		if (map)
		{
			if (map(x, arg) != 0)
				return ;
		}
		MODULE_FUN_NAME(OsRank, inorder_walk)(tree, x->right, map, arg);
	}
}

void MODULE_FUN_NAME(OsRank, preorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg)
{
	assert(tree && x);

	if (x != tree->nil)
	{
		if (map)
		{
			if (map(x, arg) != 0)
				return ;
		}
		MODULE_FUN_NAME(OsRank, preorder_walk)(tree, x->left, map, arg);
		MODULE_FUN_NAME(OsRank, preorder_walk)(tree, x->right, map, arg);
	}
}


void MODULE_FUN_NAME(OsRank, postorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg)
{
	assert(tree && x);

	if (x != tree->nil)
	{
		if (map)
		{
			if (map(x, arg) != 0)
				return ;
		}
		MODULE_FUN_NAME(OsRank, postorder_walk)(tree, x->left, map, arg);
		MODULE_FUN_NAME(OsRank, postorder_walk)(tree, x->right, map, arg);
	}
}


/*
 * test code
 */
static int os_cmp(void *arg1, void *arg2)
{
	int num1 = (int )arg1;
	int num2 = (int )arg2;

	if (num1 == num2)
	{
		return 0;
	}
	else if (num1 > num2)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

static int os_map(void *x, void *priv)
{
	NODE node = (NODE )x;
	fprintf(stdout, "node: %p, parent: %p, left: %p, right: %p, priv: %d, size: %d, color: %s\n", 
					node, node->parent, node->left, node->right, (int)node->key, node->size, node->color == RED ? "red" : "black");
	return 0;
}

int main(int argc, char *argv[])
{
	T tree = NULL;
	NODE node = NULL;
	int num = 16;
	int key = 18;
	int index = 8;

	if (argc == 3)
	{
		num = atoi(argv[1]);
		key = atoi(argv[2]);
	}

	tree = MODULE_FUN_NAME(OsRank, new)(os_cmp);
	if (tree == NULL)
	{
		fprintf(stderr, "no mem\n");
		exit(1);
	}
	fprintf(stdout, "tree: %p, tree->root: %p, tree->nil: %p\n", 
					tree, tree->root, tree->nil);


	for (int i = 0; i < num; i++)
	{
		node = (NODE )calloc(1, sizeof(*node));
		if (node)
		{
			node->key = (void *)(i + 1);
//			fprintf(stdout, "inserting key: %d\n", node->key);
			MODULE_FUN_NAME(OsRank, insert)(tree, node);
		}
		else
		{
			fprintf(stderr, "no mem: %d\n", i);
			exit(1);
		}
	}

	fprintf(stdout, "in order wal:\n");
	MODULE_FUN_NAME(OsRank, inorder_walk)(tree, tree->root, os_map, NULL);

	fprintf(stdout, "select...\n");
	for (int i = 0; i < num; i++)
	{
		node = MODULE_FUN_NAME(OsRank, select)(tree, i + 1);
		if (node == NULL)
		{
			fprintf(stderr, "not found index: %d node\n", index);
			continue;
		}

		index = MODULE_FUN_NAME(OsRank, rank)(tree, node);
		fprintf(stdout, "node: %p, parent: %p, left: %p, right: %p, priv: %d, size: %d, color: %s, rank: %d\n", 
					node, node->parent, node->left, node->right, (int)node->key, node->size, node->color == RED ? "red" : "black", index);
	}
	
	fprintf(stdout, "release....\n");

	MODULE_FUN_NAME(OsRank, free)(&tree);

	return 0;
}


