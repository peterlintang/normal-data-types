
#include <stdio.h>
#include <stdlib.h>

int merge(int in[], int p, int q, int r)
{
	int i;
	int j;
	int k;
	int n1 = 0;
	int n2 = 0;

	int *L = NULL;
	int *R = NULL;

	n1 = q - p + 1;
	n2 = r - q;

	L = (int *)calloc(n1, sizeof(int));
	R = (int *)calloc(n2, sizeof(int));
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
		if (L[i] < R[j])
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

int merge_sort(int in[], int p, int r)
{
	int q = 0;

	if (p < r)
	{
		q = (p + r) / 2;
		merge_sort(in, p, q);
		merge_sort(in, q + 1, r);
		merge(in, p, q, r);
	}
}


int main(int argc, char *argv[])
{
	int i;
	int length = 1024;
	int *in = NULL;

	length = atoi(argv[1]);
	in = (int *)calloc(length, sizeof(int));

	for (i = 0; i < length; i++)
	{
		in[i] = 10 * (length - i);
	}

	/*
	for (i = 0; i < length ; i++)
	{
		fprintf(stdout, "%d ", in[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	*/

	merge_sort(in, 0, length - 1);

	/*
	for (i = 0; i < length ; i++)
	{
		fprintf(stdout, "%d ", in[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	*/

	free(in);

	return 0;
}




