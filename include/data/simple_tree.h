/*
 * this implements the simple tree interface
 *
 *
 */
#ifndef __simple_tree_H__
#define __simple_tree_H__
#include <stdint.h>		/* for uint32_t */

typedef struct __simple_tree_node_t {
	void *data;
	struct __simple_tree_node_t *parent;
	struct __simple_tree_node_t *left;
	struct __simple_tree_node_t *right;
} simple_tree_node_t;

/* tree node interfaces */;
simple_tree_node_t *_new_simple_tree_node(void *data);
void _destroy_simple_tree_node(simple_tree_node_t **node_ptr);


typedef struct __simple_tree_t {
	struct __simple_tree_node_t *root;
	int32_t (*cmp)(const void *data1, const void *data2); /* compare function if data1 < data2 return value < 0, .... */
	int32_t count;						/* the num of elements in tree */
} simple_tree_t;

int _del_node_from_simple_tree(simple_tree_t *tree, simple_tree_node_t *old);

/* tree interfaces */
simple_tree_t *_new_simple_tree(int32_t (*cmp)(const void *, const void *));
simple_tree_node_t *_insert_node_to_simple_tree(simple_tree_t *tree, simple_tree_node_t *new);
simple_tree_node_t *_search_node_simple_tree(simple_tree_t *tree, void *data);
simple_tree_node_t *_del_simple_tree_node(simple_tree_t *tree, void *data);
simple_tree_node_t *_root_of_simple_tree(simple_tree_t *tree);
int32_t _count_of_simple_tree(simple_tree_t *tree);
void _destroy_simple_tree(simple_tree_t **tree_ptr);

#endif /* end of __simple_tree_H__ */
