
/*
 * 最大堆排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "max-heapify.h"


static int increase(void **p, void *new)
{
	return 0;
}

/*
 * 堆排序
 */
int max_heap_sort(void *a[], int size, int (*cmp)(void *, void *))
{
	void *tmp = NULL;
	MaxHeap_T h = NULL;

	assert(a);
	assert(size > 0);
	assert(cmp);

	h = MODULE_FUN_NAME(MaxHeap, new)(a, size, cmp, increase);

	while (size > 1)
	{
		tmp = MODULE_FUN_NAME(MaxHeap, extract_max)(h);
		a[size - 1] = tmp;
		size -= 1;
	}

	MODULE_FUN_NAME(MaxHeap, free)(&h, 0);	// not free array a

	return 0;
}


#if 0

static int cmp(void *x, void *y)
{
	unsigned long x_i = (unsigned long)x;
	unsigned long y_i = (unsigned long)y;

	if (x_i > y_i) return 1;
	else if (x_i < y_i) return -1;
	else return 0;
}

int main(int argc, char *argv[])
{
	void **a = NULL;
	int size = 16;
	int seed = 31;
	int del = 3;

	seed = atoi(argv[2]);
	srand(seed);

	size = atoi(argv[1]);
	a = (void **)calloc(size, sizeof(void *));

	for (int i = 0; i < size; i++)
	{
		a[i] = (void *)(rand() % size + 1);
	}

	/*
	fprintf(stdout, "before sort\n");
	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %p\n", i, a[i]);
	}
	*/

	fprintf(stdout, "after sort\n");
	max_heap_sort(a, size, cmp);

	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %p\n", i, a[i]);
	}

	free(a);

	return 0;
}

#endif

