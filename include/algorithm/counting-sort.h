
/*
 * counting 排序, 对数组的数据大小有要求
 */
#ifndef CI_COUNTING_SORT_H
#define CI_COUNTING_SORT_H


/*
 * assume that all elements' value in a is between 0 to k
 * and all the elements are diffrent;
 */
int counting_sort(int a[], int b[], int size);

/*
 * assume that all elements' value in a is between 0 to k
 */
int counting_sort2(int a[], int b[], int size, int k);

int counting_sort3(int a[], int b[], int size, int k);

int counting_sort4(int a[], int size, int k);


#endif // end of CI_COUNTING_SORT_H

