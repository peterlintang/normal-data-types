
/*
 * 实现快速排序，随机快速排序两个排序算法
 *
 */
#ifndef CI_QUICK_SORT_H
#define CI_QUICK_SORT_H

/*
 * 快速排序返回0
 * a: 指针数组
 * cmp: 比较函数第一个参数大于第二个参数返回1，小于返回-1，等于返回0
 * p: 数组第一个元素下标
 * r: 数组最后一个元素下标
 */
int quicksort(void *a[], int (*cmp)(void *, void *), int p, int r);

/*
 * 随机快速排序返回0
 * a: 指针数组
 * cmp: 比较函数第一个参数大于第二个参数返回1，小于返回-1，等于返回0
 * p: 数组第一个元素下标
 * r: 数组最后一个元素下标
 */
int random_quicksort(void *a[], int (*cmp)(void *, void *), int p, int r);

/*
 */
int tail_quicksort(void *a[], int (*cmp)(void *, void *), int p, int r);

#endif // end of CI_QUICK_SORT_H

