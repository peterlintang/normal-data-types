

/*
 * 10-1 
 * implement the queue described by this sector
 */
#ifndef MY_QUEUE_H_
#define MY_QUEUE_H_

#define QUEUE_SIZE	1024

struct Queue {
	void *q[QUEUE_SIZE];
	int head;
	int tail;
};

int queue_empty(struct Queue *q);
int queue_full(struct Queue *q);
int queue_en(struct Queue *q, void *x);
int queue_de(struct Queue *q, void **p);

#endif	// end of MY_QUEUE_H_


