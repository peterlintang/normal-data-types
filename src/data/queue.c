
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "queue.h"

/*
 * 10-1 
 * implement the queue described by this sector
 */


int queue_empty(struct Queue *q)
{
	assert(q);

	if (q->head == q->tail)
		return 1;
	else
		return 0;
}

int queue_full(struct Queue *q)
{
	assert(q);

	if (((q->tail + 1) % QUEUE_SIZE) == q->head)
		return 1;
	else
		return 0;
}

int queue_en(struct Queue *q, void *x)
{
	assert(q);
	assert(x);

	if (queue_full(q))
	{
		fprintf(stderr, "queue full\n");
		return -1;
	}

	q->q[q->tail] = x;
	if (q->tail == QUEUE_SIZE)
		q->tail = (q->tail + 1) % QUEUE_SIZE;
	else
		q->tail = q->tail + 1;

	return 0;
}

int queue_de(struct Queue *q, void **p)
{
	assert(q);
	assert(p);

	if (queue_empty(q))
	{
		fprintf(stderr, "queue empty\n");
		return -1;
	}

	*p = q->q[q->head];
	if (q->head == QUEUE_SIZE)
		q->head = (q->head + 1) % QUEUE_SIZE;
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




