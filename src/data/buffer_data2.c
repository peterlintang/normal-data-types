
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "buffer_data.h"

#define T buffer_data_t

struct FreeNode {
	void *next;
};

struct T {
	int free_elements;
	int total_elements;
	int element_size;
	struct FreeNode *free_list;
	void *buffer;
};

T buffer_data_create(int element_size, int total_elements)
{
	T bd = (T)calloc(1, sizeof(*bd));
	if (bd)
	{
		bd->total_elements = total_elements;
		bd->free_elements = total_elements;
		bd->element_size = element_size;
		bd->buffer = (void *)calloc(total_elements, element_size + sizeof(struct FreeNode));
		if (bd->buffer == NULL)
		{
			free(bd);
			return NULL;
		}
	}

	bd->free_list = NULL;
	char *buffer = bd->buffer;
	int offset = 0;
	struct FreeNode *node = NULL;
	for (int i = 0; i < total_elements; i++)
	{
		offset = i * (element_size + sizeof(struct FreeNode));
		node = (struct FreeNode *)(buffer + offset);
		node->next = bd->free_list;
		bd->free_list = node;

	}

	return bd;
}

void buffer_data_destroy(T *bp)
{
	T bd = *bp;

	assert(bp);
	assert(bd);

	free(bd->buffer);
	free(bd);
	*bp = NULL;
}

void *buffer_data_calloc(T b)
{
	assert(b);

	struct FreeNode *node = NULL;
	if (b->free_list)
	{
		node = b->free_list;
		b->free_list = node->next;
		node->next = NULL;
		b->free_elements--;
		return (void *)((char *)node + sizeof(struct FreeNode));
	}
	else
	{
		return NULL;
	}

}

void buffer_data_free(T b, void *p)
{
	assert(b);
	assert(p);

	struct FreeNode *node = NULL;
	node = (struct FreeNode *)((char *)p - sizeof(struct FreeNode));

	node->next = b->free_list;
	b->free_list = node;
	b->free_elements++;
}

void buffer_data_print_info(T b)
{
	printf("free elements: %d\n", b->free_elements);
}

#undef T

#if 1
#define T buffer_data_t

#define ELEMENT_NUM	1000000
int main(int argc, char *argv[])
{
	struct aa {
		int x;
		int y;
		double f;
	};
	struct aa *p = NULL;
	struct aa *p2 = NULL;

	struct aa *array[ELEMENT_NUM];

	T bd = buffer_data_create(sizeof(struct aa), ELEMENT_NUM);

	for (int i = 0; i < ELEMENT_NUM; i++)
	{
		array[i] = (struct aa *)buffer_data_calloc(bd);
		if (array[i] == NULL)
		{
			printf("calloc failed %d\n", i);
			return 0;
		}
		array[i]->x = 10;
		array[i]->y = 20;
		array[i]->f = 30.0;
	}

	buffer_data_print_info(bd);

	for (int i = 0; i < ELEMENT_NUM; i++)
	{
		buffer_data_free(bd, (void *)array[i]);
	}
	buffer_data_print_info(bd);

	buffer_data_destroy(&bd);

	return 0;
}

#undef T
#endif
