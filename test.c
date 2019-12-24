
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>



#include "bit.h"
static void test_bit(void)
{
	Bit_T set = NULL;
	set = MODULE_FUN_NAME(Bit, new)(36);
	if (NULL == set)
	{
		return ;
	}
	MODULE_FUN_NAME(Bit, set)(set, 5, 10);
//	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));
	MODULE_FUN_NAME(Bit, put)(set, 20, 1);
//	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));
	MODULE_FUN_NAME(Bit, free)(&set);

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

struct test_routine {
	void (*call_back)(void);
	char *name;
};

struct test_routine my_test_routines[] = 
{
		{test_bit, "bit"},
		{test_dlist, "dlist"},
		{test_queue_link, "queue_link"},
		{test_stack_link, "stack_link"},
//		{test_queue_array, "queue_array"},
//		{test_stack_array, "stack_array"},
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
