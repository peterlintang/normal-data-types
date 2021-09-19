

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bit.h"

struct bit_t {
	unsigned char *bits;
	unsigned int num;
};

struct bit_t *bit_create(unsigned int num)
{
	struct bit_t *bits = NULL;

	bits = (struct bit_t *)calloc(1, sizeof(*bits));
	if (bits)
	{
		bits->bits = (unsigned char *)calloc( ((num >> 3) + (num & 0x7 ? 1 : 0)) , sizeof(unsigned char)); // num >> 3 + 1 should modify
		if (bits->bits == NULL)
		{
			free(bits);
			return NULL;
		}
		bits->num = num;
	}
	return bits;
}

int bit_destroy(struct bit_t **bits)
{
	free((*bits)->bits);
	free(*bits);
	*bits = NULL;
	return 0;
}

int bit_set(struct bit_t *bits, unsigned int index)
{
	int offset = 0;

	assert(((bits != NULL) || (index < bits->num)));

	offset = index % 8;
	bits->bits[index >> 3] |= 1 << offset;

	return 1;
}

int bit_clear(struct bit_t *bits, unsigned int index)
{
	int offset = 0;

	assert(((bits != NULL) || (index < bits->num)));

	offset = index % 8;
	bits->bits[index >> 3] &= ~(1 << offset);

	return 1;
}

int bit_get(struct bit_t *bits, unsigned int index)
{
	int offset = 0;

	assert(((bits != NULL) || (index < bits->num)));

	offset = index % 8;
	return ((bits->bits[index >> 3]) >> offset) & 1;
}



/***************************test*************************/
int main(int argc, char *argv[])
{
	int num = atoi(argv[1]);
	struct bit_t *bits = bit_create(num);
	if (bits == NULL)
	{
		fprintf(stderr, "create bit failed\n");
		exit(0);
	}
	fprintf(stdout, "bits->num: %u, %u, %u\n", bits->num, bits->num >> 3, bits->num & 0x7);
	for (int i = 0; i < bits->num; i++)
	{
		bit_set(bits, i);
		if (bit_get(bits, i) == 0)
			fprintf(stderr, "i: %d, failed to set\n", i);
//		fprintf(stdout, "i: %d: %d\n", i, bit_get(bits, i));
		bit_clear(bits, i);
		if (bit_get(bits, i) != 0)
			fprintf(stderr, "i: %d, failed to clear\n", i);
//		fprintf(stdout, "i: %d: %d\n", i, bit_get(bits, i));
	}

	for (int i = 1024; i < 1038; i++)
	{
		bit_set(bits, i);
	}

	for (int i = 1024 >> 3; i < (1038 >> 3) + 1; i++)
	{
		fprintf(stdout, "%x ", bits->bits[i]);
	}
	fprintf(stdout, "\n");

	for (int i = 1024; i < 1027; i++)
	{
		bit_clear(bits, i);
	}

	for (int i = 1024 >> 3; i < (1038 >> 3) + 1; i++)
	{
		fprintf(stdout, "%x ", bits->bits[i]);
	}
	fprintf(stdout, "%d %d\n", bit_get(bits, 1028), bit_get(bits, 1039));

//	fprintf(stdout, "destroy bits\n");
	bit_destroy(&bits);

	return 0;
}




