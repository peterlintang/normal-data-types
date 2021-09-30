
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/time.h>



#include "bit.h"
static void test_bit(void)
{
#define BIT_TEST_LEN	1024000000
	Bit_T set = NULL;
	Bit_T set2 = NULL;
	set = MODULE_FUN_NAME(Bit, new)(BIT_TEST_LEN);
	set2 = MODULE_FUN_NAME(Bit, new)(BIT_TEST_LEN);
	if (NULL == set)
	{
		return ;
	}
	MODULE_FUN_NAME(Bit, set)(set, 0, BIT_TEST_LEN - 1);
	MODULE_FUN_NAME(Bit, set)(set2, 0, BIT_TEST_LEN - 1);
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));
//	MODULE_FUN_NAME(Bit, put)(set, 20, 0);
//	MODULE_FUN_NAME(Bit, put)(set, 21, 0);
	MODULE_FUN_NAME(Bit, clear)(set, 1, 10);
	fprintf(stdout, "set bit eq: %d\n", MODULE_FUN_NAME(Bit, eq)(set, set2));
	fprintf(stdout, "set bit leq: %d\n", MODULE_FUN_NAME(Bit, leq)(set, set2));
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, get)(set, 2));
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));
	MODULE_FUN_NAME(Bit, free)(&set);
	MODULE_FUN_NAME(Bit, free)(&set2);

}

#include "dlist.h"
static void test_dlist(void)
{
	List_T list = NULL;
	ListNode_T node = NULL;

	int i;

	list = MODULE_FUN_NAME(List, new)();
	if (NULL == list)
		return ;

	for (i = 0; i < 20; i++)
	{
		node = MODULE_FUN_NAME(ListNode, new)((void *)(i + 1));
		MODULE_FUN_NAME(List, insert)(list, node);
	}

	for (i = MODULE_FUN_NAME(List, count)(list), 
			node = MODULE_FUN_NAME(List, head)(list);
			i > 0;
			i--, node = MODULE_FUN_NAME(List, next)(node))
	{
//		fprintf(stdout, "i: %d, value: %d\n", i, (int)node->priv);
	}

	while (node = MODULE_FUN_NAME(List, head)(list))
	{
//		fprintf(stdout, "value: %d\n", (int)node->priv);
		MODULE_FUN_NAME(List, remove)(list, node);
		MODULE_FUN_NAME(ListNode, free)(&node);
	}

	MODULE_FUN_NAME(List, free)(&list);

}

#if 1
#include "queue_link.h"
static void test_queue_link(void)
{
	Queue_T q = NULL;
	int i;
	int *p = NULL;

	q = MODULE_FUN_NAME(Queue, new)(10);
	if (NULL == q)
		return ;

	for (i = 0; i < 300000; i++)
	{
		MODULE_FUN_NAME(Queue, put)(q, (void *)i);
	}

	while (MODULE_FUN_NAME(Queue, isEmpty)(q) == 0)
	{
		MODULE_FUN_NAME(Queue, get)(q, (void **)&p);
//		fprintf(stdout, "value: %d\n", (int)p);
	}

	MODULE_FUN_NAME(Queue, free)(&q);
}
#endif

#if 1
#include "stack_link.h"
static void test_stack_link(void)
{
	Stack_T s = NULL;
	int i;
	int *p = NULL;

	s = MODULE_FUN_NAME(Stack, new)();
	if (NULL == s)
		return ;

	for (i = 0; i < 300000; i++)
	{
		MODULE_FUN_NAME(Stack, push)(s, (void *)i);
	}

	while (MODULE_FUN_NAME(Stack, isEmpty)(s) == 0)
	{
		MODULE_FUN_NAME(Stack, pop)(s, (void **)&p);
//		fprintf(stdout, "value: %d\n", (int)p);
	}

	MODULE_FUN_NAME(Stack, free)(&s);
}
#endif

