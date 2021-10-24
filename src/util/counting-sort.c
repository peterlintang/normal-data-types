
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * assume that all elements' value in a is between 0 to k
 * and all the elements are diffrent;
 */
int counting_sort(int a[], int b[], int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		b[a[i]] = a[i];
	}

	return 0;
}

/*
 * assume that all elements' value in a is between 0 to k
 */
int counting_sort2(int a[], int b[], int size, int k)
{
	int i;
	int j;
	int c[k];

	memset(c, 0, sizeof(c));

	/* tong ji yuan su de pin lv, */
	for (i = 0; i < size; i++)
	{
		c[a[i]]++;
	}

	/* gen ju pin lv, tian mei ge yuan su */
	int sum = 0;
	for (i = 0; i < k; i++)
	{
		for (j = 0; j < c[i]; j++)
		{
			b[sum++] = i;
		}
	}
	

	return 0;
}

int counting_sort3(int a[], int b[], int size, int k)
{
	int i;
	int c[k];

	for (i = 0; i < k; i++)
		c[i] = 0;

	for (i = 0; i < size; i++)
	{
		c[a[i]]++;
	}

	/*
	fprintf(stdout, "hello world\n");
	for (i = 0; i < k; i++)
		fprintf(stdout, "i: %d, %d\n", i, c[i]);
	fprintf(stdout, "hello world\n");
	*/

	for (i = 1; i < k; i++)
	{
		c[i] = c[i] + c[i - 1];
	}

	/*
	for (i = 0; i < k; i++)
		fprintf(stdout, "i: %d, %d\n", i, c[i]);
	fprintf(stdout, "hello world\n");
	*/

	for (i = size - 1; i >= 0; i--)
	{
		b[c[a[i]] - 1] = a[i];
		c[a[i]] -= 1;
	}

	return 0;
}

int counting_sort4(int a[], int size, int k)
{
	int i;
	int b[size];
	int c[k];

	for (i = 0; i < k; i++)
		c[i] = 0;

	for (i = 0; i < size; i++)
	{
		c[a[i]]++;
	}

	for (i = 1; i < k; i++)
	{
		c[i] = c[i] + c[i - 1];
	}

	for (i = size - 1; i >= 0; i--)
	{
		b[c[a[i]] - 1] = a[i];
		c[a[i]] -= 1;
	}

	for (i = 0; i < size; i++)
		a[i] = b[i];

	return 0;
}

int main(int argc, char *argv[])
{
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))
	int a[] = { 8, 2, 3, 7, 9, 5, 6, 0, 1, 4 };
	int b[10]  = { 0 };

//	counting_sort(a, b, ARRAY_SIZE(a));
//	counting_sort2(a, b, ARRAY_SIZE(a), 10);
	counting_sort3(a, b, ARRAY_SIZE(a), 10);
	counting_sort4(a, ARRAY_SIZE(a), 10);

	for (int i = 0; i < ARRAY_SIZE(a); i++)
	{
		fprintf(stdout, "%d\n", a[i]);
	}
	fprintf(stdout, "after sort...\n");
	for (int i = 0; i < ARRAY_SIZE(b); i++)
	{
		fprintf(stdout, "%d\n", b[i]);
	}

	return 0;
}


