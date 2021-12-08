
/*
 * 数组实现最小堆结构
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "min-heapify.h"

#define LEFT(i)		((i) * 2)
#define RIGHT(i)	((i) * 2 + 1)
#define PARENT(i)	((i) / 2)

#define T MinHeap_T

struct T {
	void **a;
	int size;		// 堆中有多少个元素
	int total;		// 数组a的大小
	int (*cmp)(void *, void *);
	int (*decrease)(void **, void *);
};

/*
 * modify the property of min heap
 */
static void min_heapify(void *a[], int i, int size, int (*cmp)(void *, void *))
{
	int l;
	int r;
	int minimum = 0;
	void *tmp = NULL;

	l = LEFT(i);
	r = RIGHT(i);

	if (l <= size && cmp(a[l], a[i]) < 0)
	{
		minimum = l;
	}
	else
	{
		minimum = i;
	}
	
	if (r <= size && cmp(a[r], a[minimum]) < 0)
	{
		minimum = r;
	}

	if (i != minimum)
	{
		tmp = a[i];
		a[i] = a[minimum];
		a[minimum] = tmp;
		min_heapify(a, minimum, size, cmp);
	}

}

/*
 *use array to build min heap
 */
T MODULE_FUN_NAME(MinHeap, new)(void *a[], int size, int (*cmp)(void *, void *), int (*decrease)(void **, void *))
{
	T h = NULL;

	assert(a);
	assert(size > 0);
	assert(cmp);
	assert(decrease);

	h = (T)calloc(1, sizeof(*h));
	if (h != NULL)
	{
		int i;
		for (i = size / 2; i >= 0; i--)
		{
			min_heapify(a, i, size - 1, cmp);
		}

		h->a = a;
		h->size = size;
		h->total = size;
		h->cmp = cmp;
		h->decrease = decrease;
	}

	return h;
}

void MODULE_FUN_NAME(MinHeap, free)(T *hp, int free_array)
{
	assert(hp && (*hp));

	if (free_array) free((*hp)->a);

	free(*hp);
	*hp = NULL;
}

void *MODULE_FUN_NAME(MinHeap, min)(T h)
{
	assert(h);

	if(h->size <= 0) 
	{
		return NULL;
	}

	void **a = h->a;

	return a[0];
}

void *MODULE_FUN_NAME(MinHeap, extract_min)(T h)
{
	assert(h);

	void *min = NULL;
	void **a = NULL;

	if(h->size <= 0) 
	{
//		fprintf(stdout, "%s: size: %d\n", __func__, h->size);
		return NULL;
	}

	a = h->a;

	min = a[0];
	a[0] = a[h->size - 1];
	h->size = h->size - 1;

	min_heapify(a, 0, h->size - 1, h->cmp);
	return min;
}

int MODULE_FUN_NAME(MinHeap, decrease_key)(T h, int i, void *new_key)
{
	int parent;
	void **a = NULL;
	void *tmp = NULL;

	assert(h);
	assert(new_key);
	
	if (i < 0 || i > h->size - 1)
	{
		return -2;
	}

	a = h->a;

	if (h->cmp(a[i], new_key) < 0)
	{
		return -1;
	}

	h->decrease(&(a[i]), new_key);

	parent = i >> 1; //parent = PARENT(i);
	while ((h->cmp(a[i], a[parent]) < 0) && i > 0)
	{
		tmp = a[i];
		a[i] = a[parent];
		a[parent] = tmp;
		i = parent;
		parent = i >> 1; // parent = PARENT(i);
	}

	return 0;
}

int MODULE_FUN_NAME(MinHeap, insert)(T h, void *new_key)
{
	assert(h && new_key);

	int parent = 0;
	int i = 0;
	void *tmp = NULL;
	void ** a = h->a;

	if (h->size == h->total)
	{
		return -1;
	}

	a[h->size] = new_key; // 特殊标志，空指针表示最小值,不可能存在堆中
	h->size += 1;

	i = h->size - 1;
	parent = i >> 1; //parent = PARENT(i);
	while ((h->cmp(a[i], a[parent]) < 0) && i > 0)
	{
		tmp = a[i];
		a[i] = a[parent];
		a[parent] = tmp;
		i = parent;
		parent = i >> 1; // parent = PARENT(i);
	}

	return 0;
}

void *MODULE_FUN_NAME(MinHeap, delete)(T h, int index)
{
	assert(h);

	void *tmp = NULL;
	void **a = h->a;

	if (index < 0 || index > h->size - 1)
	{
		return NULL;
	}

	tmp = a[h->size - 1];
	a[h->size - 1] = a[index];
	a[index] = tmp;

	if (h->cmp(a[index], a[PARENT(index)]) < 0)
	{
		while (index > 0 && h->cmp(a[index], a[PARENT(index)]) < 0)
		{
			tmp = (a[index]);
			a[index] = a[PARENT(index)];
			a[PARENT(index)] = tmp;
			index = PARENT(index);
		}
	}
	else
	{
		min_heapify(a, index, h->size - 1, h->cmp);
	}

	tmp = a[h->size - 1];
	a[h->size - 1] = NULL;
	h->size -= 1;

	return tmp;
}

