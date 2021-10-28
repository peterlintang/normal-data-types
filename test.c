
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define DLIST_ITEM_LEN	10240000
	ListD_T list = NULL;
	ListDNode_T node = NULL;

	int i;

	list = MODULE_FUN_NAME(ListD, new)();
	if (NULL == list)
		return ;

	for (i = 0; i < DLIST_ITEM_LEN; i++)
	{
		node = MODULE_FUN_NAME(ListDNode, new)((void *)(i + 1));
		MODULE_FUN_NAME(ListD, insert)(list, node);
	}

	for (i = MODULE_FUN_NAME(ListD, count)(list), 
			node = MODULE_FUN_NAME(ListD, head)(list);
			i > 0;
			i--, node = MODULE_FUN_NAME(ListD, next)(node))
	{
		fprintf(stdout, "i: %d, value: %d\n", i, (int)node->priv);
	}

	while (node = MODULE_FUN_NAME(ListD, head)(list))
	{
		fprintf(stdout, "value: %d\n", (int)node->priv);
		MODULE_FUN_NAME(ListD, remove)(list, node);
		MODULE_FUN_NAME(ListDNode, free)(&node);
	}

	MODULE_FUN_NAME(ListD, free)(&list);

}

#if 1
#include "queue_link.h"
static void test_queue_link(void)
{
#define QUUEE_LINK_ITEM_LEN	10240000
	QueueL_T q = NULL;
	int i;
	int *p = NULL;

	q = MODULE_FUN_NAME(QueueL, new)(10);
	if (NULL == q)
		return ;

	for (i = 0; i < QUUEE_LINK_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(QueueL, put)(q, (void *)i);
	}

	while (MODULE_FUN_NAME(QueueL, isEmpty)(q) == 0)
	{
		MODULE_FUN_NAME(QueueL, get)(q, (void **)&p);
		fprintf(stdout, "value: %d\n", (int)p);
	}

	MODULE_FUN_NAME(QueueL, free)(&q);
}
#endif

#if 1
#include "stack_link.h"
static void test_stack_link(void)
{
#define STACK_LINK_ITEM_LEN	1024000
	StackL_T s = NULL;
	int i;
	int *p = NULL;

	s = MODULE_FUN_NAME(StackL, new)();
	if (NULL == s)
		return ;

	for (i = 0; i < STACK_LINK_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(StackL, push)(s, (void *)i);
	}

	while (MODULE_FUN_NAME(StackL, isEmpty)(s) == 0)
	{
		MODULE_FUN_NAME(StackL, pop)(s, (void **)&p);
		fprintf(stdout, "value: %d\n", (int)p);
	}

	MODULE_FUN_NAME(StackL, free)(&s);
}
#endif

#if 1
#include "queue_array.h"
static void test_queue_array(void)
{
#define QUEUEA_ITEM_LEN	10240000
	struct aa {
		int a;
		char b[11];
	};


	QueueA_T q = NULL;
	int i;
	int *p = NULL;

	q = MODULE_FUN_NAME(QueueA, new)(QUEUEA_ITEM_LEN + 1, sizeof(struct aa));
	if (NULL == q)
		return ;

	fprintf(stdout, "putiing \n");
	for (i = 0; i < QUEUEA_ITEM_LEN; i++)
	{
		struct aa tmp; 
		tmp.a = i;
		strcpy(tmp.b, "hello");
		fprintf(stdout, "i: %d, \n", i);
		MODULE_FUN_NAME(QueueA, put)(q, (void *)&tmp);
	}

	fprintf(stdout, "gettiing \n");
	for (i = 0; i < QUEUEA_ITEM_LEN; i++)
	{
		struct aa *tmp = NULL;
		MODULE_FUN_NAME(QueueA, get)(q, (void **)&tmp);
		fprintf(stdout, "i: %d, \n", i);
		if (tmp != NULL)
			fprintf(stdout, "first get: value: %d, %s\n", tmp->a, tmp->b);
	}

	fprintf(stdout, "puttiing 2\n");
	for (i = 0; i < QUEUEA_ITEM_LEN; i++)
	{
		struct aa tmp; 
		tmp.a = i;
		strcpy(tmp.b, "world");
		fprintf(stdout, "i: %d, \n", i);
		MODULE_FUN_NAME(QueueA, put)(q, (void *)&tmp);
	}

	fprintf(stdout, "gettiing \n");
	while (MODULE_FUN_NAME(QueueA, isEmpty)(q) == 0)
	{
		struct aa *tmp = NULL;
		MODULE_FUN_NAME(QueueA, get)(q, (void **)&tmp);
		if (tmp != NULL)
		{
			fprintf(stdout, "second get: value: %d, %s\n", tmp->a, tmp->b);
		}
	}

	fprintf(stdout, "freeing \n");
	MODULE_FUN_NAME(QueueA, free)(&q);
}
#endif

#if 1
#include "stack_array.h"
static void test_stack_array(void)
{
#define STACKA_ITEM_LEN	10240000
	StackA_T s = NULL;
	int i;
	int *p = NULL;

	s = MODULE_FUN_NAME(StackA, new)(STACKA_ITEM_LEN + 1, sizeof(int));
	if (NULL == s)
		return ;

	for (i = 0; i < STACKA_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(StackA, push)(s, &i);
	}

	while (MODULE_FUN_NAME(StackA, isEmpty)(s) == 0)
	{
		MODULE_FUN_NAME(StackA, pop)(s, (void **)&p);
		fprintf(stdout, "value: %d\n", *p);
	}

	MODULE_FUN_NAME(StackA, free)(&s);
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
			
			for (int i = 0; i < TEST_INT_NUM; i++)
				fprintf(stdout, "i: %d\n", p[i]);
			
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
#define THREADPOOL_ITEM_LEN	10240000
	int i;
	int ret = 0;
	ThreadPool_T p = NULL;

	p = MODULE_FUN_NAME(ThreadPool, new)(10);
	ret = MODULE_FUN_NAME(ThreadPool, init)(p);
	fprintf(stdout, "%s: init: %d\n", __func__, ret);
	for (i = 0; i < THREADPOOL_ITEM_LEN; i++)
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
#define ARRAY_TEST_ITEMS	10240000
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
#define SET_ITEM_LEN	10240000
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
//		fprintf(stdout, "i: %d\n", (int)p);
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
		fprintf(stdout, "i: %d <--> value: %d\n", i, (int)(array[i]));
	
	}

	free(array);
	
	MODULE_FUN_NAME(Set, free)(&set);
	MODULE_FUN_NAME(Set, free)(&set2);
	MODULE_FUN_NAME(Set, free)(&set3);
	

}


#include "set-list.h"

static int set_list_cmp(void *priv, void *arg)
{
	int num1 = (int)priv;
	int num2 = (int)arg;

//	fprintf(stdout, "%s: %d,%d\n", __func__, num1, num2);
	if (num1 > num2) return 1;
	else if (num1 < num2) return -1;
	else return 0;
}

static int set_list_print(void *priv, void *arg)
{
	fprintf(stdout, "%d\n", (int)priv);
	return 0;
}

