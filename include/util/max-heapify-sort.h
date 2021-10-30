
/*
 * 最大堆排序
 */

#ifndef MAX_HEAPIFY_SORT_H
#define MAX_HEAPIFY_SORT_H

#include "max-heapify.h"

/*
 * 堆排序
 * a: 指针数组，待排序
 * size: 指针数组a的大小，
 * cmp:比较函数，第一个参数大于第二个参数返回1，小于返回-1，等于返回0
 */
int max_heap_sort(void *a[], int size, int (*cmp)(void *, void *));


#endif // end of MAX_HEAPIFY_SORT_H

