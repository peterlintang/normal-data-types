
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "buffer_data.h"

#define T buffer_data_t

struct T {
	unsigned int *map;
	int map_size;
	int total_elements;
	int element_size;
	void *elements;
};

T buffer_data_create(int element_size, int total_elements)
{
	T bd = (T)calloc(1, sizeof(*bd));
	if (bd)
	{
		bd->total_elements = total_elements;
		bd->element_size = element_size;
		bd->elements = (void *)calloc(total_elements, element_size);
		if (bd->elements == NULL)
		{
			free(bd);
			return NULL;
		}
		int map_size = total_elements / (8 * sizeof(unsigned int)) 
			+ (total_elements % (8 * sizeof(unsigned int)) == 0 ? 0 : 1);
		bd->map = (unsigned int *)calloc(map_size, sizeof(unsigned int));
		if (bd->map == NULL)
		{
			free(bd->elements);
			free(bd);
			return NULL;
		}
		bd->map_size = map_size;
	}
	return bd;
}

void buffer_data_destroy(T *bp)
{
	T bd = *bp;

	assert(bp);
	assert(bd);

	free(bd->elements);
	free(bd->map);
	free(bd);
	*bp = NULL;
}

void *buffer_data_calloc(T b)
{
	assert(b);

	unsigned int i;
	unsigned int j;
	unsigned int index = -1;

	for (i = 0; i < b->map_size; i++)
	{
		if (b->map[i] + 1 != 0)
			break;
	}

	if (i >= b->map_size)
	{
		return NULL;
	}

	for (j = 0; j < sizeof(unsigned int) * 8; j++)
	{
		if ((b->map[i] & (0x1 << j)) == 0)
			break;
	}

	if (j >= sizeof(unsigned int) * 8)
	{
		return NULL;
	}

	index = i * sizeof(unsigned int) * 8 + j;
	if (index >= b->total_elements)
	{
		return NULL;
	}

	b->map[i] = b->map[i] + (0x1 << j);

	char *p = (char *)b->elements;
	char *np = p + index * b->element_size;
	void *rp = (void *)np;

	return rp;
}

void buffer_data_free(T b, void *p)
{
	char *p1 = (char *)p;
	char *p2 = (char *)b->elements;
	unsigned int index = (p1 - p2) / b->element_size;

	assert(index >= 0 && index < b->total_elements);

	unsigned int i;
	unsigned int j;

	i = index / (sizeof(unsigned int) * 8);
	j = index % (sizeof(unsigned int) * 8);
	b->map[i] = b->map[i] & (~(0x1 << j));
}

void buffer_data_print_info(T b)
{
	unsigned int i;

	for (i = 0; i < b->map_size; i++)
	{
		printf("0x%0x ", b->map[i]);
		if ((i + 1) % 8 == 0)
			printf("\n");
	}
	if ((i + 1) % 8 != 0)
		printf("\n");
}

#undef T

#if 1
#define T buffer_data_t

#define ELEMENT_NUM	100000
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
