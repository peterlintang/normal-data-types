
/*
 * for charpter 13
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct node {
	struct node *left;
	struct node *right;
	struct node *parent;
	int key;
#define RED		0x1
#define BLACK	0x2
	int color;
};

struct rb_tree {
	struct node *root;
	struct node *nil;
};

static struct node *tree_search(struct rb_tree *T, 
				struct node *x, int key)
{
	if (x == T->nil)
		return NULL;
	if (x == NULL || x->key == key)
		return x;
	if (x->key > key)
		return tree_search(T, x->left, key);
	else
		return tree_search(T, x->right, key);
}

struct node *rb_tree_search(struct rb_tree *T, int key)
{
	return tree_search(T, T->root, key);
}

struct node *iterative_rb_tree_search(struct rb_tree *T, int key)
{
	struct node *x = T->root;

	while ((x != T->nil) && (x->key != key))
	{
		if (x->key > key)
			x = x->left;
		else
			x = x->right;
	}

	return x;
}

static struct node *tree_minimum(struct rb_tree *T, struct node *x)
{
	if (x == T->nil)
	{
		fprintf(stdout, "%s: nil: %p\n", __func__, x);
		return x;
	}

	while (x->left != T->nil)
		x = x->left;

	return x;
}

struct node *rb_tree_minimum(struct rb_tree *T)
{
	struct node *x = T->root;

	if (x == T->nil)
		return NULL;

	return tree_minimum(T, x);
}

static struct node *tree_maximum(struct rb_tree *T, struct node *x)
{
	while (x->right != T->nil)
		x = x->right;

	return x;
}

struct node *rb_tree_maximum(struct rb_tree *T)
{
	struct node *x = T->root;

	if (x == T->nil)
		return NULL;

	return tree_maximum(T, x);
}

struct node *rb_tree_successor(struct rb_tree *T, struct node *x)
{
	struct node *y = NULL;
	struct node *p = NULL;

	if (x->right != T->nil)
		return tree_minimum(T, x->right);

	y = x->parent;
	while ((y != T->nil) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

struct node *rb_tree_predecessor(struct rb_tree *T, struct node *x)
{
	struct node *y = NULL;
	struct node *p = NULL;

	if (x->left != T->nil)
		return tree_maximum(T, x->left);

	y = x->parent;
	while ((y != T->nil) && (x == y->left))
	{
		x = y;
		y = y->parent;
	}

	return y;
}

static void left_rotate(struct rb_tree *T, struct node *x)
{
	struct node *y = NULL;

	assert(x != T->nil);
	assert(x->right != T->nil);

	y = x->right;

	x->right = y->left;
	if (y->left != T->nil)
		y->left->parent = x;

	y->parent = x->parent;
	if (x->parent == T->nil)
		T->root = y;
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
static void right_rotate(struct rb_tree *T, struct node *y)
{
	struct node *x = NULL;

	assert(y != T->nil);
	assert(y->left != T->nil);

	x = y->left;

	y->left = x->right;
	if (x->right != T->nil)
		x->right->parent = y;

	x->parent = y->parent;
	if (y->parent == T->nil)
		T->root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	y->parent = x;
	x->right = y;
}

static void rb_insert_fixup(struct rb_tree *T, struct node *z)
{
	struct node *y = NULL;

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
					left_rotate(T, z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				right_rotate(T, z->parent->parent);
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
					right_rotate(T, z);// ?
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				left_rotate(T, z->parent->parent);//?
			}
		}
	}

	T->root->color = BLACK;
}

struct node *rb_insert(struct rb_tree *T, struct node *node)
{
	struct node *y = NULL;
	struct node *x = NULL;

	y = T->nil;
	x = T->root;

	while (x != T->nil)
	{
		y = x;
		if (x->key > node->key)
			x = x->left;
		else
			x = x->right;
	}

	node->parent = y;
	if (y == T->nil)
		T->root = node;
	else if (node->key < y->key)
		y->left = node;
	else
		y->right = node;

	node->left = T->nil;
	node->right = T->nil;
	node->color = RED;

	rb_insert_fixup(T, node);

	return node;
}

static void rb_transplant(struct rb_tree *T, 
				struct node *u, struct node *v)
{
	if (u->parent == T->nil)
	{
		T->root = v;
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

static void	rb_delete_fixup(struct rb_tree *T, struct node *x)
{
	struct node *w = NULL;
	while ((x != T->root) && (x->color == BLACK))
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				left_rotate(T, x->parent);
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
				right_rotate(T, w);
				w = x->parent->right;
			}
			w->color = x->parent->color;
			x->parent->color = BLACK;
			w->right->color = BLACK;
			left_rotate(T, x->parent);
			x = T->root;
		}
		else
		{
			w = x->parent->left;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				right_rotate(T, x->parent);
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
				left_rotate(T, w);
				w = x->parent->left;
			}
			w->color = x->parent->color;
			x->parent->color = BLACK;
			w->left->color = BLACK;
			right_rotate(T, x->parent);
			x = T->root;
		}
	}

	x->color = BLACK;
}

struct node *rb_delete(struct rb_tree *T, struct node *z)
{
	int y_origin_color = RED;
	struct node *y = NULL;
	struct node *x = NULL;

	y = z;
	y_origin_color = y->color;

	if (z->left == T->nil)
	{
//		fprintf(stdout, "1 %s: y:%d\n", __func__, y->key);
		x = z->right;
		rb_transplant(T, z, z->right);
	}
	else if (z->right == T->nil)
	{
//		fprintf(stdout, "2 %s: y:%d\n", __func__, y->key);
		x = z->left;
		rb_transplant(T, z, z->left);
	}
	else
	{
		y = tree_minimum(T, z->right);
		y_origin_color = y->color;
//		fprintf(stdout, "3 %s: y:%d\n", __func__, y->key);

		x = y->right;
		if (y->parent == z)
		{
			x->parent = y; // bu tai li jie zhe de yi si
		}
		else
		{
			rb_transplant(T, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		rb_transplant(T, z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if (y_origin_color == BLACK)
	{
		rb_delete_fixup(T, x);
	}

	return z;
}


struct rb_tree *rb_tree_root(void)
{
	struct rb_tree *T = NULL;

	T = (struct rb_tree *)calloc(1, sizeof(*T));
	if (T == NULL) 
	{
		return NULL;
	}

	T->root = T->nil = (struct node *)calloc(1, sizeof(struct node));
	if (T->nil == NULL) 
	{
		free(T);
		return NULL;
	}
	T->nil->color = BLACK;

	return T;
}



void inorder_tree_walk(struct node *x, struct node *nil)
{
//	fprintf(stdout, "%s: x: %p, nil: %p\n", __func__, x, nil);
	if (x != nil)
	{
		inorder_tree_walk(x->left, nil);
		fprintf(stdout, "key: %4d, color: %s \n", 
						x->key, x->color == RED ? "RED" : "BLACK");
		inorder_tree_walk(x->right, nil);
	}
}

void preorder_tree_walk(struct node *x, struct node *nil)
{
//	fprintf(stdout, "%s: x: %p, nil: %p\n", __func__, x, nil);
	if (x != nil)
	{
		fprintf(stdout, "key: %4d, color: %s \n", 
						x->key, x->color == RED ? "RED" : "BLACK");
		preorder_tree_walk(x->left, nil);
		preorder_tree_walk(x->right, nil);
	}
}


void postorder_tree_walk(struct node *x, struct node *nil)
{
//	fprintf(stdout, "%s: x: %p, nil: %p\n", __func__, x, nil);
	if (x != nil)
	{
		postorder_tree_walk(x->left, nil);
		postorder_tree_walk(x->right, nil);
		fprintf(stdout, "key: %4d, color: %s \n", 
						x->key, x->color == RED ? "RED" : "BLACK");
	}
}


/*
 * test code
 */
