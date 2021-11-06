
/*
 * 计算数组的最大最小值
 */
#ifndef CI_MINIMUM_H_
#define CI_MINIMUM_H_

/*
 * find the min in array a
 */
void *minimum(void *a[], int size, int (*cmp)(void *, void *));

/*
 * find the max in array a
 */
void *maximum(void *a[], int size, int (*cmp)(void *, void *));

/*
 * find the min and max in array a
 */
int minmax(void *a[], int size, int (*cmp)(void *, void *), void **pmin, void **pmax);

/*
 * find the min and max in array a
 */
int minmax2(void *a[], int size, int (*cmp)(void *, void *), void **pmin, void **pmax);


#endif // end of CI_MINIMUM_H_

