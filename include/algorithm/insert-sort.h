
/*
 * 插入排序
 */
#ifndef CI_INSERT_SORT_H
#define CI_INSERT_SORT_H
#include <stdio.h>
#include <stdlib.h>


/*
 * 插入排序,返回0
 * in: 指针数组
 * cmp: 比较函数，第一个参数大于第二个参数，返回1，小于返回-1，等于返回0
 * p: 数组第一个元素的下标
 * r: 数组最后一个元素的下标
 */
int insert_sort(void *in[], int (*cmp)(void *, void *), int p, int r);

#endif // end of CI_INSERT_SORT_H


