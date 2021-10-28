
#include <stdlib.h>	// for srand
#include <sys/time.h> // for time

#include "quicksort.h"

/* O(r-p+1) */
static int partition(void *a[], int (*cmp)(void *, void *), int p, int r)
{
	int i;
	int j;
	void *x = a[r];
	void *tmp;
	
	j = p - 1;
	for (i = p; i < r; i++)
	{
		if (cmp(a[i], x) <= 0)
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

static int partition2(void *a[], int (*cmp)(void *, void *), int p, int r)
{
	int i;
	int j;
	void *tmp;
	void *part_key;

	part_key = a[p];
	i = p;
	j = r + 1;

	while (1)
	{
		do { i++; } while ((cmp(a[i], part_key) < 0) && i <= r);
		do { j--; } while (cmp(a[j], part_key) > 0); // bu yong jia xia biao pan duan, yin wei part_key ke zuo shao bin 
		if (i > j) break;
		tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}

	tmp = a[j];
	a[j] = a[p];
	a[p] = tmp;

	return j;
}

/*
 * T(r-p+1)=T(q-1-p+1)+T(r-(q+1)+1)+O(r-p+1)
 */
int quicksort(void *a[], int (*cmp)(void *, void *), int p, int r)
{
	int q;

	if (p < r)
	{
		q = partition2(a, cmp, p, r);
		quicksort(a, cmp, p, q - 1);
		quicksort(a, cmp, q + 1, r);
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

static int random_partition(void *a[], int (*cmp)(void *, void *), int p, int r)
{
	int j;
	void *tmp;

	j = random_key(p, r);
	tmp = a[j];
	a[j] = a[r];
	a[r] = tmp;

	return partition(a, cmp, p, r);
}

int random_quicksort(void *a[], int (*cmp)(void *, void *), int p, int r)
{
	int q;

	if (p < r)
	{
		q = random_partition(a, cmp, p, r);
		random_quicksort(a, cmp, p, q - 1);
		random_quicksort(a, cmp, q + 1, r);
	}

	return 0;
}

#if 0
static int cmp(void *x, void *y)
{
	int i_x = (int)x;
	int i_y = (int)y;

	if (i_x > i_y) return 1;
	else if (i_x < i_y) return -1;
	else return 0;
}

int main(int argc, char *argv[])
{
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
	void *a[] = { 
			(void *)3, (void *)4, (void *)8, (void *)5, 
			(void *)6, (void *)7, (void *)9, (void *)2, 
			(void *)12, (void *)18, (void *)29, (void *)30, 
			(void *)13, (void *)15, (void *)87, (void *)76, (void *)56, (void *)36 };
//	int a[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

	fprintf(stdout, "before sort...:%d\n", ARRAY_SIZE(a));
	for (int i = 0; i < ARRAY_SIZE(a); i++)
	{
		fprintf(stdout, "%p\n", a[i]);
	}

//	quicksort(a, cmp, 0, ARRAY_SIZE(a) - 1);
	random_quicksort(a, cmp, 0, ARRAY_SIZE(a) - 1);
	fprintf(stdout, "after sort...hello\n");
	for (int i = 0; i < ARRAY_SIZE(a); i++)
	{
		fprintf(stdout, "%p\n", a[i]);
	}

	return 0;
}

#endif 


