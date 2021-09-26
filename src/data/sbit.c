

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bit2.h"


#define T SBit_T

struct T {
	unsigned char *bits;
	unsigned int num;
};

T MODULE_FUN_NAME(sbit, create)(unsigned int num)
{
	T bits = NULL;

	bits = (T)calloc(1, sizeof(*bits));
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

int MODULE_FUN_NAME(sbit, destroy)(T *sb)
{
	free((*sb)->bits);
	free(*sb);
	*sb = NULL;
	return 0;
}

int MODULE_FUN_NAME(sbit, set)(T sb, unsigned int index)
{
	int offset = 0;

	assert(((sb != NULL) || (index < sb->num)));

	offset = index % 8;
	sb->bits[index >> 3] |= 1 << offset;

	return 1;
}

int MODULE_FUN_NAME(sbit, clear)(T sb, unsigned int index)
{
	int offset = 0;

	assert(((sb != NULL) || (index < sb->num)));

	offset = index % 8;
	sb->bits[index >> 3] &= ~(1 << offset);

	return 1;
}

int MODULE_FUN_NAME(sbit, get)(T sb, unsigned int index)
{
	int offset = 0;

	assert(((sb != NULL) || (index < sb->num)));

	offset = index % 8;
	return ((sb->bits[index >> 3]) >> offset) & 1;
}



/***************************test*************************/
/*
int main(int argc, char *argv[])
{
	int num = atoi(argv[1]);
	T bits = MODULE_FUN_NAME(sbit, create)(num);
	if (bits == NULL)
	{
		fprintf(stderr, "create bit failed\n");
		exit(0);
	}
	fprintf(stdout, "bits->num: %u, %u, %u\n", bits->num, bits->num >> 3, bits->num & 0x7);
	for (int i = 0; i < bits->num; i++)
	{
		MODULE_FUN_NAME(sbit, set)(bits, i);
		if (MODULE_FUN_NAME(sbit, get)(bits, i) == 0)
			fprintf(stderr, "i: %d, failed to set\n", i);
//		fprintf(stdout, "i: %d: %d\n", i, MODULE_FUN_NAME(sbit, get)(bits, i));
		MODULE_FUN_NAME(sbit, clear)(bits, i);
		if (MODULE_FUN_NAME(sbit, get)(bits, i) != 0)
			fprintf(stderr, "i: %d, failed to clear\n", i);
//		fprintf(stdout, "i: %d: %d\n", i, MODULE_FUN_NAME(sbit, get)(bits, i));
	}

	for (int i = 1024; i < 1038; i++)
	{
		MODULE_FUN_NAME(sbit, set)(bits, i);
	}

	for (int i = 1024 >> 3; i < (1038 >> 3) + 1; i++)
	{
		fprintf(stdout, "%x ", bits->bits[i]);
	}
	fprintf(stdout, "\n");

	for (int i = 1024; i < 1027; i++)
	{
		MODULE_FUN_NAME(sbit, clear)(bits, i);
	}

	for (int i = 1024 >> 3; i < (1038 >> 3) + 1; i++)
	{
		fprintf(stdout, "%x ", bits->bits[i]);
	}
	fprintf(stdout, "%d %d\n", MODULE_FUN_NAME(sbit, get)(bits, 1028), MODULE_FUN_NAME(sbit, get)(bits, 1039));

//	fprintf(stdout, "destroy bits\n");
	MODULE_FUN_NAME(sbit, destroy)(&bits);

	return 0;
}
*/






