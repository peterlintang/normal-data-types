
#ifndef CI_FIB_HEAP_HEADER
#define CI_FIB_HEAP_HEADER

#include "module.h"

#define T FibHeap_T
#define NODE FibHeap_Node

typedef struct T *T;
typedef struct NODE *NODE;

void *MODULE_FUN_NAME(FibHeap, NodePriv)(NODE node);

NODE MODULE_FUN_NAME(FibHeap, NodeCreate)(void *key);

void MODULE_FUN_NAME(FibHeap, NodeFree)(NODE *pnode);

/*
 * 创建一个空fibheap
 * cmp:			比较函数
 * infinite:		不在集合中的元素，且比集合中元素最小的值要小,调用delete时需要用到
 */
T MODULE_FUN_NAME(FibHeap, new)(int cmp(void *, void *), void *infinite);

/*
 * 释放fibheap
 * hp:			指针
 */
void MODULE_FUN_NAME(FibHeap, free)(T *hp);

/*
 * 将@x插入堆@h中，返回
 * h:			fib堆	
 * x:			待插入元素
 */
NODE MODULE_FUN_NAME(FibHeap, insert)(T h, NODE x);

/*
 * @h不为空,取@h中的最小元素; 为空返回NULL
 * h:			fib堆	
 */
NODE MODULE_FUN_NAME(FibHeap, minmum)(T h);

/*
 * @h不为空,取@h中的最小元素;并将元素从@h删除; 为空返回NULL
 * h:			fib堆	
 */
NODE MODULE_FUN_NAME(FibHeap, extractMin)(T h);

/*
 * 合并h1与h2，并返回新的fibheap，失败返回NULL
 * h1:			待合并的fib堆	(h1的内容已被释放，不可再访问)
 * h2:			待合并的fib堆	(h2的内容已被释放，不可再访问)
 */
T MODULE_FUN_NAME(FibHeap, union)(T *h1p, T *h2p);

/*
 * fib堆@h中的元素@x的关键值置为k,返回旧值的指针
 * h:			fib堆
 * x:			堆中元素
 * k:			新值指针
 */
void *MODULE_FUN_NAME(FibHeap, decreaseKey)(T h, NODE x, void *k);

/*
 * 删除fib堆@h中的元素@x，返回删除的x指针
 * h:			fib堆
 * x:			堆中元素
 * infinite:	比元素集合的最小值还要小
 */
NODE MODULE_FUN_NAME(FibHeap, delete)(T h, NODE x);

void MODULE_FUN_NAME(FibHeap, print)(T h);

#undef T
#undef NODE


#endif // end of CI_FIB_HEAP_HEADER


