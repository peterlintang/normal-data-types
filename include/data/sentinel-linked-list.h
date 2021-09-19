
#ifndef SENTINEL_LINKED_LIST_H
#define SENTINEL_LINKED_LIST_H

struct item {
	struct item *prev;
	struct item *next;
	void *key;
};

struct Linked_list {
	struct item nil;
};


int list_search(struct Linked_list *l, void *key);
int list_insert(struct Linked_list *l, void *key);
int list_delete(struct Linked_list *l, void *key);
void list_print(struct Linked_list *l);

#endif // end of SENTINEL_LINKED_LIST_H

