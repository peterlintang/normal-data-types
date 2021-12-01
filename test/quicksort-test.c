

#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"

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
	void **a = NULL;
	int len = 1024;
	int value = 0;

	if (argc == 2)
		len = atoi(argv[1]);

	a = (void **)calloc(len, sizeof(void *));
	if (a == NULL)
	{
		fprintf(stdout, "no mem %d\n", len);
		return -1;
	}

	for (int i = 0; i < len; i++)
	{
		value = random() % len + 1;
//		value = i + 1;
		a[i] = (void *)value;
	//	fprintf(stdout, "%p\n", a[i]);
	}

//	fprintf(stdout, "quick sort\n");
	quicksort(a, cmp, 0, len - 1);

	for (int i = 0; i < len; i++)
	{
		fprintf(stdout, "%d: %p\n", i, a[i]);
	}

	return 0;
}

