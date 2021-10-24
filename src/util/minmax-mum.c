
#include <stdio.h>
#include <stdlib.h>

/*
 * find the min and max in array a
 */
int minmax_mum(int a[], int size, int *pmin, int *pmax)
{
	int min = a[0];
	int max = a[0];

	for (int i = 1; i < size; i++)
	{
		if (a[i] < min)
			min = a[i];
		if (a[i] > max)
			max = a[i];
	}

	*pmin = min;
	*pmax = max;

	return 0;
}

int minmax_mum2(int a[], int size, int *pmin, int *pmax)
{
	int min;
	int max;

	if (size % 2 == 0)
	{
		min = a[0] > a[1] ? a[1] : a[0];
		max = a[0] > a[1] ? a[0] : a[1];
	}
	else
	{
		min = max = a[0];
	}

	for (int i = (size % 2 == 0 ? 2 : 1); i < size; i += 2)
	{
		if (a[i] > a[i + 1])
		{
			if (max < a[i])
				max = a[i];
			if (min > a[i + 1])
				min = a[i + 1];
		}
		else
		{
			if (max < a[i + 1])
				max = a[i + 1];
			if (min > a[i])
				min = a[i];
		}
	}

	*pmin = min;
	*pmax = max;

	return 0;
}



int main(int argc, char *argv[])
{
	int length = 1024;
	int seed = 79;
	int *a = NULL;
	int min;
	int max;

	if (argc == 3)
	{
		seed = atoi(argv[1]);
		length = atoi(argv[2]);
	}
	if (argc == 2)
		seed = atoi(argv[1]);

	a = (int *)calloc(length, sizeof(int));
	if (a == NULL) exit(3);

	srand(seed);
	for (int i = 0; i < length; i++)
	{
		a[i] = rand() % length;
	}

	minmax_mum2(a, length, &min, &max);
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%4d ", a[i]);
		if ((i + 1) % 10 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\nmin: %d, max: %d\n", min, max);

	free(a);

	return 0;
}



