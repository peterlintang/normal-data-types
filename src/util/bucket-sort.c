

#include <stdio.h>
#include <stdlib.h>

struct node {
	float data;
	struct node *next;
};

struct node *new_node(float data)
{
	struct node *node = NULL;

	node = (struct node *)calloc(1, sizeof(*node));
	if (node)
	{
		node->data = data;
		node->next = NULL;
	}
	else
	{
		exit(8);
	}

	return node;
}

struct node *insert_sort(struct node *list)
{
	struct node *node = NULL;
	struct node *new_list = NULL;

	if (list == NULL)
	{
		return NULL;
	}

	while (list)
	{
		/* get a node from @list */
		node = list;
		list = list->next;
		node->next = NULL;

		if (new_list == NULL)
		{
			new_list = node;
		}
		else
		{
			struct node *prev = NULL;
			struct node *tmp = new_list;

			while (tmp)
			{
				if (node->data <= tmp->data)
				{
					node->next = tmp;
					if (prev)
						prev->next = node;
					else
						new_list = node;
					break;
				}
				prev = tmp;
				tmp = tmp->next;
			}
		}
	}

	return new_list;
}

/*
 * tong pai xu // bucket sort
 * a: array, elements in a between [0,1)
 * size: array size;
 */
int bucket_sort(float a[], int size)
{
//	struct node *b[size]; // list array
	struct node **b = NULL;
	struct node *node = NULL;
	struct node *old = NULL;
	int j = 0;
	int index = 0;

	b = (struct node **)calloc(size, sizeof(struct node *));
	if (b == NULL) exit(5);

	for (int i = 0; i < size; i++)
		b[i] = NULL;

	for (int i = 0; i < size; i++)
	{
//		insert a[i] to b[size * a[i]]; 
		index = size * a[i];
		node = new_node(a[i]);
		node->next = b[index];
		b[index] = node;
	}
#if 0
	for (int i = 0; i < size; i++)
	{
		node = b[i];
		while (node)
		{
			fprintf(stdout, "i: %d, value: %f\n", i, node->data);
			node = node->next;
		}
	}
#endif

	for (int i = 0; i < size; i++)
	{
//		use insert sort to sort list b[i];
		b[i] = insert_sort(b[i]);
	}

#if 0
	for (int i = 0; i < size; i++)
	{
		node = b[i];
		while (node)
		{
			fprintf(stdout, "i: %d, value: %f\n", i, node->data);
			node = node->next;
		}
	}
#endif
//	concatenate the list b[0] ... b[n-1] togethor in order;
	for (int i = 0; i < size; i++)
	{
		node = b[i];
		while (node)
		{
			a[j++] = node->data;
			old = node;
			node = node->next;
			free(old);
		}
	}

	free(b);
	return 0;
}



int main(int argc, char *argv[])
{
	int length = 64;
	int seed = 73;
	float *a = NULL;
	/*
	float a[] = {0.1, 0.2, 0.3, 0.1, 
				 0.4, 0.5, 0.8, 0.1,
				 0.4, 0.3, 0.7, 0.5,
				 0.1, 0.1, 0.1, 0.3};
				 */

	if (argc == 3)
	{
		seed = atoi(argv[1]);
		length = atoi(argv[2]);
	}

	if (argc == 2)
	{
		seed = atoi(argv[1]);
	}

	a = (float *)calloc(length, sizeof(float));
	if (a == NULL) exit(5);

	srand(seed);

#if 0
	for (int i = 0; i < length; i++)
	{
		a[i] = (rand() % length) / (float)length;
		fprintf(stdout, "%f ", a[i]);
		if ((i + 1) % 8 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	bucket_sort(a, length);

#if 0
	for (int i = 0; i < length; i++)
	{
		fprintf(stdout, "%f ", a[i]);
		if ((i + 1) % 8 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	free(a);

	return 0;
}









