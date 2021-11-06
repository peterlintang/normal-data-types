
#include <stdio.h>
#include <stdlib.h>

static int get_index(int value, int d)
{
	int tmp1;
	int tmp2;
	int index;

	tmp1 = value;
	do {
		index = tmp1 % 10;
		tmp2 = tmp1 / 10;
		tmp1 = tmp2;
	} while (d-- > 0);

	return index;
}

static int counting_sort4(int a[], int size, int d, int k)
{
	int i;
	int *b = NULL;
	int *c = NULL;
	int index;

	b = (int *)calloc(size, sizeof(int));
	if (b == NULL) exit(5);
	c = (int *)calloc(k, sizeof(int));
	if (c == NULL) exit(5);

	for (i = 0; i < size; i++)
		c[i] = 0;

	for (i = 0; i < size; i++)
	{
		index = get_index(a[i], d);
		c[index]++;
	}

	for (i = 1; i < size; i++)
	{
		c[i] = c[i] + c[i - 1];
	}

	for (i = size - 1; i >= 0; i--)
	{
		index = get_index(a[i], d);
		b[c[index] - 1] = a[i];
		c[index] -= 1;
	}

	for (i = 0; i < size; i++)
		a[i] = b[i];

	free(b);
	free(c);

	return 0;
}



/*
 * a[]:	array;
 * size: array size;
 * d: element's width; wei kuan
 * k: mei yi wei shang shu zi de qu zhi wei 0 dao (k - 1);
 */
int radix_sort(int a[], int size, int d, int k)
{
		// shi yong yi ge wen ding de pai su
	for (int i = 0; i < d; i++)
	{
		counting_sort4(a, size, i, k);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int *a = NULL;
	int length = 1024;
	int seed = 23;
	int d = 0;
	int max = 0;
	int k;

	if (argc == 3)
	{
		length = atoi(argv[2]);
		seed = atoi(argv[1]);
	}
	if (argc == 2)
		seed = atoi(argv[1]);

	a = (int *)calloc(length, sizeof(int));
	if (a == NULL) exit(4);

	srand(seed);
	k = length + 1123;

	for (int i = 0; i < length; i++)
	{
		a[i] = rand() % (length + 1123);
		if (a[i] > max) max = a[i];
#if 0
		fprintf(stdout, "%6d ", a[i]);
		if ((i + 1) % 10 == 0) fprintf(stdout, "\n");
#endif
	}
	fprintf(stdout, "\n");
	
	fprintf(stdout, "d: %d, max: %d\n", d, max);
	{
		int quot;
		int surp;
		do {
			quot = max / 10;
			surp = max % 10;
			max = quot;
			d++;
		} while (quot > 0);
	}
	fprintf(stdout, "d: %d, max: %d\n", d, max);


	radix_sort(a, length, d, k);

#if 0
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%6d ", a[i]);
		if ((i + 1) % 10 == 0) fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif
	free(a);

	return 0;
}


