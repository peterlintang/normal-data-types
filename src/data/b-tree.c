
#include <stdio.h>
#include <stdlib.h>

/*
 *
 * implement a simple bin search tree
 *
 */

struct node {
	struct node *left;
	struct node *right;
	struct node *parent;
	int key;
};

struct b_tree {
	struct node *root;
};

void inorder_tree_walk(struct node *x)
{
	if (x != NULL)
	{
		inorder_tree_walk(x->left);
		fprintf(stdout, "%4d \n", x->key);
		inorder_tree_walk(x->right);
	}
}

void preorder_tree_walk(struct node *x)
{
	if (x != NULL)
	{
		fprintf(stdout, "%4d \n", x->key);
		inorder_tree_walk(x->left);
		inorder_tree_walk(x->right);
	}
}


void postorder_tree_walk(struct node *x)
{
	if (x != NULL)
	{
		inorder_tree_walk(x->left);
		inorder_tree_walk(x->right);
		fprintf(stdout, "%4d \n", x->key);
	}
}

struct node *tree_search(struct node *x, int key)
{
	if (x == NULL || x->key == key)
		return x;
	if (x->key > key)
		return tree_search(x->left, key);
	else
		return tree_search(x->right, key);
}

struct node *iterative_tree_search(struct node *x, int key)
{
	while ((x != NULL) && (x->key != key))
	{
		if (x->key > key)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

struct node *tree_minimum(struct node *x)
{
	while (x->left != NULL)
		x = x->left;

	return x;
}

struct node *tree_maximum(struct node *x)
{
	while (x->right != NULL)
		x = x->right;

	return x;
}

struct node *tree_successor(struct node *x)
{
	struct node *y = NULL;
	struct node *p = NULL;

	if (x->right)
		return tree_minimum(x->right);

	y = x->parent;
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

struct node *tree_predecessor(struct node *x)
{
	struct node *y = NULL;
	struct node *p = NULL;

	if (x->left)
		return tree_maximum(x->left);

	y = x->parent;
	while ((y != NULL) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

struct node *tree_insert(struct b_tree *tree, struct node *new)
{
	struct node *y = NULL;
	struct node *x = NULL;

	x = tree->root;
	while (x != NULL)
	{
		y = x;
		if (new->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	new->parent = y;
	if (y == NULL)
		tree->root = new;
	else if (new->key < y->key)
		y->left = new;
	else
		y->right = new;

	return new;
}

static struct node *transplant(struct b_tree *tree, 
				struct node *u,
				struct node *v)
{
	if (u->parent == NULL)
		tree->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v != NULL)
		v->parent = u->parent;

	return u;
}

struct node *tree_delete(struct b_tree *tree, struct node *old)
{
	struct node *y = NULL;

	if (old->left == NULL)
		transplant(tree, old, old->right);
	else if (old->right == NULL)
		transplant(tree, old, old->left);
	else
	{
		y = tree_minimum(old->right);
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





/*
 *
 * test code
 *
 */
int main(int argc, char *argv[])
{
	int seed = 73;
	int num = 1024;
	int key = 75;
	struct node *node = NULL;
	struct b_tree tree;

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

	tree.root = NULL;

	srand(seed);

	for (int i = 0; i < num; i++)
	{
		node = (struct node *)calloc(1, sizeof(*node));
		if (node)
		{
			node->key = rand();
			node->left = NULL;
			node->right = NULL;
			node->parent = NULL;
			tree_insert(&tree, node);
		}
		else
		{
			fprintf(stderr, "no mem: %d\n", i);
			exit(1);
		}
	}

	inorder_tree_walk(tree.root);

	node = tree_search(tree.root, key);
	if (node)
	{
		fprintf(stdout, "key: %d to search: %p->%d\n", 
				key, node, node->key);
	}
	else
	{
		fprintf(stdout, "no found key: %d in tree\n", key);
	}

	fprintf(stdout, "root key: %d\n", tree.root->key);

	return 0;
}




