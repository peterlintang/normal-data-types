
/*
 * 普通的二叉树
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "module.h"
#include "b-tree.h"

#define T BTree_T
#define NODE BNode_T

struct NODE {
	NODE left;
	NODE right;
	NODE parent;
	void *key;
};

struct T {
	NODE root;
	int (*cmp)(void *, void *);
};


NODE MODULE_FUN_NAME(BTree, NodeNew)(void *priv)
{
	NODE node = NULL;

	node = (NODE)calloc(1, sizeof(*node));
	if (node)
	{
		node->key = priv;
	}

	return node;
}

void MODULE_FUN_NAME(BTree, NodeFree)(NODE *nodep)
{
	assert(nodep && *nodep);

	free(*nodep);
	*nodep = NULL;
}

void *MODULE_FUN_NAME(BTree, NodeGetPriv)(NODE node)
{
	assert(node);

	return node->key;
}

void *MODULE_FUN_NAME(BTree, NodeSetPriv)(NODE node, void *priv)
{
	assert(node);

	void *old = node->key;

	node->key = priv;

	return old;
}

/*
 * 接口函数负责检查参数
 */
static void inorder_walk(NODE x, int (*map)(void *priv, void *arg), void *arg)
{
	if (x == NULL)
	{
		return ;
	}

	inorder_walk(x->left, map, arg);
	if (map)
	{
		if (map(x->key, arg) != 0)
		{
			return ;
		}
	}
	inorder_walk(x->right, map, arg);
}

void MODULE_FUN_NAME(BTree, inorder_walk)(T tree, int (*map)(void *priv, void *arg), void *arg)
{
	assert(tree);

	if (tree->root)
	{
		inorder_walk(tree->root, map, arg);
	}
}

void preorder_walk(NODE x, int (*map)(void *priv, void *arg), void *arg)
{
	if (x == NULL)
	{
		return ;
	}

	if (map)
	{
		if (map(x->key, arg) != 0)
		{
			return ;
		}
	}
	preorder_walk(x->left, map, arg);
	preorder_walk(x->right, map, arg);
}

void MODULE_FUN_NAME(BTree, preorder_walk)(T tree, int (*map)(void *priv, void *arg), void *arg)
{
	assert(tree);

	if (tree->root)
	{
		preorder_walk(tree->root, map, arg);
	}
}


void postorder_walk(NODE x, int (*map)(void *priv, void *arg), void *arg)
{
	if (x == NULL)
	{
		return ;
	}

	postorder_walk(x->left, map, arg);
	postorder_walk(x->right, map, arg);
	if (map)
	{
		if (map(x->key, arg) != 0)
		{
			return ;
		}
	}
}

void MODULE_FUN_NAME(BTree, postorder_walk)(T tree, int (*map)(void *priv, void *arg), void *arg)
{
	assert(tree);

	if (tree->root)
	{
		postorder_walk(tree->root, map, arg);
	}
}





static NODE _search(T tree, NODE x, void *key)
{
	int ret = 0;
	if (x == NULL)
	{
		return NULL;
	}

	ret = tree->cmp(x->key, key);
	if (ret > 0)
		return _search(tree, x->left, key);
	else if (ret < 0)
		return _search(tree, x->right, key);
	else
		return x;
}

NODE MODULE_FUN_NAME(BTree, search)(T tree, void *key)
{
	assert(tree);

	return _search(tree, tree->root, key);
}

NODE MODULE_FUN_NAME(BTree, iterative_search)(T tree, void *key)
{
	assert(tree);

	int ret = 0;
	NODE x = tree->root;

	while ((x != NULL))
	{
		ret = tree->cmp(x->key, key);
		if (ret > 0)
			x = x->left;
		else if (ret < 0)
			x = x->right;
		else
			return x;
	}

	return x;
}

NODE MODULE_FUN_NAME(BTree, minimum)(T tree, NODE x)
{
	assert(tree);

	int count = 0;

	if (tree->root == NULL) return NULL;

	if (x == NULL) x = tree->root;

	while (x->left != NULL)
	{
		x = x->left;
		count++;
	}

//	fprintf(stdout, "%s: count: %d\n", __func__, count);

	return x;
}

NODE MODULE_FUN_NAME(BTree, maximum)(T tree, NODE x)
{
	assert(tree);

	int count = 0;

	if (tree->root == NULL) return NULL;

	if (x == NULL) x = tree->root;

	while (x->right != NULL)
	{
		x = x->right;
		count++;
	}

//	fprintf(stdout, "%s: count: %d\n", __func__, count);

	return x;
}

