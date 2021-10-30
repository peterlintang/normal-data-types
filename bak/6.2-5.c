
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LEFT(i)		((i) * 2)
#define RIGHT(i)	((i) * 2 + 1)
#define PARENT(i)	((i) / 2)

/*
 * try to use xun huan dai ti di gui diao yong 
 * modify the property of max heap
 */
void max_heapify(int a[], int i, int size)
{
	int l;
	int r;
	int largest;
	int tmp;

	l = LEFT(i);
	r = RIGHT(i);

	if (l < size && a[l] > a[i])
	{
		largest = l;
	}
	else
	{
		largest = i;
	}
	
	if (r < size && a[r] > a[largest])
	{
		largest = r;
	}

	if (i != largest)
	{
		tmp = a[i];
		a[i] = a[largest];
		a[largest] = tmp;
		max_heapify(a, largest, size);
	}

}

/*
 *use array to build max heap
 */
void build_max_heap(int a[], int size)
{
	int i;
	for (i = size / 2; i >= 0; i--)
	{
		max_heapify(a, i, size);
	}
}

/*
 * interfaces for max heap
 *
 * */
int max_heap_maxmum(int a[], int size)
{
	return a[0];
}

int max_heap_extract_max(int a[], int *sizep)
{
	int max;
	int size;

	assert(sizep);

	size = *sizep;

	assert(size > 0);

	max = a[0];
	a[0] = a[size - 1];
	*sizep = size - 1;

	max_heapify(a, 0, size - 1);
	return max;
}

int max_heap_increase_key(int a[], int i, int key)
{
	int parent;

	if (a[i] > key)
	{
		return -1;
	}

	a[i] = key;
	parent = i >> 1; //parent = PARENT(i);
	while (a[i] > a[parent] && i > 0)
	{
		a[i] = a[parent];
		a[parent] = key;
		i = parent;
		parent = i >> 1; // parent = PARENT(i);
	}

	return 0;
}

int max_heap_insert(int a[], int *sizep, int key)
{

	a[*sizep] = -589238923;  // fu wu qiong ;
	*sizep += 1;
	max_heap_increase_key(a, *sizep - 1, key);
}

/*
 * sort ..........
 */
void max_heap_sort(int a[], int size)
{
	int tmp;
	build_max_heap(a, size);
	while (size > 1)
	{
		tmp = a[0];
		a[0] = a[size - 1];
		a[size - 1] = tmp;
		size -= 1;
		max_heapify(a, 0, size);
	}
}


int main(int argc, char *argv[])
{
	int a[] = {1,2,8,7,14,4,16,3,9,10};
//	int a[] = {5, 3, 17, 10, 84, 19, 6, 22, 9};
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof(a[0]))

//	build_max_heap(a, ARRAY_SIZE(a));
	max_heap_sort(a, ARRAY_SIZE(a));

	for (int i = 0; i < ARRAY_SIZE(a); i++)
	{
		fprintf(stdout, "a[%d] = %d\n", i, a[i]);
	}

	return 0;
}

