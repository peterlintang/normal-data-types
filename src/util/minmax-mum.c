
#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include "minmax.h"

void *minimum(void *a[], int size, int (*cmp)(void *, void *))
{
	void *min = a[0];

	for (int i = 1; i < size; i++)
	{
		if (cmp(a[i], min) < 0)
			min = a[i];
	}

	return min;
}

void *maximum(void *a[], int size, int (*cmp)(void *, void *))
{
	void *max = a[0];

	for (int i = 1; i < size; i++)
	{
		if (cmp(a[i], max) > 0)
			max = a[i];
	}

	return max;
}

/*
 * find the min and max in array a
 */
int minmax(void *a[], int size, int (*cmp)(void *, void *), void **pmin, void **pmax)
{
	void *min = a[0];
	void *max = a[0];

	for (int i = 1; i < size; i++)
	{
		if (cmp(a[i], min) < 0)
			min = a[i];
		if (cmp(a[i], max) > 0)
			max = a[i];
	}

	*pmin = min;
	*pmax = max;

	return 0;
}

int minmax2(void *a[], int size, int (*cmp)(void *, void *), void **pmin, void **pmax)
{
	void *min = NULL;
	void *max = NULL;

	if (size % 2 == 0)
	{
		min = cmp(a[0], a[1]) > 0 ? a[1] : a[0];
		max = cmp(a[0], a[1]) > 0 ? a[0] : a[1];
	}
	else
	{
		min = max = a[0];
	}

	for (int i = (size % 2 == 0 ? 2 : 1); i < size; i += 2)
	{
		if (cmp(a[i], a[i + 1]) > 0)
		{
			if (cmp(max, a[i]) < 0)
				max = a[i];
			if (cmp(min, a[i + 1]) > 0)
				min = a[i + 1];
		}
		else
		{
			if (cmp(max, a[i + 1]) < 0)
				max = a[i + 1];
			if (cmp(min, a[i]) > 0)
				min = a[i];
		}
	}

	*pmin = min;
	*pmax = max;

	return 0;
}

#if 0

static int cmp(void *x, void *y)
{
	int x_i = (int)x;
	int y_i = (int)y;

	if (x_i > y_i) return 1;
	else if (x_i < y_i) return -1;
	else return 0;
}

int main(int argc, char *argv[])
{
	int length = 1024;
	int seed = 79;
	void **a = NULL;
	void *min = NULL;
	void *max = NULL;

	if (argc == 3)
	{
		seed = atoi(argv[1]);
		length = atoi(argv[2]);
	}
	if (argc == 2)
		seed = atoi(argv[1]);

	a = (void **)calloc(length, sizeof(void *));
	if (a == NULL) exit(3);

	srand(seed);
	for (int i = 0; i < length; i++)
	{
		a[i] = (void *)(rand() % length + 1);
	}

	min = minimum(a, length, cmp);
	fprintf(stdout, "min: %p\n", min);

	max = maximum(a, length, cmp);
	fprintf(stdout, "max: %p\n", max);

	minmax2(a, length, cmp, &min, &max);
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%p\n", a[i]);
	}

	fprintf(stdout, "min: %p, max: %p\n", min, max);

	free(a);

	return 0;
}

#endif


