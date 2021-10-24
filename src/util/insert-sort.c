
#include <stdio.h>
#include <stdlib.h>


int insert_sort(int in[], int p, int r)
{
	int i;
	int j;
	int tmp;

	for (i = p + 1; i <= r; i++)
	{
		j = i;
		while ((j > 0) && (in[j] < in[j - 1]))
		{
			tmp = in[j];
			in[j] = in[j - 1];
			in[j - 1] = tmp;
			j--;
		}
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

	insert_sort(in, 0, length - 1);

	/*
	for (i = 0; i < length ; i++)
	{
		fprintf(stdout, "%d ", in[i]);
		if ((i + 1) % 10 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	*/

	free(in);

	return 0;
}