static void test_set_list(void)
{
#define SETL_ITEM_LEN	10240
	int ret = 0;
	void *priv = NULL;
	SetL_T set1 = NULL;
	SetL_T set2 = NULL;
	SetL_T set3 = NULL;
	int num = 0;
	int remove_count = 0;

	set1 = MODULE_FUN_NAME(SetL, new)(set_list_cmp);
	set2 = MODULE_FUN_NAME(SetL, new)(set_list_cmp);

	for (int i = 0; i < SETL_ITEM_LEN; i++)
	{
		num = random() % SETL_ITEM_LEN + 1;
//		ret = MODULE_FUN_NAME(SetL, add)(set1, (void *)(2 * i + 1));
		ret = MODULE_FUN_NAME(SetL, add)(set1, (void *)(num));
//		fprintf(stdout, "add %d to set1\n", num);
		if (ret != 0)
		{
//			fprintf(stdout, "set1 add: %d failed %d\n", 2 * i + 1, ret);
		}

		MODULE_FUN_NAME(SetL, add)(set2, (void *)(2 * i + 2));
		if (ret != 0)
		{
//			fprintf(stdout, "set1 add: %d failed %d\n", 2 * i + 2, ret);
		}
	}

	for (int i = 0; i < SETL_ITEM_LEN; i++)
	{
		priv = MODULE_FUN_NAME(SetL, get)(set1, i);
//		fprintf(stdout, "get: %d, %d\n", i, (int)priv);
	}
	priv = MODULE_FUN_NAME(SetL, get)(set1, SETL_ITEM_LEN);
	fprintf(stdout, "get: %d, %d\n", SETL_ITEM_LEN, (int)priv);

	for (int i = 0; i < SETL_ITEM_LEN; i++)
	{
		num = random() % SETL_ITEM_LEN + 1;
		if (MODULE_FUN_NAME(SetL, isMember)(set1, (void *)num))
		{
//			fprintf(stdout, "%d is member\n", num);
		}
		priv = MODULE_FUN_NAME(SetL, search)(set1, (void *)num);
//		fprintf(stdout, "search: %d, %d\n", num, (int)(priv));
		if (priv != NULL)
		{
//			fprintf(stdout, "deleting %d\n", (int)(priv));
			priv = MODULE_FUN_NAME(SetL, remove)(set1, priv);
			if (priv == NULL)
			{
//				fprintf(stdout, "remove failed\n");
			}
			else
			{
				remove_count++;
			}
		}
	}

	fprintf(stdout, "set1: %d, remove_count: %d\n", MODULE_FUN_NAME(SetL, count)(set1), remove_count);
	MODULE_FUN_NAME(SetL, map)(set1, set_list_print, NULL);

	for (int i = 0; i < SETL_ITEM_LEN; i++)
	{
		priv = MODULE_FUN_NAME(SetL, get)(set1, i);
		if (priv)
		{
//			fprintf(stdout, "get: %d success: %d\n", i, (int)(priv));
		}
		else
		{
//			fprintf(stdout, "get: %d failed: %d\n", i, (int)(priv));
		}
	}

	fprintf(stdout, "set1: %d, remove_count: %d\n", MODULE_FUN_NAME(SetL, count)(set1), remove_count);
	MODULE_FUN_NAME(SetL, map)(set1, set_list_print, NULL);
	fprintf(stdout, "set2: %d\n", MODULE_FUN_NAME(SetL, count)(set2));
	MODULE_FUN_NAME(SetL, map)(set2, set_list_print, NULL);

	fprintf(stdout, "set3: test\n");
	set3 = MODULE_FUN_NAME(SetL, union)(set1, set2);
	fprintf(stdout, "set3: %d\n", MODULE_FUN_NAME(SetL, count)(set3));

	set3 = MODULE_FUN_NAME(SetL, inter)(set1, set2);
	fprintf(stdout, "set3: %d\n", MODULE_FUN_NAME(SetL, count)(set3));

	set3 = MODULE_FUN_NAME(SetL, diff)(set1, set2);
	fprintf(stdout, "set3: %d\n", MODULE_FUN_NAME(SetL, count)(set3));

	set3 = MODULE_FUN_NAME(SetL, minus)(set1, set2);
	fprintf(stdout, "set3: %d\n", MODULE_FUN_NAME(SetL, count)(set3));


	MODULE_FUN_NAME(SetL, free)(&set1);
	MODULE_FUN_NAME(SetL, free)(&set2);
	MODULE_FUN_NAME(SetL, free)(&set3);
}

#include "table.h"

static int table_apply(const void *key, void **pvalue, void *priv)
{
	fprintf(stdout, "%d: %d\n", (int)key, (int)(*pvalue));
	return 0;
}

static void test_table(void)
{
#define TABLE_ITEM_LEN	10240000

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
//		MODULE_FUN_NAME(Table, map)(table, table_apply, NULL);
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

#include "seq.h"

static void test_seq(void)
{
#define SEQ_ITEM_LEN	10240000
	Seq_T seq = NULL;
	Seq_T seq2 = NULL;
	int len = 0;
	void *prev = NULL;

	fprintf(stdout, "first\n");
	seq = MODULE_FUN_NAME(Seq, seq)("c", "c++", "html", "java", NULL);
	seq2 = MODULE_FUN_NAME(Seq, new)(10);

	len = MODULE_FUN_NAME(Seq, length)(seq);
	fprintf(stdout, "len: %d\n", len);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Seq, get)(seq, i));
	}

	fprintf(stdout, "second\n");
	prev = MODULE_FUN_NAME(Seq, put)(seq, 3, "shell");
	len = MODULE_FUN_NAME(Seq, length)(seq);
	fprintf(stdout, "len: %d, prev: %s\n", len, (char *)prev);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Seq, get)(seq, i));
	}

	fprintf(stdout, "third\n");
	MODULE_FUN_NAME(Seq, addhi)(seq, "java");
	MODULE_FUN_NAME(Seq, addlo)(seq, "icon");
	len = MODULE_FUN_NAME(Seq, length)(seq);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Seq, get)(seq, i));
	}

	fprintf(stdout, "four\n");
	MODULE_FUN_NAME(Seq, rmlo)(seq);
	MODULE_FUN_NAME(Seq, rmhi)(seq);
	len = MODULE_FUN_NAME(Seq, length)(seq);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Seq, get)(seq, i));
	}

	fprintf(stdout, "five\n");
	for (int i = 0; i < SEQ_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(Seq, addhi)(seq2, (void *)i);
		MODULE_FUN_NAME(Seq, addlo)(seq2, (void *)i);
	}
	len = MODULE_FUN_NAME(Seq, length)(seq2);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %d\n", i, (int)MODULE_FUN_NAME(Seq, get)(seq2, i));
	}

	MODULE_FUN_NAME(Seq, free)(&seq);
	MODULE_FUN_NAME(Seq, free)(&seq2);
}


#include "ring.h"

