
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LEFT(i)		((i) * 2)
#define RIGHT(i)	((i) * 2 + 1)
#define PARENT(i)	((i) / 2)

/*
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
 * you wen ti, luo ji bu dui
 * zuo wei li zi liu zhe,
 * ru shan chu 
 * 						x1
 * 			x2						x3
 * 	x4				x5		x6				x7	
 * 	shan chu x2 ke neng xing cheng ru xia 
 * 						x1
 * 			x4						x3
 * 					x5		x6				x7	
 * 	bu neng wei chi dui de xing zhi 
 */
int max_heap_delete(int a[], int *psize, int index)
{
	assert(index >= 0 && index < *psize);

	int size = *psize;
	int left = -1;
	int right = -1;
	int tmp = 0;
	int tmp_index = index;
	int max = -589238923;

	tmp = a[index];
	left = LEFT(index);
	right = RIGHT(index);

	if (left < size)
	{
		max = a[left];
		index = left;
	}
	if (right < size && max < a[right])
	{
		max = a[right];
		index = right;
	}

	if (left < size || right < size)
	{
		a[tmp_index] = max;
		max_heap_delete(a, psize, index);
	}
	else
	{
		// dui da xiao jian yi
		for (int i = index; i < *psize; i++)
		{
			a[i] = a[i + 1];
		}
		*psize -= 1;
	}

	return 0;
}

int max_heap_delete2(int a[], int *psize, int index)
{
	int size = *psize;
	int tmp;

	tmp = a[size - 1];
	a[size - 1] = a[index];
	a[index] = tmp;

	if (a[index] > a[PARENT(index)])
	{
		int tmp;
		while (index > 0 && a[index] > a[PARENT(index)])
		{
			tmp = a[index];
			a[index] = a[PARENT(index)];
			a[PARENT(index)] = tmp;
			index = PARENT(index);
		}
	}
	else
	{
		max_heapify(a, index, size - 1);
	}

	a[size - 1] = -1111111;
	*psize -= 1;
	return 0;
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
	int *a = NULL;
	int size = 16;
	int seed = 31;
	int del = 3;

	seed = atoi(argv[2]);
	srand(seed);

	del = atoi(argv[3]);

	size = atoi(argv[1]);
	a = calloc(size, sizeof(int));

	for (int i = 0; i < size; i++)
	{
		a[i] = rand() % size;
	}

	build_max_heap(a, size);
//	max_heap_sort(a, size);

	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %d\n", i, a[i]);
	}
	fprintf(stdout, "elements : %d\n", size);

	fprintf(stdout, "delete element : %d\n", del);
	max_heap_delete2(a, &size, del);
	for (int i = 0; i < size; i++)
	{
		fprintf(stdout, "a[%d] = %d\n", i, a[i]);
	}
	fprintf(stdout, "elements : %d\n", size);

	return 0;
}