#if 0
#include "queue_array.h"
static void test_queue_array(void)
{
	struct aa {
		int a;
		char b[11];
	};


	Queue_T q = NULL;
	int i;
	int *p = NULL;

	q = MODULE_FUN_NAME(Queue, new)(30000, sizeof(struct aa));
	if (NULL == q)
		return ;

	for (i = 0; i < 20000; i++)
	{
		struct aa tmp; 
		tmp.a = i;
		strcpy(tmp.b, "hello");
		MODULE_FUN_NAME(Queue, put)(q, &tmp);
	}

	for (i = 0; i < 2000; i++)
	{
		struct aa *tmp = NULL;
		MODULE_FUN_NAME(Queue, get)(q, &tmp);
//		fprintf(stdout, "first get: value: %d, %s\n", tmp->a, tmp->b);
	}

	for (i = 0; i < 20000; i++)
	{
		struct aa tmp; 
		tmp.a = i;
		strcpy(tmp.b, "world");
		MODULE_FUN_NAME(Queue, put)(q, &tmp);
	}

	while (MODULE_FUN_NAME(Queue, isEmpty)(q) == 0)
	{
		struct aa *tmp = NULL;
		MODULE_FUN_NAME(Queue, get)(q, &tmp);
//		fprintf(stdout, "second get: value: %d, %s\n", tmp->a, tmp->b);
	}

	MODULE_FUN_NAME(Queue, free)(&q, 0);
}
#endif

#if 0
#include "stack_array.h"
static void test_stack_array(void)
{
	Stack_T s = NULL;
	int i;
	int *p = NULL;

	s = MODULE_FUN_NAME(Stack, new)(300000, sizeof(int));
	if (NULL == s)
		return ;

	for (i = 0; i < 300000; i++)
	{
		MODULE_FUN_NAME(Stack, push)(s, &i);
	}

	while (MODULE_FUN_NAME(Stack, isEmpty)(s) == 0)
	{
		MODULE_FUN_NAME(Stack, pop)(s, &p);
//		fprintf(stdout, "value: %d\n", *p);
	}

	MODULE_FUN_NAME(Stack, free)(&s);
}
#endif

#include "ap.h"
static void test_ap(void)
{
	char tmp[40] = { 0 };
	char *p = NULL;
	AP_T ap1 = NULL;
	AP_T ap2 = NULL;
	AP_T ap3 = NULL;
	AP_T ap4 = NULL;
	ap1 = MODULE_FUN_NAME(AP, new)(20);
	ap2 = MODULE_FUN_NAME(AP, new)(30);

	ap3 = MODULE_FUN_NAME(AP, muli)(ap1, 100);
	ap4 = MODULE_FUN_NAME(AP, addi)(ap2, 200);

	p = MODULE_FUN_NAME(AP, tostr)(tmp, 30, 10, ap1);
	fprintf(stdout, "ap1: value: %s\n", p);
	p = MODULE_FUN_NAME(AP, tostr)(tmp, 30, 10, ap2);
	fprintf(stdout, "ap2: value: %s\n", p);
	p = MODULE_FUN_NAME(AP, tostr)(tmp, 30, 10, ap3);
	fprintf(stdout, "ap3: value: %s\n", p);
	p = MODULE_FUN_NAME(AP, tostr)(tmp, 30, 10, ap4);
	fprintf(stdout, "ap4: value: %s\n", p);

	MODULE_FUN_NAME(AP, free)(&ap1);
	MODULE_FUN_NAME(AP, free)(&ap2);
	MODULE_FUN_NAME(AP, free)(&ap3);
	MODULE_FUN_NAME(AP, free)(&ap4);
}

