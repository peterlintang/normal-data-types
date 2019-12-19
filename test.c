
#include <stdio.h>
#include <stdlib.h>

#include "bit.h"
#include "list2.h"


static void test_bit(void)
{
	Bit_T set = NULL;
	set = MODULE_FUN_NAME(Bit, new)(36);
	if (NULL == set)
	{
		return ;
	}
	MODULE_FUN_NAME(Bit, set)(set, 5, 10);
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));
	MODULE_FUN_NAME(Bit, put)(set, 20, 1);
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));

}

static void test_list2(void)
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
		fprintf(stdout, "i: %d, value: %d\n", i, (int)node->priv);
	}

	while (node = MODULE_FUN_NAME(List, head)(list))
	{
		fprintf(stdout, "value: %d\n", (int)node->priv);
		MODULE_FUN_NAME(List, remove)(list, node);
		MODULE_FUN_NAME(ListNode, free)(&node);
	}

	MODULE_FUN_NAME(List, free)(&list);

}

struct test_routine {
	void (*call_back)(void);
};

struct test_routine my_test_routines[] = 
{
		test_bit,
		test_list2,
		NULL,
};

int main(int argc, char *argv[])
{
	int ret = 0;
	int i;

	for (i = 0; my_test_routines[i].call_back != NULL; i++)
	{
		my_test_routines[i].call_back();
	}

	return ret;
}
