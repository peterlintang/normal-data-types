
/*
 * 最小堆排序
 */
#ifndef CI_MIN_HEAPIFY_SORT_H
#define CI_MIN_HEAPIFY_SORT_H

/*
 * 最小堆排序
 * a: 数组指针,
 * size: 数组a大小
 * cmp: 比较函数,第一个参数大于第二个参数,返回1，小于返回-1，等于返回0
 */
int min_heap_sort(void *a[], int size, int (*cmp)(void *, void *));

#endif // end of CI_MIN_HEAPIFY_SORT_H