static void test_ring(void)
{
#define RING_ITEM_LEN	1024000
	Ring_T ring = NULL;
	int len = 0;


#if 0
	fprintf(stdout, "first\n");
	ring = MODULE_FUN_NAME(Ring, ring)("hello", "world", "c", "language");
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, len: %d, %s\n", i, len, (char *)(MODULE_FUN_NAME(Ring, get)(ring, i)));
	}

	fprintf(stdout, "second\n");
	MODULE_FUN_NAME(Ring, put)(ring, 1, "ni");
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}

	fprintf(stdout, "third\n");
	MODULE_FUN_NAME(Ring, addhi)(ring, "na");
	MODULE_FUN_NAME(Ring, addlo)(ring, "ha");
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}

	fprintf(stdout, "third\n");
	MODULE_FUN_NAME(Ring, rmhi)(ring);
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}
	MODULE_FUN_NAME(Ring, rmlo)(ring);
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}

	fprintf(stdout, "four\n");
	MODULE_FUN_NAME(Ring, add)(ring, 3, "yahada");
	MODULE_FUN_NAME(Ring, add)(ring, -5, "wone");
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}

	fprintf(stdout, "five\n");
	MODULE_FUN_NAME(Ring, remove)(ring, 3);
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}

	fprintf(stdout, "six\n");
	MODULE_FUN_NAME(Ring, rotate)(ring, 3);
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %s\n", i, (char *)MODULE_FUN_NAME(Ring, get)(ring, i));
	}

	fprintf(stdout, "seven\n");
	MODULE_FUN_NAME(Ring, rotate)(ring, -3);
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
//		fprintf(stdout, "i: %d, %s\n", i, MODULE_FUN_NAME(Ring, get)(ring, i));
		MODULE_FUN_NAME(Ring, rmhi)(ring);
	}
#endif

	
	
	ring = MODULE_FUN_NAME(Ring, new)();
	for (int i = 0; i < RING_ITEM_LEN; i++)	
	{
		MODULE_FUN_NAME(Ring, addhi)(ring, (void *)(i + 1));
	}
	len = MODULE_FUN_NAME(Ring, length)(ring);
	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "i: %d, %d\n", i, (int)MODULE_FUN_NAME(Ring, get)(ring, i));
	}
	
	

	MODULE_FUN_NAME(Ring, free)(&ring);
}

#include "str.h"
static void test_str(void)
{
	char *p = NULL;

	for (int i = 0; i < strlen("hello world"); i++)
	{
		p = MODULE_FUN_NAME(Str, sub)("hello world", i, strlen("hello world") + 1);
		fprintf(stdout, "len: %d, %s\n", MODULE_FUN_NAME(Str, len)(p, 1, strlen(p)), p);
	}
}


#include "xp.h"

static void test_xp(void)
{
#define XP_TEST_NUM	102400
#define XP_LEN	16
	int ret = 0;
	int len = 0;
	XP_T xp = NULL;
	unsigned char xp_str[XP_LEN] = { 0 };
	unsigned char str[XP_LEN * 3] = { 0 };
	XP_T xp2 = NULL;
	unsigned char xp2_str[XP_LEN] = { 0 };
	unsigned char str2[XP_LEN * 3] = { 0 };
	XP_T sum = NULL;
	unsigned char sum_str[XP_LEN] = { 0 };
	XP_T z = NULL;
	unsigned char z_str[XP_LEN * 2] = { 0 };
	XP_T q = NULL;
	unsigned char q_str[XP_LEN] = { 0 };
	XP_T r = NULL;
	unsigned char r_str[XP_LEN] = { 0 };
	XP_T tmp = NULL;
	unsigned char tmp_str[XP_LEN * 2 + 2] = { 0 };

	xp = xp_str;
	xp2 = xp2_str;
	sum = sum_str;
	z = z_str;

	q = q_str;
	r = r_str;
	tmp = tmp_str;

	fprintf(stdout, "first\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, xp) != i)
			fprintf(stdout, "i: %d, toint: %lu\n", i, MODULE_FUN_NAME(XP, toint)(XP_LEN, xp));
	}

	fprintf(stdout, "second add\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, add)(XP_LEN, sum, xp, xp2, 0);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i + i + 1)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "third sub\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, sub)(XP_LEN, sum, xp2, xp, 0);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != 1)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "four mul\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(z, 0, XP_LEN * 2);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, mul)(z, XP_LEN, xp2, XP_LEN, xp);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN * 2, z) != ((unsigned long)i) * (unsigned long)(i + 1))
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN * 2, z));
	}

	fprintf(stdout, "four div\n");
	MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, 9);
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(z, 0, XP_LEN * 2);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i);
		MODULE_FUN_NAME(XP, div)(XP_LEN, q, xp2, XP_LEN, xp, r, tmp);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN , q) != (i) / (9) 
			|| MODULE_FUN_NAME(XP, toint)(XP_LEN, r) != i % 9)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, q),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, r));
	}
	
	fprintf(stdout, "five sum\n");
	for (int j = 1; j < 256; j++)
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, sum)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i + j)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "six diff\n");
	for (int j = 1; j < 256; j++)
	{
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i + j);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, diff)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}
	}

	fprintf(stdout, "seven product\n");
	for (int j = 1; j < 256; j++)
	{
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, product)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != (unsigned long)i * (unsigned long)j)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}
	}

	fprintf(stdout, "eight quotient\n");
	for (int j = 1; j < 256; j++)
	{
	int ret = 0;
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		ret = MODULE_FUN_NAME(XP, quotient)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i / j 
			|| ret != i % j)
			fprintf(stdout, "i: %d, j: %d, toint: %lu, %lu\n", i, j,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}
	}

	fprintf(stdout, "nine cmp\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i);
		if (MODULE_FUN_NAME(XP, cmp)(XP_LEN, xp, xp2) != 0)
			fprintf(stdout, "i: %d, toint: %lu, %lu\n", i,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2));
	}

	fprintf(stdout, "ten neg\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, neg)(XP_LEN, xp2, xp, 0);
		MODULE_FUN_NAME(XP, neg)(XP_LEN, sum, xp2, 0);
		if (MODULE_FUN_NAME(XP, cmp)(XP_LEN, xp, sum) != 0)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "eleven lshift\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, lshift)(XP_LEN, xp2, XP_LEN, xp, 2, 0);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2) != i << 2)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "twelve rshift\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(xp, 0, XP_LEN);
		memset(xp2, 0, XP_LEN);
		memset(str, 0, XP_LEN * 2);
		memset(str2, 0, XP_LEN * 2);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, rshift)(XP_LEN, xp2, XP_LEN, xp, 9, 1);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2) != i >> 9)
			fprintf(stdout, "i: %d, tostr: %s, %s, \n", i,
							MODULE_FUN_NAME(XP, tostr)(str, XP_LEN * 3, 16, XP_LEN, xp),
							MODULE_FUN_NAME(XP, tostr)(str2, XP_LEN * 3, 16, XP_LEN, xp2));
	}

}

