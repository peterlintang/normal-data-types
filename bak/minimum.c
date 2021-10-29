
#include <stdio.h>
#include <stdlib.h>

int minimum(int a[], int size)
{
	int min = a[0];

	for (int i = 1; i < size; i++)
	{
		if (a[i] < min)
			min = a[i];
	}

	return min;
}


int main(int argc, char *argv[])
{
	int length = 1024;
	int seed = 79;
	int *a = NULL;
	int min;

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

	min = minimum(a, length);
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%4d ", a[i]);
		if ((i + 1) % 10 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\nmin: %d\n", min);

	free(a);

	return 0;
}
