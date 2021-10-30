
/*
 * 数组实现最大堆结构
 */

#ifndef CI_MAX_HEAPIFY_H
#define CI_MAX_HEAPIFY_H

#include "module.h"

#define T MinHeap_T
typedef struct T *T;

/*
 *use array to build max heap
 */
T MODULE_FUN_NAME(MinHeap, new)(void *a[], int size, int (*cmp)(void *, void *), int (*decrease)(void **, void *));

void MODULE_FUN_NAME(MinHeap, free)(T *hp, int free_array);

/*
 * interfaces for max heap
 */
void *MODULE_FUN_NAME(MinHeap, max)(T h);

void *MODULE_FUN_NAME(MinHeap, extract_max)(T h);

int MODULE_FUN_NAME(MinHeap, decrease_key)(T h, int i, void *new_key);

int MODULE_FUN_NAME(MinHeap, insert)(T h, void *new_key);

void *MODULE_FUN_NAME(MinHeap, delete)(T h, int index);

#undef T

#endif // end of CI_MAX_HEAPIFY_H


