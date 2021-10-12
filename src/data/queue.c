
/*
 * 10-1 
 * implement the queue described by this sector
 */

#include <stdlib.h>
#include <assert.h>

#include "queue.h"


#define T Queue_T

struct Queue_T {
	int head;
	int tail;
	int size;
	void **array;
};

/*
 * 创建一个队列,返回队列指针，失败返回空指针
 * size: 队列大小
 */
T MODULE_FUN_NAME(Queue, new)(int size)
{
	T q = NULL;

	assert(size > 0);

	q = (T)calloc(1, sizeof(*q));
	if (q)
	{
		q->array = (void **)calloc(1, size * sizeof(void *));
		if (q->array == NULL)
		{
			free(q);
			q = NULL;
		}
		else
		{
			q->size = size;
		}
	}

	return q;
}

/*
 * 销毁一个队列,
 * qp:	指向队列的指针
 */
void MODULE_FUN_NAME(Queue, free)(T *qp)
{
	assert(qp && (*qp));

//	if ((*qp)->head != (*qp)->tail)
			// not empty

	free(*qp);
	*qp = NULL;
}

/*
 * 判断队列是否为空，是返回1，不是返回0,
 * qp:	指向队列的指针
 */
int MODULE_FUN_NAME(Queue, isEmpty)(T q)
{
	assert(q);

	if (q->head == q->tail)
		return 1;
	else
		return 0;
}

/*
 * 判断队列是否满，是返回1，不是返回0,
 * qp:	指向队列的指针
 */
int MODULE_FUN_NAME(Queue, isFull)(T q)
{
	assert(q);

	if (((q->tail + 1) % q->size) == q->head)
		return 1;
	else
		return 0;
}

/*
 * 往队列压元素x，成功返回0，失败返回-1
 * qp:	指向队列的指针
 * x:	待压送数据
 */
int MODULE_FUN_NAME(Queue, en)(T q, void *x)
{
	assert(q);

	if (MODULE_FUN_NAME(Queue, isFull)(q))
	{
		return -1;
	}

	q->array[q->tail] = x;
	if (q->tail == q->size)
		q->tail = (q->tail + 1) % q->size;
	else
		q->tail = q->tail + 1;

	return 0;
}

/*
 * 从队列压元素对头，成功返回0，失败返回-1
 * qp:	指向队列的指针
 * p:	取回的数据存放的地方
 */
int MODULE_FUN_NAME(Queue, de)(T q, void **p)
{
	assert(q);
	assert(p);

	if (MODULE_FUN_NAME(Queue, isEmpty)(q))
	{
		return -1;
	}

	*p = q->array[q->head];
	if (q->head == q->size)
		q->head = (q->head + 1) % q->size;
	else
		q->head = q->head + 1;

	return 0;
}


/*
 * test code
int main(int argc, char *argv[])
{
	struct Queue *q = NULL;
	int ret = 0;
	int value = 0;
	int *pvalue = NULL;

	q = (struct Queue *)calloc(1, sizeof(*q));
	if (q == NULL)
	{
		fprintf(stderr, "no mem\n");
		return -1;
	}

	for (int i = 1; i < QUEUE_SIZE; i++)
	{
		queue_en(q, (void *)i);
	}

	fprintf(stdout, "de queue\n");
	pvalue = &value;
	for (int i = 1; i < QUEUE_SIZE; i++)
	{
		ret = queue_de(q, &pvalue);
		fprintf(stdout, "i: %d, s: %d, pvalue: %d, value: %d\n", 
						i, ret, (int )pvalue, value);
	}

	return 0;
}
 */


