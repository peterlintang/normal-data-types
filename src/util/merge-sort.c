
#include <stdio.h>
#include <stdlib.h>
#include "merge-sort.h"

int merge(void *in[], int (*cmp)(void *, void *), int p, int q, int r)
{
	int i;
	int j;
	int k;
	int n1 = 0;
	int n2 = 0;

	void **L = NULL;
	void **R = NULL;

	n1 = q - p + 1;
	n2 = r - q;

	L = (void **)calloc(n1, sizeof(void *));
	R = (void **)calloc(n2, sizeof(void *));
	if (NULL == L || NULL == R)
	{
		fprintf(stderr, "no mem\n");
		exit(1);
	}

	for (i = 0; i < n1; i++)
		L[i] = in[p + i];
	for (j = 0; j < n2; j++)
		R[j] = in[q + j + 1];

	k = p;
	i = 0;
	j = 0;
	while (i < n1 && j < n2)
	{
		if (cmp(L[i], R[j]) < 0)
		{
			in[k] = L[i];
			i++;
		}
		else
		{
			in[k] = R[j];
			j++;
		}
		k++;
	}

	if (i == n1)
	{
		for (; j < n2; j++, k++)
			in[k] = R[j];
	}

	if (j == n2)
	{
		for (; i < n1; i++, k++)
			in[k] = L[i];
	}

	free(L);
	free(R);
	return 0;
}

int merge_sort(void *in[], int (*cmp)(void *, void *), int p, int r)
{
	int q = 0;

	if (p < r)
	{
		q = (p + r) / 2;
		merge_sort(in, cmp, p, q);
		merge_sort(in, cmp, q + 1, r);
		merge(in, cmp, p, q, r);
	}

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
	int i;
	int length = 1024;
	void **in = NULL;

	if (argc == 2)
		length = atoi(argv[1]);

	in = (void **)calloc(length, sizeof(void *));

	for (i = 0; i < length; i++)
	{
		in[i] = (void *)(random() % length + 1);
	}

	
	for (i = 0; i < length ; i++)
	{
		fprintf(stdout, "%p\n", in[i]);
	}
	fprintf(stdout, "\n");
	

	merge_sort(in, cmp, 0, length - 1);

	for (i = 0; i < length ; i++)
	{
		fprintf(stdout, "%p\n", in[i]);
	}

	free(in);

	return 0;
}

#endif