NODE MODULE_FUN_NAME(BTree, successor)(T tree, NODE x)
{
	assert(tree && x);

	NODE y = NULL;
	NODE p = NULL;

	if (x->right)
		return MODULE_FUN_NAME(BTree, minimum)(tree, x->right);

	y = x->parent;
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

NODE MODULE_FUN_NAME(BTree, predecessor)(T tree, NODE x)
{
	assert(tree && x);

	NODE y = NULL;
	NODE p = NULL;

	if (x->left)
		return MODULE_FUN_NAME(BTree, maximum)(tree, x->left);

	y = x->parent;
	while ((y != NULL) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

NODE MODULE_FUN_NAME(BTree, insert)(T tree, NODE new)
{
	assert(tree && new);

	NODE y = NULL;
	NODE x = NULL;

	x = tree->root;
	while (x != NULL)
	{
		y = x;
		if (tree->cmp(new->key, x->key) < 0)
			x = x->left;
		else
			x = x->right;
	}

	new->parent = y;
	if (y == NULL)
	{
		tree->root = new;
		new->parent = NULL;
	}
	else if (tree->cmp(new->key, y->key) < 0)
		y->left = new;
	else
		y->right = new;

	return new;
}

static NODE transplant(T tree, NODE u, NODE v)
{
	if (u->parent == NULL)
	{
		tree->root = v;
	}
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v != NULL)
		v->parent = u->parent;

	return u;
}

NODE MODULE_FUN_NAME(BTree, delete)(T tree, NODE old)
{
	assert(tree && old);

	NODE y = NULL;

	if (old->left == NULL)
	{
		transplant(tree, old, old->right);
	}
	else if (old->right == NULL)
	{
		transplant(tree, old, old->left);
	}
	else
	{
		y = MODULE_FUN_NAME(BTree, minimum)(tree, old->right);
		if (y->parent != old)
		{
			transplant(tree, y, y->right);
			y->right = old->right;
			y->right->parent = y;
		}
		transplant(tree, old, y);
		y->left = old->left;
		y->left->parent = y;
	}

	return old;
}


T MODULE_FUN_NAME(BTree, new)(int (*cmp)(void *, void *))
{
	assert(cmp);

	T tree = NULL;

	tree = (T)calloc(1, sizeof(*tree));
	if (tree)
	{
		tree->cmp = cmp;
	}

	return tree;
}

void MODULE_FUN_NAME(BTree, free)(T *treep)
{
	assert(treep && *treep);

	free(*treep);
	*treep = NULL;
}



#if 0
/*
 *
 * test code
 *
 */

static int print(void *priv, void *arg)
{
	fprintf(stdout, "%d\n", (int)priv);
	return 0;
}

static int cmp(void *priv, void *arg)
{
	int num1 = (int)priv;
	int num2 = (int)arg;

	if (num1 > num2) return 1;
	else if (num1 < num2) return -1;
	else return 0;
}

int main(int argc, char *argv[])
{
	int seed = 73;
	int num = 1024;
	int key = 75;
	NODE node = NULL;
	T tree;

	if (argc == 4)
	{
		seed = atoi(argv[1]);
		num = atoi(argv[2]);
		key = atoi(argv[3]);
	}

	if (argc == 3)
	{
		seed = atoi(argv[1]);
		num = atoi(argv[2]);
	}
	
	if (argc == 2)
	{
		seed = atoi(argv[1]);
	}

	tree = MODULE_FUN_NAME(BTree, new)(cmp);

	srand(seed);

	for (int i = 0; i < num; i++)
	{
		node = (NODE )calloc(1, sizeof(*node));
		if (node)
		{
			node->key = (void *)(rand() % num + 1);
			node->left = NULL;
			node->right = NULL;
			node->parent = NULL;
			MODULE_FUN_NAME(BTree, insert)(tree, node);
		}
		else
		{
			fprintf(stderr, "no mem: %d\n", i);
			exit(1);
		}
	}

	MODULE_FUN_NAME(BTree, inorder_walk)(tree, print, NULL);

	node = MODULE_FUN_NAME(BTree, search)(tree, (void *)key);
	if (node)
	{
		fprintf(stdout, "key: %d to search: %p->%d\n", 
				key, node, (int)(node->key));
	}
	else
	{
		fprintf(stdout, "no found key: %d in tree\n", key);
	}

	MODULE_FUN_NAME(BTree, free)(&tree);

	return 0;
}

#endif