#include "arena.h"
static void test_arena(void)
{
#define TEST_INT_NUM	1024
#define TEST_TIMES		256
	Arena_T area = NULL;

	area = MODULE_FUN_NAME(Arena, new)();
	if (area)
	{
		int *p = NULL;
		for (int k = 0; k < TEST_TIMES; k++)
		{
		for (int j = 0; j < TEST_TIMES; j++)
		{
			p = (int *)MODULE_FUN_NAME(Arena, calloc)(area, TEST_INT_NUM, sizeof(int),
						__FILE__, __LINE__);
			if (p)
			{
			for (int i = 0; i < TEST_INT_NUM; i++)
				p[i] = i + 1000;
			/*
			for (int i = 0; i < TEST_INT_NUM; i++)
				fprintf(stdout, "i: %d\n", p[i]);
			*/
			}
		}
		MODULE_FUN_NAME(Arena, free)(area);
		}
		MODULE_FUN_NAME(Arena, dispose)(&area);
	}
}

#include "thread_pool.h"
static void pool_test_cb(void *cl)
{
	fprintf(stdout, "%d: hello world: %d\n", pthread_self(), (int)cl);
}
static void test_threadPool(void)
{
	int i;
	int ret = 0;
	ThreadPool_T p = NULL;

	p = MODULE_FUN_NAME(ThreadPool, new)(10);
	ret = MODULE_FUN_NAME(ThreadPool, init)(p);
	fprintf(stdout, "%s: init: %d\n", __func__, ret);
	for (i = 0; i < 10000; i++)
	{
		ret = MODULE_FUN_NAME(ThreadPool, post)(p, pool_test_cb, (void *)i);
	//	fprintf(stdout, "%s: post: %d\n", __func__, ret);
	}
	ret = MODULE_FUN_NAME(ThreadPool, destroy)(p);
	fprintf(stdout, "%s: destroy: %d\n", __func__, ret);
	MODULE_FUN_NAME(ThreadPool, free)(&p);

}

#include "array.h"

static void test_array(void)
{
#define ARRAY_TEST_ITEMS	1024
	Array_T array;
	array = MODULE_FUN_NAME(Array, new)(ARRAY_TEST_ITEMS, sizeof(int));
	if (array == NULL) return ;

	
	for (int i = 2; i < ARRAY_TEST_ITEMS; i++)
	{
		MODULE_FUN_NAME(Array, put)(array, i, &i);
	}
	

	fprintf(stdout, "len: %d, size: %d\n", MODULE_FUN_NAME(Array, length)(array), MODULE_FUN_NAME(Array, size)(array));

	for (int i = 0; i < ARRAY_TEST_ITEMS; i++)
	{
		*((int *)MODULE_FUN_NAME(Array, get)(array, i)) += 1000;
//		fprintf(stdout, "%d: %d\n", i, *((int *)MODULE_FUN_NAME(Array, get)(array, i)));
	}
	MODULE_FUN_NAME(Array, free)(&array);

	array = MODULE_FUN_NAME(Array, new)(ARRAY_TEST_ITEMS, sizeof(double));
	for (int i = 2; i < ARRAY_TEST_ITEMS; i++)
	{
		MODULE_FUN_NAME(Array, put)(array, i, &i);
	}
	for (int i = 0; i < ARRAY_TEST_ITEMS; i++)
	{
		*((double *)MODULE_FUN_NAME(Array, get)(array, i)) = i + 1.0;
		fprintf(stdout, "%d: %f\n", i, *((double *)MODULE_FUN_NAME(Array, get)(array, i)));
	}
	Array_T copy;
	copy = MODULE_FUN_NAME(Array, copy)(array, 512);
	fprintf(stdout, "len: %d, size: %d\n", MODULE_FUN_NAME(Array, length)(copy), MODULE_FUN_NAME(Array, size)(copy));
	for (int i = 0; i < MODULE_FUN_NAME(Array, length)(copy); i++)
	{
		fprintf(stdout, "%d: %f\n", i, *((double *)MODULE_FUN_NAME(Array, get)(copy, i)));
	}

	MODULE_FUN_NAME(Array, resize)(copy, 500);
	fprintf(stdout, "len: %d, size: %d\n", MODULE_FUN_NAME(Array, length)(copy), MODULE_FUN_NAME(Array, size)(copy));
	for (int i = 0; i < MODULE_FUN_NAME(Array, length)(copy); i++)
	{
		fprintf(stdout, "%d: %f\n", i, *((double *)MODULE_FUN_NAME(Array, get)(copy, i)));
	}
}


