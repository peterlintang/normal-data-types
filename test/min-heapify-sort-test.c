
/*
 * 最大堆排序
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "min-heapify.h"
#include "min-heapify-sort.h"




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

	seed = atoi(argv[2]);
	srand(seed);

	size = atoi(argv[1]);
	a = (void **)calloc(size, sizeof(void *));

	for (int i = 0; i < size; i++)
	{
		a[i] = (void *)(rand() % size + 1);
	}

	fprintf(stdout, "before sort\n");
	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %p\n", i, a[i]);
	}

	fprintf(stdout, "after sort\n");
	min_heap_sort(a, size, cmp);

	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %p\n", i, a[i]);
	}

	free(a);


	return 0;
}


