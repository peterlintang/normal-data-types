
/*
 * 实现快速排序，随机快速排序两个排序算法
 *
 */
#ifndef CI_QUICK_SORT_H
#define CI_QUICK_SORT_H

int quicksort(void *a[], int (*cmp)(void *, void *), int p, int r);

int random_quicksort(void *a[], int (*cmp)(void *, void *), int p, int r);


#endif // end of CI_QUICK_SORT_H

