
/*
 * filename: 		rb_tree.h
 * description:		实现红黑树数据结构
 * date:			2021-10-08
 * author:			lxj
 */
/*
 * for charpter 13
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "rb_tree.h"

#define T RB_Tree
#define NODE RB_Node

static NODE tree_search(T tree, 
				NODE x, void *key)
{
	if (x == tree->nil)
		return NULL;
	if (x == NULL || tree->cmp(x->priv, key) == 0)
		return x;
	if (tree->cmp(x->priv, key) > 0)
		return tree_search(tree, x->left, key);
	else
		return tree_search(tree, x->right, key);
}

NODE MODULE_FUN_NAME(RB_Tree, search)(T tree, void *key)
{
	assert(tree && key);

	return tree_search(tree, tree->root, key);
}

NODE MODULE_FUN_NAME(RB_Tree, iterative_search)(T tree, void *key)
{
	assert(tree && key);

	NODE x = tree->root;

	while ((x != tree->nil) && tree->cmp(x->priv, key) != 0)
	{
		if (tree->cmp(x->priv, key) > 0)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

static NODE tree_minimum(T tree, NODE x)
{
	int count = 0;
	if (x == tree->nil)
	{
//		fprintf(stdout, "%s: nil: %p\n", __func__, x);
		return x;
	}

	while (x->left != tree->nil)
	{
		count++;
		x = x->left;
	}
	fprintf(stdout, "minimum: %d\n", count);

	return x;
}

NODE MODULE_FUN_NAME(RB_Tree, minimum)(T tree)
{
	assert(tree);

	NODE x = tree->root;

	if (x == tree->nil)
		return NULL;

	return tree_minimum(tree, x);
}

static NODE tree_maximum(T tree, NODE x)
{
	int count = 0;
	while (x->right != tree->nil)
	{
		count++;
		x = x->right;
	}

	fprintf(stdout, "maximum: %d\n", count);
	return x;
}

NODE MODULE_FUN_NAME(RB_Tree, maximum)(T tree)
{
	assert(tree);

	NODE x = tree->root;

	if (x == tree->nil)
		return NULL;

	return tree_maximum(tree, x);
}

NODE MODULE_FUN_NAME(RB_Tree, successor)(T tree, NODE x)
{
	NODE y = NULL;
	NODE p = NULL;

	assert(tree && x);

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

NODE MODULE_FUN_NAME(RB_Tree, predecessor)(T tree, NODE x)
{
	NODE y = NULL;
	NODE p = NULL;

	assert(tree && x);

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

NODE MODULE_FUN_NAME(RB_Tree, insert)(T tree, NODE node)
{
	NODE y = NULL;
	NODE x = NULL;

	assert(tree && node);

	y = tree->nil;
	x = tree->root;

	while (x != tree->nil)
	{
		y = x;
		if (tree->cmp(x->priv, node->priv) > 0)
			x = x->left;
		else
			x = x->right;
	}

	node->parent = y;
	if (y == tree->nil)
		tree->root = node;
	else if (tree->cmp(node->priv, y->priv) < 0)
		y->left = node;
	else
		y->right = node;

	node->left = tree->nil;
	node->right = tree->nil;
	node->color = RED;

	rb_insert_fixup(tree, node);

	return node;
}

static void rb_transplant(T tree, 
				NODE u, NODE v)
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
//			fprintf(stdout, "x: %p, parent: %p, left: %p, right: %p, color: %s\n", 
//							x, x->parent, x->left, x->right, x->color == RED ? "red" : "black");
		if (x == x->parent->left)
		{
			w = x->parent->right;
		//	fprintf(stdout, "w: %p, parent: %p, left: %p, right: %p, color: %s\n", 
	//						w, w->parent, w->left, w->right, w->color == RED ? "red" : "black");
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

NODE MODULE_FUN_NAME(RB_Tree, delete)(T tree, NODE z)
{
	int y_origin_color = RED;
	NODE y = NULL;
	NODE x = NULL;

	assert(tree && z);

	y = z;
	y_origin_color = y->color;

	if (z->left == tree->nil)
	{
//		fprintf(stdout, "1 %s: y:%d\n", __func__, y->key);
		x = z->right;
		rb_transplant(tree, z, z->right);
	}
	else if (z->right == tree->nil)
	{
//		fprintf(stdout, "2 %s: y:%d\n", __func__, y->key);
		x = z->left;
		rb_transplant(tree, z, z->left);
	}
	else
	{
		y = tree_minimum(tree, z->right);
		y_origin_color = y->color;
//		fprintf(stdout, "3 %s: y:%d\n", __func__, y->key);

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
	}

	if (y_origin_color == BLACK)
	{
		rb_delete_fixup(tree, x);
	}

	return z;
}


T MODULE_FUN_NAME(RB_Tree, new)(int (*cmp)(void *, void *))
{
	T tree = NULL;

	assert(cmp);

	tree = (T)calloc(1, sizeof(*tree));
	if (tree == NULL) 
	{
		return NULL;
	}

	tree->root = tree->nil = (NODE )calloc(1, sizeof(struct NODE));
	if (tree->nil == NULL) 
	{
		free(tree);
		return NULL;
	}
	tree->nil->color = BLACK;
	tree->cmp = cmp;

	return tree;
}

void MODULE_FUN_NAME(RB_Tree, free)(T *tree)
{
	assert(tree && *tree);
}



void MODULE_FUN_NAME(RB_Tree, inorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg)
{
	assert(tree && x);

	if (x != tree->nil)
	{
		MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, x->left, map, arg);
		if (map)
		{
			if (map(x, arg) != 0) 
				return ;
		}
//		fprintf(stdout, "priv: %p, color: %s \n", 
//						x->priv, x->color == RED ? "RED" : "BLACK");
		MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, x->right, map, arg);
	}
}

void MODULE_FUN_NAME(RB_Tree, preorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg)
{
	assert(tree && x);

	if (x != tree->nil)
	{
		if (map) 
		{
			if (map(x->priv, arg) != 0)
				return ;
		}
//		fprintf(stdout, "priv: %p, color: %s \n", 
//						x->priv, x->color == RED ? "RED" : "BLACK");
		MODULE_FUN_NAME(RB_Tree, preorder_walk)(tree, x->left, map, arg);
		MODULE_FUN_NAME(RB_Tree, preorder_walk)(tree, x->right, map, arg);
	}
}


void MODULE_FUN_NAME(RB_Tree, postorder_walk)(T tree, NODE x, int (*map)(void *, void *), void *arg)
{
	assert(tree && x);

	if (x != tree->nil)
	{
		MODULE_FUN_NAME(RB_Tree, postorder_walk)(tree, x->left, map, arg);
		MODULE_FUN_NAME(RB_Tree, postorder_walk)(tree, x->right, map, arg);
		if (map) 
		{
			if (map(x->priv, arg) != 0) 
				return ;
		}
//		fprintf(stdout, "priv: %p, color: %s \n", 
//						x->priv, x->color == RED ? "RED" : "BLACK");
	}
}


#if 0
/*
 * test code
 */
