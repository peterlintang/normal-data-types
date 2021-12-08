
/*
 * 最大堆排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "min-heapify.h"
#include "min-heapify-sort.h"


static int decrease(void **p, void *new)
{
	return 0;
}

/*
 * 堆排序
 */
int min_heap_sort(void *a[], int size, int (*cmp)(void *, void *))
{
	void *tmp = NULL;
	MinHeap_T h = NULL;

	assert(a);
	assert(size > 0);
	assert(cmp);

	h = MODULE_FUN_NAME(MinHeap, new)(a, size, cmp, decrease);

	/*
	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "%s: i: %d, %p\n", __func__, i, a[i]);
	}
	*/

	while (size > 0)
	{
		tmp = MODULE_FUN_NAME(MinHeap, extract_min)(h);
		a[size - 1] = tmp;
		size -= 1;
//		fprintf(stdout, "%s: size: %d, %p\n", __func__, size, tmp);
	}

	MODULE_FUN_NAME(MinHeap, free)(&h, 0);	// not free array a

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
	min_heap_sort(a, size, cmp);

	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %p\n", i, a[i]);
	}

	free(a);


	return 0;
}

#endif