static void test_ap2(void)
{
#define XP_TEST_NUM	1024000
#define XP_LEN	16
	int ret = 0;
	int len = 0;
	XP_T xp = NULL;
	unsigned char xp_str[XP_LEN] = { 0 };
	unsigned char str[XP_LEN * 3] = { 0 };
	XP_T xp2 = NULL;
	unsigned char xp2_str[XP_LEN] = { 0 };
	unsigned char str2[XP_LEN * 3] = { 0 };
	XP_T sum = NULL;
	unsigned char sum_str[XP_LEN] = { 0 };
	XP_T z = NULL;
	unsigned char z_str[XP_LEN * 2] = { 0 };
	XP_T q = NULL;
	unsigned char q_str[XP_LEN] = { 0 };
	XP_T r = NULL;
	unsigned char r_str[XP_LEN] = { 0 };
	XP_T tmp = NULL;
	unsigned char tmp_str[XP_LEN * 2 + 2] = { 0 };

	xp = xp_str;
	xp2 = xp2_str;
	sum = sum_str;
	z = z_str;

	q = q_str;
	r = r_str;
	tmp = tmp_str;

	fprintf(stdout, "first\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, xp) != i)
			fprintf(stdout, "i: %d, toint: %lu\n", i, MODULE_FUN_NAME(XP, toint)(XP_LEN, xp));
	}

	fprintf(stdout, "second add\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, add)(XP_LEN, sum, xp, xp2, 0);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i + i + 1)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "third sub\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, sub)(XP_LEN, sum, xp2, xp, 0);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != 1)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "four mul\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(z, 0, XP_LEN * 2);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, mul)(z, XP_LEN, xp2, XP_LEN, xp);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN * 2, z) != ((unsigned long)i) * (unsigned long)(i + 1))
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN * 2, z));
	}

	fprintf(stdout, "four div\n");
	MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, 9);
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(z, 0, XP_LEN * 2);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i);
		MODULE_FUN_NAME(XP, div)(XP_LEN, q, xp2, XP_LEN, xp, r, tmp);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN , q) != (i) / (9) 
			|| MODULE_FUN_NAME(XP, toint)(XP_LEN, r) != i % 9)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, q),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, r));
	}
	
	fprintf(stdout, "five sum\n");
	for (int j = 1; j < 256; j++)
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, sum)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i + j)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "six diff\n");
	for (int j = 1; j < 256; j++)
	{
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i + j);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, diff)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}
	}

	fprintf(stdout, "seven product\n");
	for (int j = 1; j < 256; j++)
	{
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i + 1);
		MODULE_FUN_NAME(XP, product)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != (unsigned long)i * (unsigned long)j)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i, 
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}
	}

	fprintf(stdout, "eight quotient\n");
	for (int j = 1; j < 256; j++)
	{
	int ret = 0;
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		ret = MODULE_FUN_NAME(XP, quotient)(XP_LEN, sum, xp, j);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, sum) != i / j 
			|| ret != i % j)
			fprintf(stdout, "i: %d, j: %d, toint: %lu, %lu\n", i, j,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}
	}

	fprintf(stdout, "nine cmp\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp2, i);
		if (MODULE_FUN_NAME(XP, cmp)(XP_LEN, xp, xp2) != 0)
			fprintf(stdout, "i: %d, toint: %lu, %lu\n", i,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2));
	}

	fprintf(stdout, "ten neg\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, neg)(XP_LEN, xp2, xp, 0);
		MODULE_FUN_NAME(XP, neg)(XP_LEN, sum, xp2, 0);
		if (MODULE_FUN_NAME(XP, cmp)(XP_LEN, xp, sum) != 0)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "eleven lshift\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(sum, 0, XP_LEN);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, lshift)(XP_LEN, xp2, XP_LEN, xp, 2, 0);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2) != i << 2)
			fprintf(stdout, "i: %d, toint: %lu, %lu, %lu\n", i,
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2),
							MODULE_FUN_NAME(XP, toint)(XP_LEN, sum));
	}

	fprintf(stdout, "twelve rshift\n");
	for (int i = 0; i < XP_TEST_NUM; i++)
	{
		memset(xp, 0, XP_LEN);
		memset(xp2, 0, XP_LEN);
		memset(str, 0, XP_LEN * 2);
		memset(str2, 0, XP_LEN * 2);
		MODULE_FUN_NAME(XP, fromint)(XP_LEN, xp, i);
		MODULE_FUN_NAME(XP, rshift)(XP_LEN, xp2, XP_LEN, xp, 9, 1);
		if (MODULE_FUN_NAME(XP, toint)(XP_LEN, xp2) != i >> 9)
			fprintf(stdout, "i: %d, tostr: %s, %s, \n", i,
							MODULE_FUN_NAME(XP, tostr)(str, XP_LEN * 3, 16, XP_LEN, xp),
							MODULE_FUN_NAME(XP, tostr)(str2, XP_LEN * 3, 16, XP_LEN, xp2));
	}

}

#include "rb_tree.h"