int main(int argc, char *argv[])
{
	struct rb_tree *T = NULL;
	struct node *node = NULL;
	int num = 16;
	int seed = 79;
	int key = 18;

	if (argc == 4)
	{
		seed = atoi(argv[1]);
		num = atoi(argv[2]);
		key = atoi(argv[3]);
	}

	T = rb_tree_root();
	if (T == NULL)
	{
		fprintf(stderr, "no mem\n");
		exit(1);
	}
	fprintf(stdout, "T: %p, T->root: %p, T->nil: %p\n", 
					T, T->root, T->nil);

	srand(seed);

	for (int i = 0; i < num; i++)
	{
		node = (struct node *)calloc(1, sizeof(*node));
		if (node)
		{
			node->key = rand();
			fprintf(stdout, "inserting key: %d\n", node->key);
			rb_insert(T, node);
		}
		else
		{
			fprintf(stderr, "no mem: %d\n", i);
			exit(1);
		}
	}

	fprintf(stdout, "in order wal:\n");
	inorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "pre order wal:\n");
	preorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "post order wal:\n");
	postorder_tree_walk(T->root, T->nil);

	/*
	node = rb_tree_minimum(T);
	fprintf(stdout, "minimum: %p->%d\n", node, node->key);
	fprintf(stdout, "deleting minim node...\n");
	rb_delete(T, node);
	fprintf(stdout, "done\n");
	node = rb_tree_minimum(T);
	fprintf(stdout, "minimum: %p->%d\n", node, node->key);

	fprintf(stdout, "in order wal:\n");
	inorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "pre order wal:\n");
	preorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "post order wal:\n");
	postorder_tree_walk(T->root, T->nil);

	node = rb_tree_maximum(T);
	fprintf(stdout, "maximum: %p->%d\n", node, node->key);
	fprintf(stdout, "deleting maxim node...\n");
	rb_delete(T, node);
	fprintf(stdout, "done\n");
	node = rb_tree_maximum(T);
	fprintf(stdout, "maximum: %p->%d\n", node, node->key);

	fprintf(stdout, "in order wal:\n");
	inorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "pre order wal:\n");
	preorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "post order wal:\n");
	postorder_tree_walk(T->root, T->nil);
	*/

	fprintf(stdout, "\ndeleting %d...\n", key);
	node = rb_tree_search(T, key);
	if (node)
		rb_delete(T, node);
	else
	{
		fprintf(stderr, "not found\n");
	}
//	rb_delete(T, T->root);
	fprintf(stdout, "in order wal:\n");
	inorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "pre order wal:\n");
	preorder_tree_walk(T->root, T->nil);
	fprintf(stdout, "post order wal:\n");
	postorder_tree_walk(T->root, T->nil);

	return 0;
}









