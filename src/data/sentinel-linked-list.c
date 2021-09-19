
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"


int list_search(struct Linked_list *l, void *key)
{
	struct item *x = NULL;

	assert(l);

	x = l->nil.next;
	while ((x != &(l->nil)) && (x->key != key))
		x = x->next;

	if (x == NULL)
		return 0;
	else
		return 1;
}

int list_insert(struct Linked_list *l, void *key)
{
	struct item *new = NULL;

	assert(l);

	new = (struct item *)calloc(1, sizeof(*new));
	if (new)
	{
		new->key = key;
		new->next = l->nil.next;
		l->nil.next->prev = new;
		l->nil.next = new;
		new->prev = &(l->nil);

		return 0;
	}
	else
	{
		return -1;
	}
}

int list_delete(struct Linked_list *l, void *key)
{
	struct item *cur = NULL;

	assert(l);

	for (cur = l->nil.next; cur != &(l->nil); cur = cur->next)
	{
		if (cur->key == key)
			break;
	}

	if (cur != &(l->nil))
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;

		free(cur);

		return 0;
	}
	else
	{
		return -1;
	}
}

void list_print(struct Linked_list *l)
{
	struct item *cur = NULL;

	assert(l);

	for (cur = l->nil.next; cur != &(l->nil); cur = cur->next)
	{
		fprintf(stdout, "key: %p\n", cur->key);
	}
}


/*
 * test code

#define TEST_NUM	16

int main(int argc, char *argv[])
{
	struct Linked_list *l = NULL;
	int ret = 0;

	l = (struct Linked_list *)calloc(1, sizeof(*l));
	if (l == NULL)
	{
		fprintf(stderr, "no mem\n");
		return -1;
	}

	l->nil.next = &l->nil;
	l->nil.prev = &l->nil;

	for (int i = 0; i < TEST_NUM; i++)
	{
		ret = list_insert(l, i);
		if (ret == -1)
			fprintf(stderr, "insert error\n");
	}

	list_print(l);

	for (int i = 0; i < TEST_NUM; i++)
	{
		ret = list_delete(l, i);
		if (ret == -1)
			fprintf(stderr, "delete error\n");
	}

	ret = list_delete(l, TEST_NUM);
	if (ret == -1)
		fprintf(stderr, "delete error\n");

	return 0;
}
*/






