
/*
 * 文件名称: sentinel-linked-list.c
 * 功能描述：带哨兵的双向链表
 * 作者：lxj
 * 创建时间: 2021-09-26
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "sentinel-linked-list.h"

#define NODE SenDlinkNode_t

#define T SenDlink_T

struct NODE {
	NODE prev;
	NODE next;
	void *key;
};

struct T {
	struct NODE nil;
	unsigned int count;
};

T MODULE_FUN_NAME(SenDlink, create)(void)
{
	T l = NULL;

	l = (T)calloc(1, sizeof(*l));
	if (l)
	{
		l->nil.next = &l->nil;
		l->nil.prev = &l->nil;
		l->count = 0;
	}

	return l;
}

void MODULE_FUN_NAME(SenDlink, destroy)(T *pl)
{
	assert(pl);
	assert(*pl);

	unsigned int count = MODULE_FUN_NAME(SenDlink, count)(*pl);

	for (unsigned int i = 0; i < count; i++)
	{
		MODULE_FUN_NAME(SenDlink, delete_by_index)(*pl, 0);
	}

	assert((*pl)->nil.next == &((*pl)->nil));
	assert((*pl)->nil.prev == &((*pl)->nil));

	free(*pl);
	*pl = NULL;
}


int MODULE_FUN_NAME(SenDlink, search)(T l, void *key)
{
	NODE x = NULL;

	assert(l);
	assert(key);

	x = l->nil.next;
	while ((x != &(l->nil)) && (x->key != key))
		x = x->next;

	if (x == NULL)
		return -1;
	else
		return 0;
}

int MODULE_FUN_NAME(SenDlink, insert)(T l, void *key)
{
	NODE new = NULL;

	assert(l);
//	assert(key);

	new = (NODE)calloc(1, sizeof(*new));
	if (new)
	{
		new->key = key;
		new->next = l->nil.next;
		l->nil.next->prev = new;
		l->nil.next = new;
		new->prev = &(l->nil);

		l->count++;

		return 0;
	}
	else
	{
		return -1;
	}
}

int MODULE_FUN_NAME(SenDlink, delete)(T l, void *key)
{
	NODE cur = NULL;

	assert(l);
//	assert(key);

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

		l->count--;

		return 0;
	}
	else
	{
		return -1;
	}
}

/**
 * @index start from 0 to l->count - 1
 */
int MODULE_FUN_NAME(SenDlink, delete_by_index)(T l, unsigned int index)
{
	NODE cur = NULL;

	assert(l);
	assert(index >= 0 && index < l->count);

	for (cur = l->nil.next; cur != &(l->nil); cur = cur->next)
	{
		if (index-- == 0)
			break;
	}

	if (cur != &(l->nil))
	{
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;

		free(cur);

		l->count--;

		return 0;
	}
	else
	{
		return -1;
	}
}

unsigned int MODULE_FUN_NAME(SenDlink, count)(T l)
{
	assert(l);
	return l->count;
}

void *MODULE_FUN_NAME(SenDlink, get)(T l, int i)
{
	int len = 0;
	NODE cur = NULL;

	assert(l);
	assert(i >= 0);

	len = MODULE_FUN_NAME(SenDlink, count)(l);

	assert(i < len);
	for (cur = l->nil.next; i != 0; i--, cur = cur->next)
	{
	}

	return cur->key;
}


/*
 * @fun 返回0表示成功，遍历继续；返回非0失败，遍历停止
 *
 * return val: error -1; success 0
 */
int MODULE_FUN_NAME(SenDlink, map)(T l, int (*fun)(void *arg))
{
	NODE cur = NULL;

	assert(l);

	for (cur = l->nil.next; cur != &(l->nil); cur = cur->next)
	{
		if (fun(cur->key) != 0) return -1;
//		fprintf(stdout, "key: %p\n", cur->key);
	}

	return 0;
}


/*
 * test code

#define TEST_NUM	16

static int print(void *arg)
{
	fprintf(stdout, "key: %d\n", (int)arg);
	return 0;
}

int main(int argc, char *argv[])
{
	T l = NULL;
	int ret = 0;

	l = MODULE_FUN_NAME(SenDlink, create)();
	if (l == NULL)
	{
		fprintf(stderr, "no mem\n");
		return -1;
	}

	for (int i = 0; i < TEST_NUM; i++)
	{
		ret = MODULE_FUN_NAME(SenDlink, insert)(l, (void *)i);
		if (ret == -1)
			fprintf(stderr, "insert error\n");
	}

	MODULE_FUN_NAME(SenDlink, map)(l, print);

	for (int i = 0; i < TEST_NUM; i++)
	{
		ret = MODULE_FUN_NAME(SenDlink, delete)(l, (void *)i);
		if (ret == -1)
			fprintf(stderr, "delete error\n");
		ret = MODULE_FUN_NAME(SenDlink, insert)(l, (void *)(i + 100));
		if (ret == -1)
			fprintf(stderr, "insert error\n");
	}

	MODULE_FUN_NAME(SenDlink, map)(l, print);

	ret = MODULE_FUN_NAME(SenDlink, delete)(l, (void *)TEST_NUM);
	if (ret == -1)
		fprintf(stderr, "delete error\n");

	MODULE_FUN_NAME(SenDlink, destroy)(&l);

	return 0;
}
*/