static int rb_cmp(void *arg1, void *arg2)
{
	unsigned long num1 = (unsigned long )arg1;
	unsigned long num2 = (unsigned long )arg2;

	if (num1 > num2)
	{
		return 1;
	}
	else if (num1 == num2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int rb_print(void *priv, void *arg)
{
	RB_Node node = (RB_Node)priv;
//	fprintf(stdout, "node: %p, %d, parent: %p, left: %p, right: %p, color: %s\n", 
//					node, (int)node->priv, node->parent, node->left, node->right, node->color == RED? "red" : "black");
//	fprintf(stdout, "%d\n", (int)priv);
	return 0;
}

static void test_rb(void)
{
#define RB_ITEM_LEN	102400000
	RB_Tree tree = NULL;
	RB_Node node = NULL;

	tree = MODULE_FUN_NAME(RB_Tree, new)(rb_cmp);

	for (int i = 0; i < RB_ITEM_LEN; i++)
	{
		node = (RB_Node)calloc(1, sizeof(*node));
		node->priv = (void *)((random()) % RB_ITEM_LEN + 1);
		MODULE_FUN_NAME(RB_Tree, insert)(tree, node);
//		fprintf(stdout, "insert i: %d, %d...\n", i, (int)(node->priv));
//		MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, tree->root, rb_print, NULL);
	}

	fprintf(stdout, "after insert all\n");
	MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, tree->root, rb_print, NULL);
	for (int i = 0; i < RB_ITEM_LEN; i++)
	{
		node = MODULE_FUN_NAME(RB_Tree, search)(tree, (void *)(random() % RB_ITEM_LEN + 1));
		if (node)
		{
//		fprintf(stdout, "delete i: %d, value: %d\n", i, (int)(node->priv));
		MODULE_FUN_NAME(RB_Tree, delete)(tree, node);
		MODULE_FUN_NAME(RB_Tree, minimum)(tree);
		MODULE_FUN_NAME(RB_Tree, maximum)(tree);
		}
//	MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, tree->root, rb_print, NULL);
	}
//	MODULE_FUN_NAME(RB_Tree, inorder_walk)(tree, tree->root, rb_print, NULL);

//	MODULE_FUN_NAME(RB_Tree, free)(&tree);
}

#include "fib-heap.h"

static int fib_cmp(void *arg1, void *arg2)
{
	unsigned long num1 = (unsigned long )arg1;
	unsigned long num2 = (unsigned long )arg2;
//	fprintf(stdout, "cmp: %d, %d, %p, %p\n", num1, num2, arg1, arg2);

	if (num1 > num2)
	{
		return 1;
	}
	else if (num1 < num2)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

static void test_fib(void)
{
#define FIB_ITEM_LEN	1024000
#define FIB_INFINITE	0
	FibHeap_T h1 = NULL;
	FibHeap_T h2 = NULL;
	FibHeap_T h3 = NULL;
	FibHeap_Node node = NULL;

	h1 = MODULE_FUN_NAME(FibHeap, new)(fib_cmp, FIB_INFINITE);

	
	fprintf(stdout, "first\n");
	for (int i = 0; i < FIB_ITEM_LEN; i++)
	{
		node = MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(i + 1));
		if (node == NULL)
		{
			fprintf(stdout, "error no mem:\n");
			return ;
		}
		MODULE_FUN_NAME(FibHeap, insert)(h1, node);
	}

	for (int i = 0; i < FIB_ITEM_LEN; i++)
	{
		node = MODULE_FUN_NAME(FibHeap, extractMin)(h1);
		fprintf(stdout, "i: %d, value: %d\n", i, (int)(MODULE_FUN_NAME(FibHeap, NodePriv)(node)));
		MODULE_FUN_NAME(FibHeap, NodeFree)(&node);
	}
	

	fprintf(stdout, "second\n");
	h2 = MODULE_FUN_NAME(FibHeap, new)(fib_cmp, FIB_INFINITE);
	
	for (int i = 0; i < FIB_ITEM_LEN; i++)
	{
		node = MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(i + 1));
		if (node == NULL)
		{
			fprintf(stdout, "error no mem:\n");
			return ;
		}
		MODULE_FUN_NAME(FibHeap, insert)(h2, node);
	}

	for (int i = 0; i < FIB_ITEM_LEN; i++)
	{
		node = MODULE_FUN_NAME(FibHeap, minmum)(h2);
		fprintf(stdout, "i: %d, value: %d\n", i, (int)(MODULE_FUN_NAME(FibHeap, NodePriv)(node)));
		MODULE_FUN_NAME(FibHeap, delete)(h2, node);
		MODULE_FUN_NAME(FibHeap, NodeFree)(&node);
	}
	

	fprintf(stdout, "third\n");
	for (int i = 0; i < FIB_ITEM_LEN; i += 2)
	{
		node = MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(i + 1));
		MODULE_FUN_NAME(FibHeap, insert)(h1, node);
		node = MODULE_FUN_NAME(FibHeap, NodeCreate)((void *)(i + 2));
		MODULE_FUN_NAME(FibHeap, insert)(h2, node);
	}
	/*
	fprintf(stdout, "h1:\n");
	MODULE_FUN_NAME(FibHeap, print)(h1);
	fprintf(stdout, "h2:\n");
	MODULE_FUN_NAME(FibHeap, print)(h2);
	*/

	h3 = MODULE_FUN_NAME(FibHeap, union)(&h1, &h2);
	fprintf(stdout, "h3:\n");
	MODULE_FUN_NAME(FibHeap, print)(h3);
	for (int i = 0; i < FIB_ITEM_LEN; i++)
	{
	//	node = MODULE_FUN_NAME(FibHeap, minmum)(h3);
		node = MODULE_FUN_NAME(FibHeap, extractMin)(h3);
		fprintf(stdout, "i: %d, value: %d\n", i, (int)(MODULE_FUN_NAME(FibHeap, NodePriv)(node)));
//	MODULE_FUN_NAME(FibHeap, print)(h3);
//		MODULE_FUN_NAME(FibHeap, delete)(h3, node);
		MODULE_FUN_NAME(FibHeap, NodeFree)(&node);
	}
	MODULE_FUN_NAME(FibHeap, print)(h3);

//	MODULE_FUN_NAME(FibHeap, free)(&h1);

}

#include "os_rank.h"

static void test_os_rank(void)
{
#define OS_RANK_ITEM_LEN	1024
}

#include "interval_tree.h"

static int myprint(void *priv, void *arg)
{
	In_Node node = (In_Node)priv;
	int flag = 0;

	if (node->in.high > node->left->max 
		&& node->in.high > node->right->max)
	{
		if (node->max != node->in.high)
		{
			flag = 1;
		}
	}
	if (node->left->max > node->in.high
		&& node->left->max > node->right->max)
	{
		if (node->max != node->left->max)
		{
			flag = 1;
		}
	}
	if (node->right->max > node->left->max 
		&& node->right->max > node->in.high)
	{
		if (node->max != node->right->max)
		{
			flag = 1;
		}
	}

	if (flag)
	{
		fprintf(stdout, "error node: %p, low: %d, high: %d, max: %d, parent: %p, left: %p, left_max: %d, right: %p, right_max: %d, color: %s\n", 
			node, node->in.low, node->in.high, node->max, node->parent, node->left, node->left->max, node->right, node->right->max, node->color == RED ? "red" : "black");
		exit(0);
	}
			

	/*
	fprintf(stdout, "node: %p, low: %d, high: %d, max: %d, parent: %p, left: %p, right: %p, color: %s\n", 
					node, node->in.low, node->in.high, node->max, node->parent, node->left, node->right, node->color == RED ? "red" : "black");
					*/
					
	return 0;
}

static void test_interval_tree(void)
{
#define INTERVAL_TREE_ITEM	10240
	In_Tree tree = NULL;
	In_Node node = NULL;
	int low = 0;
	int high = 0;
	int tmp = 0;
	struct interval in;

	tree = MODULE_FUN_NAME(InTree, new)();
	fprintf(stdout, "root: %p, nil: %p\n", tree->root, tree->nil);
	for (int i = 0; i < INTERVAL_TREE_ITEM; i++)
	{
		node = (In_Node)calloc(1, sizeof(*node));
		if (node == NULL)
		{
			fprintf(stderr, "i: %d no mem exit\n", i);
			return 0;
		}
		low = random() % INTERVAL_TREE_ITEM + 1;
		high = random() % INTERVAL_TREE_ITEM + 1;
		if (low > high)
		{
			tmp = low;
			low = high;
			high = tmp;
		}
		node->in.low = low;
		node->in.high = high;
//		fprintf(stdout, "insert: i: %d, node: %p, %d, %d\n", i, node, node->in.low, node->in.high);
		MODULE_FUN_NAME(InTree, insert)(tree, node);
//		MODULE_FUN_NAME(InTree, inorder_walk)(tree, tree->root, myprint, NULL);
	}

	MODULE_FUN_NAME(InTree, inorder_walk)(tree, tree->root, myprint, NULL);
	fprintf(stdout, "second\n");
	for (int i = 0; i < INTERVAL_TREE_ITEM; i++)
	{
		low = random() % INTERVAL_TREE_ITEM + 1;
		high = random() % INTERVAL_TREE_ITEM + 1;
		if (low > high)
		{
			tmp = low;
			low = high;
			high = tmp;
		}
		in.low = low;
		in.high = high;
		node = MODULE_FUN_NAME(InTree, search)(tree, (void *)&in);
		if (node)
		{
				
				/*
			fprintf(stdout, "deleting low: %d, high: %d, node: %p, left: %p, right: %p\n", 
							node->in.low, node->in.high, node, node->left, node->right);
							*/
							
			MODULE_FUN_NAME(InTree, delete)(tree, node);
			MODULE_FUN_NAME(InTree, inorder_walk)(tree, tree->root, myprint, NULL);	// 每删除一个元素跑一次是为了验证区间树维护正常
		}
	}
	fprintf(stdout, "third\n");
	MODULE_FUN_NAME(InTree, inorder_walk)(tree, tree->root, myprint, NULL);
	MODULE_FUN_NAME(InTree, free)(&tree);
}



