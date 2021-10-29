
/*
 * for 7-4
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* 
 * time: O(r-p+1) 
 */
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


/*
 */
int tail_quicksort(int a[], int p, int r)
{
	int q = 0;

	while (p < r)
	{
		q = partition(a, p, r);
		tail_quicksort(a, p, q - 1);
		p = q + 1;
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

	tail_quicksort(a, 0, length  - 1);
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


