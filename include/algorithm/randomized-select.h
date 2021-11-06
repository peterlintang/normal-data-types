
/*
 * 随机选择，在一个数组中，随机选择一个指定的下标元素，其返回值等于指定的下标元素
 */
#ifndef RANDOMIZED_SELECT_H_
#define RANDOMIZED_SELECT_H_


/*
 * choose the @i index of elements in array between p and r
 * 下标从0开始
 * a: 指针数组
 * cmp: 比较函数
 * p: 数组开始元素下标
 * r: 数组最后一个元素下标
 * i: 查找的指定下标
 */
void *randomized_select(void *a[], int (*cmp)(void *, void *), int p, int r, int i);

#endif // end of RANDOMIZED_SELECT_H_


