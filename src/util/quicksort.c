
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* O(r-p+1) */
int partition(int a[], int p, int r)
{
	int i;
	int x = a[r];
	int tmp;
	int j;
	
	j = p - 1;
	for (i = p; i < r; i++)
	{
		if (a[i] <= x)
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

/* for 7.1-2 di er wen */
int partition2(int a[], int p, int r)
{
	int i;
	int j;
	int tmp;
	int part_key;

	part_key = a[p];
	i = p;
	j = r + 1;

	while (1)
	{
		do { i++; } while (a[i] < part_key && i <= r);
		do { j--; } while (a[j] > part_key); // bu yong jia xia biao pan duan, yin wei part_key ke zuo shao bin 
		if (i > j) break;
		tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
//		fprintf(stdout, "i: %d, j: %d\n", i, j);
	}

	tmp = a[j];
	a[j] = a[p];
	a[p] = tmp;

	return j;
}

/*
 * T(r-p+1)=T(q-1-p+1)+T(r-(q+1)+1)+O(r-p+1)
 */
int quicksort(int a[], int p, int r)
{
	int q;

	if (p < r)
	{
		q = partition2(a, p, r);
//		fprintf(stdout, "%s: index: %d\n", __func__, q); // for 7.1-2 xi ti di yi wen
//		think about 7.1-2 di er wen
		quicksort(a, p, q - 1);
		quicksort(a, q + 1, r);
	}

	return 0;
}

static int random_key(int p, int r)
{
	struct timeval tv;
	int seed = 0;

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * 1000 + tv.tv_usec);
	for (; ; )
	{
		seed = rand();
		seed = seed % r;
		if (seed >= p && seed <= r)
			break;
	}

	return seed;
}

static int random_partition(int a[], int p, int r)
{
	int j;
	int tmp;

	j = random_key(p, r);
	tmp = a[j];
	a[j] = a[r];
	a[r] = tmp;

	partition(a, p, r);
}

int random_quicksort(int a[], int p, int r)
{
	int q;

	if (p < r)
	{
		q = random_partition(a, p, r);
		random_quicksort(a, p, q - 1);
		random_quicksort(a, q + 1, r);
	}
}

int main(int argc, char *argv[])
{
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
	int a[] = { 3, 4, 8, 5, 6, 7, 9, 2, 12, 18, 29, 30, 13, 15, 87, 76, 56, 36 };
//	int a[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

	fprintf(stdout, "before sort...:%d\n", ARRAY_SIZE(a));
	for (int i = 0; i < ARRAY_SIZE(a); i++)
	{
		fprintf(stdout, "%d\n", a[i]);
	}

	quicksort(a, 0, ARRAY_SIZE(a) - 1);
//	random_quicksort(a, 0, ARRAY_SIZE(a) - 1);
	fprintf(stdout, "after sort...\n");
	for (int i = 0; i < ARRAY_SIZE(a); i++)
	{
		fprintf(stdout, "%d\n", a[i]);
	}

	return 0;
}


