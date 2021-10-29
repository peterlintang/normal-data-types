
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "randomized-select.h"

static int randomized_partition(void *a[], int (*cmp)(void *, void *), int p, int r)
{
	struct timeval tv;
	int i;
	int j;
	int k;
	int index;
	void *tmp = NULL;;
	void *x = a[r];

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * 1000 + tv.tv_usec);
	while (1)
	{
		index = random();
		index = index % (r - p + 1) + p;
		if (index <= r && index >= p)
			break;
	}

	tmp = a[index];
	a[index] = a[r];
	a[r] = tmp;

	x = a[r];
	j = k = p - 1;
	for (i = p; i < r; i++)
	{
		if (cmp(a[i], x) < 0)
		{
			j++;
			tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}

	j++;
	tmp = a[j];
	a[j] = a[r];
	a[r] = tmp;
	
	return j;
}

/*
 * choose the @i index of elements in array between p and r
 */
void *randomized_select(void *a[], int (*cmp)(void *, void *), int p, int r, int i)
{
	int q;
	static int count = 1;


//fprintf(stdout, "count: %d, p: %d, r: %d, i: %d\n", count, p, r, i);
	if (p < r)
	{
		q = randomized_partition(a, cmp, p, r);
		if (i < q)
			return randomized_select(a, cmp, p, q - 1, i);
		else if (i > q)
			return randomized_select(a, cmp, q + 1, r, i);

		return a[q];
	}
	else
	{
		if (p == r && r == i)
			return a[p];
		return NULL;
	}
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

/*
 * test main
 */
int main(int argc, char *argv[])
{
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
	void *a[] = { 
		(void *)8, (void *)9, (void *)0, (void *)7, 
		(void *)5, (void *)4, (void *)3, (void *)2, 
		(void *)6, (void *)1, (void *)12, (void *)13, 
		(void *)15, (void *)19, (void *)18, (void *)20
		};
	int index = 8;
	void *result = NULL;

	index = atoi(argv[1]);
	result = randomized_select(a, cmp, 0, ARRAY_SIZE(a) - 1, index);
	for (int i = 0; i < ARRAY_SIZE(a); i++)
		fprintf(stdout, "i: %d, %p\n", i, a[i]);

	fprintf(stdout, "index: %d, value: %p\n", index, result);

	return 0;
}

#endif 


