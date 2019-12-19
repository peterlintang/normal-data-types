
#include <stdlib.h>
#include <assert.h>

#include "array.h"
#include "arrayrep.h"

#define T Array_T

void MODULE_FUN_NAME(ArrayRep, init)(T array, int length, int size, void *ary)
{
	assert(array);
	assert(ary && length > 0 || 0 == length && NULL == ary);
	assert(size > 0);

	array->length = length;
	array->size = size;

	if (length > 0)
		array->array = ary;
	else
		array->array = NULL;
}
