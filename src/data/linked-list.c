
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct item {
	struct item *prev;
	struct item *next;
	int key;
};

struct Linked_list {
	struct item *head;
};


int list_search(struct Linked_list *l, int key)
{
	struct item *x = NULL;

	assert(l);

	x = l->head;
	while ((x != NULL) && (x->key != key))
		x = x->next;

	if (x == NULL)
		return 0;
	else
		return 1;
}

int list_insert(struct Linked_list *l, int key)
{
	struct item *new = NULL;

	assert(l);

	new = (struct item *)calloc(1, sizeof(*new));
	if (new)
	{
		new->key = key;
		new->next = NULL;
		new->prev = NULL;
		if (l->head)
		{
			new->next = l->head;
			l->head->prev = new;
			l->head = new;
		}
		else
		{
			l->head = new;
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

int list_delete(struct Linked_list *l, int key)
{
	struct item *cur = NULL;

	assert(l);

	for (cur = l->head; cur != NULL; cur = cur->next)
	{
		if (cur->key == key)
			break;
	}

	if (cur)
	{
		if (cur->prev)
		{
			cur->prev->next = cur->next;
		}
		if (cur->next)
		{
			cur->next->prev = cur->prev;
		}

		if (cur == l->head)
			l->head = cur->next;

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

	for (cur = l->head; cur != NULL; cur = cur->next)
	{
		fprintf(stdout, "key: %d\n", cur->key);
	}
}


/*
 * test code
 */

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






