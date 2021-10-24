
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

static int randomized_partition(int a[], int p, int r)
{
	struct timeval tv;
	int x = a[r];
	int i;
	int j;
	int k;
	int tmp;
	int index;

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
		if (a[i] < x)
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
int randomized_select(int a[], int p, int r, int i)
{
	int q;
	static int count = 1;


//fprintf(stdout, "count: %d, p: %d, r: %d, i: %d\n", count, p, r, i);
	if (p < r)
	{
		q = randomized_partition(a, p, r);
		if (i < q)
			return randomized_select(a, p, q - 1, i);
		else if (i > q)
			return randomized_select(a, q + 1, r, i);

		return a[q];
	}
	else
	{
		if (p == r && r == i)
			return a[p];
		return -1;
	}
}

/*
 * test main
 */
int main(int argc, char *argv[])
{
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
	int a[] = { 8, 9, 0, 7, 5, 4, 3, 2, 6, 1, 12, 13, 15, 19, 18, 20};
	int index = 8;
	int result;

	index = atoi(argv[1]);
	result = randomized_select(a, 0, ARRAY_SIZE(a) - 1, index);
	for (int i = 0; i < ARRAY_SIZE(a); i++)
		fprintf(stdout, "i: %d, %d\n", i, a[i]);

	fprintf(stdout, "index: %d, value: %d\n", index, result);

	return 0;
}