#include "b-tree.h"

static int inorder_walk(void *priv, void *arg)
{
	int num = (int)priv;

	fprintf(stdout, "%d\n", num);
	return 0;
}

static int binary_cmp(void *priv, void *arg)
{
	int num1 = (int)priv;
	int num2 = (int)arg;

	if (num1 > num2) return 1;
	else if (num1 < num2) return -1;
	else return 0;
}

static void test_binary_tree(void)
{
#define BINARY_ITEM_LEN	10240000
	BTree_T tree = NULL;
	BNode_T node = NULL;
	int priv = 0;

	tree = MODULE_FUN_NAME(BTree, new)(binary_cmp);

	for (int i = 0; i < BINARY_ITEM_LEN; i++)
	{
		priv = random() % BINARY_ITEM_LEN + 1;
		node = MODULE_FUN_NAME(BTree, NodeNew)((void *)priv);
		if (node)
		{
//			fprintf(stdout, "i: %d insert: %d\n", i, priv);
			MODULE_FUN_NAME(BTree, insert)(tree, node);
		}
		else
		{
			fprintf(stdout, "i: %d, no mem\n", i);
		}
	}

	MODULE_FUN_NAME(BTree, inorder_walk)(tree, inorder_walk, NULL);

	node = MODULE_FUN_NAME(BTree, minimum)(tree, NULL);
	fprintf(stdout, "minimum: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));
	node = MODULE_FUN_NAME(BTree, successor)(tree, node);
	if (node)
		fprintf(stdout, "successor: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));
	node = MODULE_FUN_NAME(BTree, predecessor)(tree, node);
	if (node)
		fprintf(stdout, "predecessor: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));

	node = MODULE_FUN_NAME(BTree, maximum)(tree, NULL);
	fprintf(stdout, "maximum: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));
	node = MODULE_FUN_NAME(BTree, predecessor)(tree, node);
	if (node)
		fprintf(stdout, "predecessor: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));
	node = MODULE_FUN_NAME(BTree, successor)(tree, node);
	if (node)
		fprintf(stdout, "successor: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));

	int count = 0;

	for (int i = 0; i < BINARY_ITEM_LEN; i++)
	{
		priv = random() % BINARY_ITEM_LEN + 1;
		node = MODULE_FUN_NAME(BTree, search)(tree, (void *)priv);
		if (node)
		{
			count++;
			fprintf(stdout, "i: %d, delete node: %d, %d\n", i, priv, (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));
			MODULE_FUN_NAME(BTree, delete)(tree, node);
			MODULE_FUN_NAME(BTree, NodeFree)(&node);
		}
		else
		{
			fprintf(stdout, "i: %d, no found: %d\n", i, priv);
		}
	}

	fprintf(stdout, "count: %d\n", count);

	while ((node = MODULE_FUN_NAME(BTree, minimum)(tree, NULL)) != NULL)
	{
		count++;
		fprintf(stdout, "deleting: %d\n", (int)MODULE_FUN_NAME(BTree, NodeGetPriv)(node));
		MODULE_FUN_NAME(BTree, delete)(tree, node);
		MODULE_FUN_NAME(BTree, NodeFree)(&node);
	}
	fprintf(stdout, "count: %d\n", count);

	MODULE_FUN_NAME(BTree, free)(&tree);
}

#include "list.h"

static void list_map(void **arg, void *priv)
{
	void *num = *arg;
	fprintf(stdout, "map: %d\n", (int)num);
}

static void test_list(void)
{
#define LIST_ITEM_LEN	102400000

	List_T list = NULL;
	List_T list2 = NULL;
	int num = 0;
	int len = 0;
	int *value = NULL;
	void **array = NULL;

	list = (List_T) calloc(1, sizeof(*list));
	list->first = NULL;
	list->rest = NULL;

//	list = MODULE_FUN_NAME(List, list)(&num, 1, 2);
	for (int i = 0; i < LIST_ITEM_LEN; i++)
	{
		list = MODULE_FUN_NAME(List, push)(list, (void *)(i + 1));
	}

	len = MODULE_FUN_NAME(List, length)(list);
	fprintf(stdout, "list len: %d\n", len);

	fprintf(stdout, "copy...\n");
	list2 = MODULE_FUN_NAME(List, copy)(list);
//	MODULE_FUN_NAME(List, map)(list2, list_map, NULL);

	fprintf(stdout, "reverse...\n");
	list = MODULE_FUN_NAME(List, reverse)(list);
//	MODULE_FUN_NAME(List, map)(list, list_map, NULL);

	fprintf(stdout, "pop...\n");
	for (int i = 0; i < len; i++)
	{
		list = MODULE_FUN_NAME(List, pop)(list, (void **)&value);
//		fprintf(stdout, "i: %d, value: %d, list: %p\n", i, (int)value, list);
	}

	fprintf(stdout, "toarray...\n");
	list = MODULE_FUN_NAME(List, append)(list2, list);
	array = MODULE_FUN_NAME(List, toArray)(list, (void *)(LIST_ITEM_LEN + 1));
	for (int i = 0; array[i] != (void *)(LIST_ITEM_LEN + 1); i++)
	{
//		fprintf(stdout, "i: %d, %d\n", i, (int)(array[i]));
	}

	MODULE_FUN_NAME(List, free)(&list);
}

#include "queue.h"

static void test_queue(void)
{
#define QUEUE_ITEM_LEN	10240000
	Queue_T q = NULL;
	int ret = 0;
	int *value = NULL;

	q = MODULE_FUN_NAME(Queue, new)(QUEUE_ITEM_LEN + 1);

	for (int i = 0; i <	QUEUE_ITEM_LEN; i++)
	{
		ret = MODULE_FUN_NAME(Queue, en)(q, (void *)i);
		if (ret != 0)
		{
			fprintf(stdout, "put i: %d to queue failed\n", i);
		}
	}
	for (int i = 0; i < QUEUE_ITEM_LEN; i++)
	{
		ret = MODULE_FUN_NAME(Queue, de)(q, (void **)&value);
		if (ret != 0)
		{
			fprintf(stdout, "get i: %d from queue failed\n", i);
		}
		if ((int)value != i)
		{
			fprintf(stdout, "i: %d, value: %d\n", i, (int)value);
		}
	}

	MODULE_FUN_NAME(Queue, free)(&q);
}

#include "stack.h"

static void test_stack(void)
{
#define STACK_ITEM_LEN	102400000
	Stack_T s = NULL;
	int ret = 0;
	int *value = NULL;

	s = MODULE_FUN_NAME(Stack, new)(STACK_ITEM_LEN + 1);

	for (int i = 0; i < STACK_ITEM_LEN; i++)
	{
		ret = MODULE_FUN_NAME(Stack, push)(s, (void *)i);
		if (ret != 0)
		{
			fprintf(stdout, "push i: %d error\n", i);
		}
	}

	for (int i = 0; i < STACK_ITEM_LEN; i++)
	{
		ret = MODULE_FUN_NAME(Stack, pop)(s, (void **)&value);
		if (ret != 0)
		{
			fprintf(stdout, "pop i: %d, error\n", i);
		}
		if ((int)value != STACK_ITEM_LEN - i - 1) 
		{
			fprintf(stdout, "pop i: %d, error, value: %d\n", i, (int)value);
		}
	}

	MODULE_FUN_NAME(Stack, free)(&s);
}

#include "gve.h"

static int graph_node_cmp(void *arg, void *priv)
{
	int num1 = (int )arg;
	int num2 = (int )priv;

	if (num1 == num2) return 0;
	else return -1;
}

static void test_gve(void)
{
#define GRAPH_EDGE_LEN	102400 * 1
#define GRAPH_NODE_LEN	102400 * 2

	Graph_T g = NULL;
	Edge_T e = NULL;
	Edge_T e_priv = NULL;
	VNode_T v = NULL;
	VNode_T u = NULL;
	VNode_T prev = NULL;
	int ret = 0;

	g = MODULE_FUN_NAME(Graph, GCreate)(GRAPH_NODE_LEN, GRAPH_EDGE_LEN, NULL);

	/*
	fprintf(stdout, "first\n");
	for (int i = 0; i < GRAPH_NODE_LEN; i++)
	{
		v = MODULE_FUN_NAME(Graph, VnodeCreate)((void *)i);
		ret = MODULE_FUN_NAME(Graph, VnodeAdd)(g, v);
		if (ret != 0)
		{
			fprintf(stdout, "add node: %d failed\n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
		}
	}

	for (int i = GRAPH_NODE_LEN - 1; i >= 0; i--)
	{
		v = MODULE_FUN_NAME(Graph, VnodeSearch)(g, graph_node_cmp, (void *)i);
		ret = MODULE_FUN_NAME(Graph, VnodeRemove)(g, v);
		if (ret != 0)
		{
			fprintf(stdout, "remove node: %d failed\n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
		}
		else
		{
			MODULE_FUN_NAME(Graph, VnodeFree)(&v);
		}
	}
	*/

	fprintf(stdout, "second\n");
	for (int i = 0; i < GRAPH_EDGE_LEN; i++)
	{
		v = MODULE_FUN_NAME(Graph, VnodeCreate)((void *)(2 * i));
		u = MODULE_FUN_NAME(Graph, VnodeCreate)((void *)(2 * i + 1));
		e = MODULE_FUN_NAME(Graph, EdgeCreate)(v, u, (void *)i);
		ret = MODULE_FUN_NAME(Graph, VnodeAdd)(g, v);
		if (ret != 0)
		{
			fprintf(stdout, "add node: %d failed\n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
		}

		ret = MODULE_FUN_NAME(Graph, VnodeAdd)(g, u);
		if (ret != 0)
		{
			fprintf(stdout, "add node: %d failed\n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(u));
		}

		ret = MODULE_FUN_NAME(Graph, EdgeAdd)(g, e);
		if (ret != 0)
		{
			fprintf(stdout, "add edge: %d %p failed\n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e), e);
		}
		else
		{
//			fprintf(stdout, "add edge: %d %p \n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e), e);
		}
	}

	int len = 0;
	len = MODULE_FUN_NAME(Graph, EdgesLength)(g);
	fprintf(stdout, "edges len: %d\n", len);
	for (int i = 0; i < len; i++)
	{
		e = (Edge_T)MODULE_FUN_NAME(Graph, EdgeGet)(g, i);
		if (e == NULL) continue;
		fprintf(stdout, "edge: %d %p \n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e), e);
		MODULE_FUN_NAME(Graph, EdgeRemove)(g, e);
		MODULE_FUN_NAME(Graph, EdgeFree)(&e);
	}

	len = MODULE_FUN_NAME(Graph, VnodesLength)(g);
	fprintf(stdout, "nodes len: %d\n", len);
	for (int i = 0; i < len; i++)
	{
		v = (VNode_T)MODULE_FUN_NAME(Graph, VnodeGet)(g, i);
		if (v == NULL) continue;
		fprintf(stdout, "vnode: %d %p \n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v), v);
		MODULE_FUN_NAME(Graph, VnodeRemove)(g, v);
		MODULE_FUN_NAME(Graph, VnodeFree)(&v);
	}

#if 0
	for (e = MODULE_FUN_NAME(Graph, FirstEdge)(g); e != NULL; e = MODULE_FUN_NAME(Graph, NextEdge)(g, e))
	{
		fprintf(stdout, "edge: %d \n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e));
		if (MODULE_FUN_NAME(Graph, IsLastEdge)(g, e))
		{
			fprintf(stdout, "break: %d \n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e));
			break;
		}
	}

	for (e = MODULE_FUN_NAME(Graph, FirstEdge)(g); e != NULL; e = MODULE_FUN_NAME(Graph, FirstEdge)(g))
	{
		fprintf(stdout, "remove edge: %d \n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e));
		ret = MODULE_FUN_NAME(Graph, EdgeRemove)(g, e);
		if (ret != 0)
		{
			fprintf(stdout, "remove edge: %d failed\n", (int)MODULE_FUN_NAME(Graph, EdgeGetPriv)(e));
		}
	}

	for (v = MODULE_FUN_NAME(Graph, FirstVnode)(g); v != NULL; v = MODULE_FUN_NAME(Graph, NextVnode)(g, v))
	{
		fprintf(stdout, "node: %d \n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
		if (MODULE_FUN_NAME(Graph, IsLastVnode)(g, v))
		{
			fprintf(stdout, "break: %d \n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
			break;
		}
	}

	for (v = MODULE_FUN_NAME(Graph, FirstVnode)(g); v != NULL; v = MODULE_FUN_NAME(Graph, FirstVnode)(g))
	{
		fprintf(stdout, "remove node: %d \n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
		ret = MODULE_FUN_NAME(Graph, VnodeRemove)(g, v);
		if (ret != 0)
		{
			fprintf(stdout, "remove node: %d failed\n", (int)MODULE_FUN_NAME(Graph, VnodeGetPriv)(v));
		}
	}
#endif

	MODULE_FUN_NAME(Graph, GFree)(&g);
}


#include "gve_array.h"

static int graphA_node_cmp(void *arg, void *priv)
{
	int num1 = (int )arg;
	int num2 = (int )priv;

	if (num1 == num2) return 0;
	else return -1;
}

static void test_gve_array(void)
{
#define GVE_A_NODE_LEN	10240000 * 2
#define GVE_A_EDGE_LEN	10240000
	struct node_ext {
		int index;
		int value;
	};
	struct edge_ext {
		int value;
		int d;
		int f;
	};

#define GVE_A_NODE_SIZE	sizeof(struct node_ext)
#define GVE_A_EDGE_SIZE	sizeof(struct edge_ext)

	GraphA_T g = NULL;
	int len = 0;
	struct node_ext *node_p = NULL;
	struct node_ext *node_v = NULL;
	struct node_ext *node_u = NULL;
	struct node_ext node;
	EdgeA_T edge = NULL;
	struct edge_ext *edge_ext_p = NULL;

	fprintf(stdout, "node size: %d, len: %d, edge size: %d, len: %d\n", GVE_A_NODE_SIZE, GVE_A_NODE_LEN, GVE_A_EDGE_SIZE, GVE_A_EDGE_LEN);

	g = MODULE_FUN_NAME(GraphA, create)(GVE_A_NODE_SIZE, GVE_A_NODE_LEN, GVE_A_EDGE_SIZE, GVE_A_EDGE_LEN, NULL);

	len = MODULE_FUN_NAME(GraphA, VnodesLength)(g);
	fprintf(stdout, "vnodes len: %d\n", len);
	for (int i = 0; i < len; i++)
	{
		node.index = i;
		node.value = i;
		MODULE_FUN_NAME(GraphA, VnodePut)(g, i, (void *)&node);
		node_p = (struct node_ext *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, i);
//		fprintf(stdout, "i: %d, index: %d, value: %d, node: %p\n", i, node_p->index, node_p->value, node_p);
	}

	len = MODULE_FUN_NAME(GraphA, EdgesLength)(g);
	fprintf(stdout, "edges len: %d\n", len);
	for (int i = 0; i < len; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		node_v = (struct node_ext *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, 2 * i);
		node_u = (struct node_ext *)MODULE_FUN_NAME(GraphA, VnodeGet)(g, 2 * i + 1);
		MODULE_FUN_NAME(GraphA, EdgeSetVnodes)(edge, (void *)node_v, (void *)node_u, NULL, NULL);
		edge_ext_p = (struct edge_ext *)MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
		edge_ext_p->value = i;
		edge_ext_p->d = i;
		edge_ext_p->f = i;
		fprintf(stdout, "i: %d, value: %d, d: %d, f: %d, v: %p, u: %p, edge: %p, ext: %p\n", 
						i, edge_ext_p->value, edge_ext_p->d, edge_ext_p->f, node_v, node_u, edge, edge_ext_p);
	}

	fprintf(stdout, "edges: third\n");
	for (int i = 0; i < len; i++)
	{
		edge = (EdgeA_T)MODULE_FUN_NAME(GraphA, EdgeGet)(g, i);
		edge_ext_p = MODULE_FUN_NAME(GraphA, EdgeGetPriv)(edge);
		MODULE_FUN_NAME(GraphA, EdgeGetVnodes)(edge, (void **)&node_v, (void **)&node_u);
		fprintf(stdout, "i: %d, value: %d, d: %d, f: %d, v: %p, u: %p, edge: %p, ext: %p\n", 
						i, edge_ext_p->value, edge_ext_p->d, edge_ext_p->f, node_v, node_u, edge, edge_ext_p);
	}

	MODULE_FUN_NAME(GraphA, free)(&g);
}


#include "sentinel-linked-list.h"

static int sen_link_cmp(void *arg, void *key)
{
	int num1 = 0;
	int num2 = 0;

	num1 = (int )arg;
	num2 = (int )key;

//	fprintf(stdout, "%s: num1: %d, num2: %d\n", __func__, num1, num2);

	if (num1 == num2) return 0;
	else return -1;
}

static  void test_sential_list(void)
{
#define SENDLINK_ITEM_LEN	102400
	SenDlink_T senLink = NULL;
	int ret = 0;
	int value = 0;
	int *out = NULL;

	senLink = MODULE_FUN_NAME(SenDlink, create)();

	fprintf(stdout, "first\n");
	for (int i = 0; i < SENDLINK_ITEM_LEN; i++)
	{
		MODULE_FUN_NAME(SenDlink, insert)(senLink, (void *)i);
	}

	for (int i = 0; i < SENDLINK_ITEM_LEN; i++)
	{
		value = random() % SENDLINK_ITEM_LEN;
		ret = MODULE_FUN_NAME(SenDlink, search)(senLink, sen_link_cmp, (void *)value, (void **)&out);
		if (ret != 0 || (int)out != value)
		{
			fprintf(stdout, "search: %d, failed, ret: %d, out: %d\n", value, ret, (int)out);
		}
	}

	fprintf(stdout, "second\n");
	for (int i = 0; i < SENDLINK_ITEM_LEN - 10; i++)
	{
		MODULE_FUN_NAME(SenDlink, delete)(senLink, sen_link_cmp, (void *)i);
		ret = MODULE_FUN_NAME(SenDlink, count)(senLink);
//		fprintf(stdout, "count: %d\n", ret);
	}

	for (int i = 0; i < 10; i++)
	{
//		fprintf(stdout, "i: %d\n", i);
		ret = MODULE_FUN_NAME(SenDlink, count)(senLink);
//		fprintf(stdout, "count: %d\n", ret);
		MODULE_FUN_NAME(SenDlink, delete_by_index)(senLink, 0);
	}

	MODULE_FUN_NAME(SenDlink, destroy)(&senLink);
}

struct test_routine {
	void (*call_back)(void);
	char *name;
};

struct test_routine my_test_routines[] = 
{
//		{test_bit, "bit"},					// ko
//		{test_dlist, "dlist"},				// ko
//		{test_queue_link, "queue_link"},	// ko
//		{test_stack_link, "stack_link"},	// ko
//		{test_queue_array, "queue_array"},	// ko
//		{test_stack_array, "stack_array"},	// ko
//		{test_ap, "ap"},					// kko
//		{test_arena, "arena"},				// ko
//		{test_threadPool, "threadPool"},	// ko
//		{test_array, "array"},				// ko
//		{test_set, "set"},					// ko
//		{test_set_list, "set-list"},					// ko
//		{test_table, "table"},				// ko
//		{test_seq, "seq"},					// ko
//		{test_ring, "ring"},				// kko
//		{test_xp, "xp"},					// ko
//		{test_ap2, "ap2"},					// ko
//		{test_rb, "rb_tree"},				// ko
//		{test_fib, "fib"},					// ko
//		{test_os_rank, "os_rank"},					// 
//		{test_interval_tree, "interval_tree"},	// ko
//		{test_list, "list"},					// ko
//		{test_queue, "queue"},					// ko
//		{test_stack, "stack"},					// ko
//		{test_gve, "gve"},					// ko
//		{test_gve_array, "gve_array"},					// ko
//		{test_sential_list, "sential_list"},					// ko
//		{test_mp, "mp"},
//		{test_text, "text"},
//		{test_str, "string"},
//		{test_db, "db"},
//		{test_binary_tree, "binary_tree"},		// ko
//		{test_sbit, "sbit"},					// ko 其他地方验证过ok
//		{test_ring_buf, "ring_buf"},			// ko 其他地方验证过ok
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