static int cmp(void *arg1, void *arg2)
{
	int num1 = (int )arg1;
	int num2 = (int )arg2;

	return arg1 - arg2 > 0 ? 1 : ((arg1 - arg2 == 0) ? 0 : -1 );
}

static int myprint(void *priv, void *arg)
{
	fprintf(stdout, "%d\n", (int)priv);
	return 0;
}

int main(int argc, char *argv[])
{
	T tree = NULL;
	NODE node = NULL;
	int num = 16;
	int seed = 79;
	int key = 18;

	if (argc == 4)
	{
		seed = atoi(argv[1]);
		num = atoi(argv[2]);
		key = atoi(argv[3]);
	}

	tree = MODULE_FUN_NAME(RB_Tree, new)(cmp);
	if (tree == NULL)
	{
		fprintf(stderr, "no mem\n");
		exit(1);
	}
	fprintf(stdout, "tree: %p, tree->root: %p, tree->nil: %p\n", 
					tree, tree->root, tree->nil);

	srand(seed);

	for (int i = 0; i < num; i++)
	{
		node = (NODE)calloc(1, sizeof(*node));
		if (node)
		{
			node->priv = (void *)(rand() % 100);
			fprintf(stdout, "inserting priv: %d\n", (int)node->priv);
			MODULE_FUN_NAME(RB_Tree, insert)(tree, node);
		}
		else
		{
			fprintf(stderr, "no mem: %d\n", i);
			exit(1);
		}
	}

	fprintf(stdout, "in order wal:\n");
	MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, tree->root, myprint, NULL);
	fprintf(stdout, "pre order wal:\n");
	MODULE_FUN_NAME(RB_Tree, preorder_walk)(tree, tree->root, myprint, NULL);
	fprintf(stdout, "post order wal:\n");
	MODULE_FUN_NAME(RB_Tree, postorder_walk)(tree, tree->root, myprint, NULL);

	fprintf(stdout, "\ndeleting %d...\n", key);
	node = MODULE_FUN_NAME(RB_Tree, search)(tree, (void *)key);
	if (node)
		MODULE_FUN_NAME(RB_Tree, delete)(tree, node);
	else
	{
		fprintf(stderr, "not found\n");
	}
	fprintf(stdout, "in order wal:\n");
	MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, tree->root, myprint, NULL);
	fprintf(stdout, "pre order wal:\n");
	MODULE_FUN_NAME(RB_Tree, preorder_walk)(tree, tree->root, myprint, NULL);
	fprintf(stdout, "post order wal:\n");
	MODULE_FUN_NAME(RB_Tree, postorder_walk)(tree, tree->root, myprint, NULL);

	return 0;
}
#endif



