
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "array.h"
#include "arrayrep.h"

#define T Array_T


/*
 * name: MODULE_FUN_NAME(Array, new)
 * description: create a new array, which have @length elements, 
 *      and eache element has @size byte mem space
 * return value: return the pointer to array
 * args: @length: number of elements in array
 *       @size: element's size in array
 */
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

/*
 * name: MODULE_FUN_NAME(Array, free)
 * description: free the allocated array
 * return value: return void
 * args: @array: pointer's pointer to array
 */
void MODULE_FUN_NAME(Array, free)(T *array)
{
	assert(array && *array);

	free((*array)->array);
	free(*array);
	*array = NULL;
}

/*
 * name: MODULE_FUN_NAME(Array, get)
 * description: get the index @i of elements in @array
 * return value: return the pointer of element @i in @array
 * args: @array: pointer to array
 * 		 @i: index in array
 */
void *MODULE_FUN_NAME(Array, get)(T array, int i)
{
	assert(array);
	assert(i >= 0 && i < array->length);

	return array->array + i * array->size;
}

/*
 * name: MODULE_FUN_NAME(Array, put)
 * description: put @elem to the index @i of elements in @array
 * return value: return the pointer of element @i in @array (just @elem)
 * args: @array: pointer to array
 * 		 @i: index in array
 * 		 @elem: data to put into @i element in @array
 */
void *MODULE_FUN_NAME(Array, put)(T array, int i, void *elem)
{
	assert(array);
	assert(i >= 0 && i < array->length);
	assert(elem);

	memcpy(array->array + i * array->size, elem, array->size);

	return elem;
}

/*
 * name: MODULE_FUN_NAME(Array, length)
 * description: return the length of elements 
 * 				(how many elemtns)in @array
 * return value: return the length of elements
 * args: @array: pointer to array
 */
int MODULE_FUN_NAME(Array, length)(T array)
{
	assert(array);

	return array->length;
}

/*
 * name: MODULE_FUN_NAME(Array, size)
 * description: return the size of elements 
 * 				(one element's mem space) in @array
 * return value: return element's size
 * args: @array: pointer to array
 */
int MODULE_FUN_NAME(Array, size)(T array)
{
	assert(array);

	return array->size;
}

/*
 * name: MODULE_FUN_NAME(Array, resize)
 * description: resize @array to new elements' length to @length
 * return value: return void
 * args: @array: pointer to array
 * 		 @length: new length of @array
 */
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


/*
 * name: MODULE_FUN_NAME(Array, copy)
 * description: copy @array, if @length is less than @array,
 * 			only copy @length elements
 * return value: return new copied array
 * args: @array: pointer to array
 * 		 @length: elements to copy
 */
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