#include "set.h"

static int apply(const void *member, void *cl)
{
	fprintf(stdout, "%d\n", (int)member);
	return 0;
}

static void test_set(void)
{
#define SET_ITEM_LEN	10
	Set_T set = NULL;
	Set_T set2 = NULL;
	Set_T set3 = NULL;

	/* first */
	fprintf(stdout, "\n\nfirst\n\n");
	set = MODULE_FUN_NAME(Set, new)(SET_ITEM_LEN, NULL, NULL);
	for (int i = 0; i < SET_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(Set, put)(set, (void *)(i + 1));
	}

	fprintf(stdout, "len: %d\n", MODULE_FUN_NAME(Set, length)(set));
	fprintf(stdout, "1 is member: %d\n", MODULE_FUN_NAME(Set, member)(set, (void *)1));
	fprintf(stdout, "11 is member: %d\n", MODULE_FUN_NAME(Set, member)(set, (void *)11));
	MODULE_FUN_NAME(Set, map)(set, apply, NULL);

	for (int i = 0; i < SET_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(Set, remove)(set, (void *)(i + 1));
	}

	fprintf(stdout, "len: %d\n", MODULE_FUN_NAME(Set, length)(set));
	MODULE_FUN_NAME(Set, map)(set, apply, NULL);

	/* second */
	fprintf(stdout, "\n\nsecond\n\n");
	for (int i = 0; i < SET_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(Set, put)(set, (void *)(i + 1));
	}

	void *p = NULL;
	for (p = MODULE_FUN_NAME(Set, first)(set); p != NULL; p = MODULE_FUN_NAME(Set, next)(set, p))
	{
		fprintf(stdout, "i: %d\n", (int)p);
		if (MODULE_FUN_NAME(Set, end)(set, p) == 1)
			break;
	}

	/* third */
	fprintf(stdout, "\n\nthird\n\n");
	set2 = MODULE_FUN_NAME(Set, new)(SET_ITEM_LEN, NULL, NULL);
	for (int i = 0; i < SET_ITEM_LEN - 1; i++)
	{
		MODULE_FUN_NAME(Set, put)(set2, (void *)(i + 5));
	}

	fprintf(stdout, "inter\n");
	set3 = MODULE_FUN_NAME(Set, inter)(set, NULL);
	fprintf(stdout, "set:\n");
	MODULE_FUN_NAME(Set, map)(set, apply, NULL);
	fprintf(stdout, "set2:\n");
	MODULE_FUN_NAME(Set, map)(set2, apply, NULL);
	fprintf(stdout, "set3:\n");
	MODULE_FUN_NAME(Set, map)(set3, apply, NULL);

	fprintf(stdout, "diff\n");
	set3 = MODULE_FUN_NAME(Set, diff)(set, NULL);
	fprintf(stdout, "set:\n");
	MODULE_FUN_NAME(Set, map)(set, apply, NULL);
	fprintf(stdout, "set2:\n");
	MODULE_FUN_NAME(Set, map)(set2, apply, NULL);
	fprintf(stdout, "set3:\n");
	MODULE_FUN_NAME(Set, map)(set3, apply, NULL);

	fprintf(stdout, "minus\n");
	set3 = MODULE_FUN_NAME(Set, minus)(set, NULL);	// 内存没有释放
	fprintf(stdout, "set:\n");
	MODULE_FUN_NAME(Set, map)(set, apply, NULL);
	fprintf(stdout, "set2:\n");
	MODULE_FUN_NAME(Set, map)(set2, apply, NULL);
	fprintf(stdout, "set3:\n");
	MODULE_FUN_NAME(Set, map)(set3, apply, NULL);

	fprintf(stdout, "union\n");
	set3 = MODULE_FUN_NAME(Set, union)(set, NULL);
	fprintf(stdout, "set:\n");
	MODULE_FUN_NAME(Set, map)(set, apply, NULL);
	fprintf(stdout, "set2:\n");
	MODULE_FUN_NAME(Set, map)(set2, apply, NULL);
	fprintf(stdout, "set3:\n");
	MODULE_FUN_NAME(Set, map)(set3, apply, NULL);

	void **array = NULL;
	array = MODULE_FUN_NAME(Set, toArray)(set, NULL);
	for (int i = 0; array[i] != NULL; i++)
	{
		fprintf(stdout, "i: %d <--> value: %d\n", i, (int)array[i]);
	
	}

	free(array);
	MODULE_FUN_NAME(Set, free)(&set);
	MODULE_FUN_NAME(Set, free)(&set2);
	MODULE_FUN_NAME(Set, free)(&set3);

}



