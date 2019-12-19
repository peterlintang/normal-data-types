
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "seq.h"
#include "array.h"
#include "arrayrep.h"

#define T Seq_T

struct T {
	struct Array_T array;
	int length;
	int head;
};

// < static functions 179 >
static void expand(T seq)
{
	int n = 0;

	n = seq->array.length;

	MODULE_FUN_NAME(Array, resize)(&seq->array, 2 * n);
	if (seq->head > 0)
	{
		void **old = &((void **)seq->array.array)[seq->head];

		memcpy(old + n, old, ((n - seq->head) * sizeof(void *)));
		seq->head += n;
	}
}

// < functions 175 >
T MODULE_FUN_NAME(Seq, new)(int hint)
{
	T seq = NULL;

	assert(hint >= 0);

	seq = (T)calloc(1, sizeof(*seq));
	if (NULL != seq)
	{
		if (0 == hint)
		{
			hint = 16;
		}
		MODULE_FUN_NAME(ArrayRep, init)
				(&seq->array, hint, sizeof(void *), 
					calloc(hint, sizeof(void *)));
		return seq;
	}

	return seq;
}

T MODULE_FUN_NAME(Seq, seq)(void *x, ...)
{
	va_list ap;
	T seq = NULL;

	seq = MODULE_FUN_NAME(Seq, new)(0);
	if (NULL != seq)
	{
		va_start(ap, x);
		for (; x; x = va_arg(ap, void *))
		{
			MODULE_FUN_NAME(Seq, addhi)(seq, x);
		}
		va_end(ap);
	}

	return seq;
}

void MODULE_FUN_NAME(Seq, free)(T *seq)
{
	assert(seq && *seq);
	assert((void *)*seq == (void *)&(*seq)->array);

	MODULE_FUN_NAME(Array, free)((Array_T *)seq);
	// or 
	/*
	Array_T tmp = &(*seq)->array;
	Array_free(&tmp);
	free(*seq);
	*/
	*seq = NULL;
}

int MODULE_FUN_NAME(Seq, length)(T seq)
{
	assert(seq);

	return seq->length;
}

void *MODULE_FUN_NAME(Seq, get)(T seq, int i)
{
	assert(seq);
	assert(i >= 0 && i < seq->length);

	return ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
}

void *MODULE_FUN_NAME(Seq, put)(T seq, int i, void *x)
{
	void *prev = NULL;

	assert(seq);
	assert(i >= 0 && i < seq->length);

	prev = ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
	((void **)seq->array.array)[(seq->head + i) % seq->array.length] = x;

	return prev;
}

void *MODULE_FUN_NAME(Seq, rmhi)(T seq)
{
	int i = 0;

	assert(seq);
	assert(seq->length > 0);

	i == --seq->length;
	return ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
}

void *MODULE_FUN_NAME(Seq, rmlo)(T seq)
{
	int i = 0;
	void *x = NULL;

	assert(seq);
	assert(seq->length > 0);

	x = ((void **)seq->array.array)[(seq->head + i) % seq->array.length];
	seq->head = (seq->head + 1) % seq->array.length;
	--seq->length;

	return x;
}

void *MODULE_FUN_NAME(Seq, addhi)(T seq, void *x)
{
	int i = 0;

	assert(seq);

	if (seq->length == seq->array.length)
	{
		expand(seq);
	}

	i = seq->length++;
	((void **)seq->array.array)[(seq->head + i) % seq->array.length] = x;
	return x;
}

void *MODULE_FUN_NAME(Seq, addlo)(T seq, void *x)
{
	int i = 0;

	assert(seq);

	if (seq->length == seq->array.length)
	{
		expand(seq);
	}

	if (--seq->head < 0)
	{
		seq->head = seq->array.length - 1;
	}

	seq->length++;
	((void **)seq->array.array)[(seq->head + i) % seq->array.length] = x;
	return x;
}



