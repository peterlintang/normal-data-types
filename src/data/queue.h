

/*
 * 10-1 
 * implement the queue described by this sector
 * queue中有一个位置需要空出来作为标志，是否为满，空
 */
#ifndef CI_QUEUE_H__
#define CI_QUEUE_H__

#include "module.h"

#define T Queue_T

typedef struct T *T;


/*
 * 创建一个队列,返回队列指针，失败返回空指针
 * size: 队列大小
 */
T MODULE_FUN_NAME(Queue, new)(int size);

/*
 * 销毁一个队列,
 * qp:	指向队列的指针
 */
void MODULE_FUN_NAME(Queue, free)(T *qp);

/*
 * 判断队列是否为空，是返回1，不是返回0,
 * qp:	指向队列的指针
 */
int MODULE_FUN_NAME(Queue, isEmpty)(T q);

/*
 * 判断队列是否满，是返回1，不是返回0,
 * qp:	指向队列的指针
 */
int MODULE_FUN_NAME(Queue, isFull)(T q);

/*
 * 往队列压元素x，成功返回0，失败返回-1
 * qp:	指向队列的指针
 * x:	待压送数据
 */
int MODULE_FUN_NAME(Queue, en)(T q, void *x);

/*
 * 从队列压元素对头，成功返回0，失败返回-1
 * qp:	指向队列的指针
 * p:	取回的数据存放的地方
 */
int MODULE_FUN_NAME(Queue, de)(T q, void **p);

#undef T

#endif	// end of MY_QUEUE_H__


