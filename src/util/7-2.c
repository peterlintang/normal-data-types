
/*
 * for 7-2 b
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* time: O(r-p+1) 
 *
 * a[p...r]
 *
 * return a[p...q..t...r]
 * p <= k <= q - 1; a[k] < a[q];
 * q <= k <= t;    a[k] = a[q] = q[t];
 * t + 1 <= k <= r; a[k] > a[t];
 *
 * hai ke yi you hua, shi jian fu za du ke yi shi O(r-p+1)
 */
int partition(int a[], int p, int r, int *q, int *t)
{
	int i;
	int x = a[r];
	int tmp;
	int j;
	
	/* count q */
	j = p - 1;
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

	*q = j;

	/* count t */
	for (i = j + 1; i <= r; i++)
	{
		if (a[i] == x)
		{
			j++;
			tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}

	*t = j;
	return 0;
}

int partition2(int a[], int p, int r, int *q, int *t)
{
	int i;
	int x = a[r];
	int tmp;
	int j;
	int k; // record the count number of elements equal to x
	
	j = p - 1;
	k = 0;
	for (i = p; i < r; i++)
	{
		if (a[i] < x)
		{
			j++;
			/*
			if (k == 0)
			{
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
			else
			{
				int index = j + k;
				tmp = a[i];
				a[i] = a[index];
				a[index] = a[j];
				a[j] = tmp;
			}
			*/  // can be changed as below
			tmp = a[i];
			a[i] = a[j + k];
			a[j + k] = a[j];
			a[j] = tmp;
		} 
		else if (a[i] == x)
		{
			int index = j + 1 + k;
			tmp = a[i];
			a[i] = a[index];
			a[index] = tmp;
			k++;
		}
	}

	tmp = a[j + 1 + k];
	a[j + 1 + k] = a[r];
	a[r] = tmp;

	*q = j + 1;
	*t = j + 1 + k;

	return 0;
}

/*
 * T(r-p+1)=T(q-1-p+1)+T(r-(q+1)+1)+O(r-p+1)
 */
int quicksort(int a[], int p, int r)
{
	int q = 0;
	int t = 0;

	if (p < r)
	{
		partition2(a, p, r, &q, &t);
		fprintf(stdout, "%s: index: %d, %d\n", __func__, q, t); 
		quicksort(a, p, q - 1);
		quicksort(a, t + 1, r);
	}

	return 0;
}


int main(int argc, char *argv[])
{
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
//	int a[] = { 3, 4, 8, 5, 6, 7, 9, 2, 12, 18, 29, 30, 13, 15, 87, 76, 56, 36, 7};
//	int a[] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
	int *a = NULL;
	int length = 1024;
	int seed = 91;

	if (argc == 3)
	{
		length = atoi(argv[1]);
		seed = atoi(argv[2]);
	}

	if (argc == 2)
		seed = atoi(argv[1]);

	a = (int *)calloc(length, sizeof(int));
	if (a == NULL) exit(4);

	srand(seed);

	for (int i = 0; i < length; i++)
	{
		a[i] = rand() % length;
	}

	fprintf(stdout, "before sort...:%d\n", length);
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%4d ", a[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

	quicksort(a, 0, length  - 1);
	fprintf(stdout, "after sort...\n");
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%4d ", a[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

	return 0;
}


