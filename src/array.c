
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "array.h"
#include "arrayrep.h"

#define T Array_T


// < functions 166>
T MODULE_FUN_NAME(Array, new)(int length, int size)
{
	T array = NULL;

	array = (T)calloc(1, sizeof(*array));
	if (array)
	{
		if (length > 0)
		{
			char *tmp = NULL;

			tmp = (char *)calloc(length, size);
			if (tmp)
			{
				MODULE_FUN_NAME(ArrayRep, init)
						(array, length, size, tmp);
			}
			else
			{
				free(array);
				return NULL;
			}
		}
		else
		{
			MODULE_FUN_NAME(ArrayRep, init)
					(array, length, size, NULL);
		}
	}

	return array;
}

void MODULE_FUN_NAME(Array, free)(T *array)
{
	assert(array && *array);

	free((*array)->array);
	free(*array);
	*array = NULL;
}

void *MODULE_FUN_NAME(Array, get)(T array, int i)
{
	assert(array);
	assert(i >= 0 && i < array->length);

	return array->array + i * array->size;
}

void *MODULE_FUN_NAME(Array, put)(T array, int i, void *elem)
{
	assert(array);
	assert(i >= 0 && i < array->length);
	assert(elem);

	memcpy(array->array + i * array->size, elem, array->size);

	return elem;
}

int MODULE_FUN_NAME(Array, length)(T array)
{
	assert(array);

	return array->length;
}

int MODULE_FUN_NAME(Array, size)(T array)
{
	assert(array);

	return array->size;
}

void MODULE_FUN_NAME(Array, resize)(T array, int length)
{
	assert(array);
	assert(length >= 0);

	if (0 == array->length)
	{
		array->array = (char *)calloc(length, array->size);
	}
	else if (length > 0)
	{
		array->array = realloc(array->array, length * array->size);
		if (NULL == array->array)
		{
			// TODO
		}
	}
	else
	{
		free(array->array);
	}

	array->length = length;
}


T MODULE_FUN_NAME(Array, copy)(T array, int length)
{
	T copy = NULL;

	assert(array);
	assert(length >= 0);

	copy = MODULE_FUN_NAME(Array, new)(length, array->size);
	if (copy->length >= array->length && array->length > 0)
	{
		memcpy(copy->array, array->array, array->length * array->size);
	}
	else if (array->length > copy->length && copy->length > 0)
	{
		memcpy(copy->array, array->array, copy->length * copy->size);
	}

	return copy;
}


