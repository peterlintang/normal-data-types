
/*
 * 数组实现最大堆结构
 */

#ifndef CI_MAX_HEAPIFY_H
#define CI_MAX_HEAPIFY_H

#include "module.h"

#define T MaxHeap_T
typedef struct T *T;

/*
 *use array to build max heap
 */
T MODULE_FUN_NAME(MaxHeap, new)(void *a[], int size, int (*cmp)(void *, void *), int (*increase)(void **, void *));

void MODULE_FUN_NAME(MaxHeap, free)(T *hp, int free_array);

/*
 * interfaces for max heap
 */
void *MODULE_FUN_NAME(MaxHeap, max)(T h);

void *MODULE_FUN_NAME(MaxHeap, extract_max)(T h);

int MODULE_FUN_NAME(MaxHeap, increase_key)(T h, int i, void *new_key);

int MODULE_FUN_NAME(MaxHeap, insert)(T h, void *new_key);

void *MODULE_FUN_NAME(MaxHeap, delete)(T h, int index);

#undef T

#endif // end of CI_MAX_HEAPIFY_H


