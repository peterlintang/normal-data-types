
#include <stdio.h>
#include <stdlib.h>

#include "insert-sort.h"


int insert_sort(void *in[], int (*cmp)(void *, void *), int p, int r)
{
	int i;
	int j;
	void *tmp = NULL;

	for (i = p + 1; i <= r; i++)
	{
		j = i;
		while ((j > 0) && (cmp(in[j], in[j - 1]) < 0))
		{
			tmp = in[j];
			in[j] = in[j - 1];
			in[j - 1] = tmp;
			j--;
		}
	}

	return 0;
}


#if 0

static int cmp(void *x, void *y)
{
	int x_i = (int )x;
	int y_i = (int )y;

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
//		in[i] = (void *)(10 * i + 1);
		in[i] = (void *)(random() % length + 1);
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

	insert_sort(in, cmp, 0, length - 1);

	
	for (i = 0; i < length ; i++)
	{
		fprintf(stdout, "%p\n", in[i]);
	}

	free(in);

	return 0;
}


#endif 