#include "table.h"

static int table_apply(const void *key, void **pvalue, void *priv)
{
	fprintf(stdout, "%d: %d\n", (int)key, (int)(*pvalue));
	return 0;
}

static void test_table(void)
{
#define TABLE_ITEM_LEN	10

	Table_T table = NULL;

	table = MODULE_FUN_NAME(Table, new)(10, NULL, NULL);

	fprintf(stdout, "first\n");
	for (int i = 0; i < TABLE_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(Table, put)(table, (void *)(i), (void *)i);
	}

	for (int i = 0; i < TABLE_ITEM_LEN; i++)
	{
		fprintf(stdout, "%d: %d\n", MODULE_FUN_NAME(Table, get)(table, (void *)i));
	}

	fprintf(stdout, "second \n");
	for (int i = 0; i < TABLE_ITEM_LEN - 5; i++)
	{
		fprintf(stdout, "remove %d item\n", i);
		MODULE_FUN_NAME(Table, remove)(table, (void *)i);
		MODULE_FUN_NAME(Table, map)(table, table_apply, NULL);
	}


	fprintf(stdout, "third\n");
	void **array = NULL;
	void *key = NULL;
	void *value = NULL;
	array = MODULE_FUN_NAME(Table, toArray)(table, (void *)111);
	for (int i = 0; i < MODULE_FUN_NAME(Table, length)(table) * 2; i += 2)
	{
		key = array[i];
		value = array[i + 1];
		fprintf(stdout, "key: %d, value: %d\n", (int)key, (int)value);
	}
	fprintf(stdout, "end: %d\n", (int)array[MODULE_FUN_NAME(Table, length)(table) * 2]);
	fprintf(stdout, "table len: %d\n", MODULE_FUN_NAME(Table, length)(table));

	MODULE_FUN_NAME(Table, free)(&table);
}


struct test_routine {
	void (*call_back)(void);
	char *name;
};

struct test_routine my_test_routines[] = 
{
//		{test_bit, "bit"},
//		{test_dlist, "dlist"},
//		{test_queue_link, "queue_link"},
//		{test_stack_link, "stack_link"},
//		{test_queue_array, "queue_array"},
//		{test_stack_array, "stack_array"},
//		{test_ap, "ap"},
//		{test_arena, "arena"},
//		{test_threadPool, "threadPool"},
//		{test_array, "array"},
//		{test_set, "set"},
		{test_table, "table"},
		{NULL,NULL},
};

int main(int argc, char *argv[])
{
	int ret = 0;
	int i;
	struct timeval time1;
	struct timeval time2;

	for (i = 0; my_test_routines[i].call_back != NULL; i++)
	{
		fprintf(stdout, "%d testing : %s\n", i, my_test_routines[i].name);
		gettimeofday(&time1, NULL);
		my_test_routines[i].call_back();
		gettimeofday(&time2, NULL);
	fprintf(stdout, "%d testing : %s, elapse: %ld seconds, %ld useconds\n", 
				i, my_test_routines[i].name,
				time2.tv_sec - time1.tv_sec,
				time2.tv_usec - time1.tv_usec);
	}

	return ret;
}
