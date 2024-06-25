
/**
 *
 * 生产者消费者问题，多个生产者，一个消费者
 * 使用条件变量
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXITEMS	1000000
#define MAXTHREADS	100

#define min(a, b)	(a) > (b) ? (b) : (a)

int nItems = 0;
int buf[MAXITEMS];

struct {
	pthread_mutex_t mutex;
	int input;
	int inval;

} put = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.input = 0,
	.inval = 0,
};

struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int nready;
} nready = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.cond = PTHREAD_COND_INITIALIZER,
	.nready = 0,
};

static void *producer(void *arg)
{
	for ( ; ; )
	{
		pthread_mutex_lock(&put.mutex);
		if (put.input >= nItems)
		{
			pthread_mutex_unlock(&put.mutex);
			return NULL;
		}
		buf[put.input] = put.inval;
		put.input++;
		put.inval++;
		pthread_mutex_unlock(&put.mutex);

		pthread_mutex_lock(&nready.mutex);
		if (nready.nready == 0)
			pthread_cond_signal(&nready.cond);
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);

		*((int *)(arg)) += 1;
	}
}


static void *consumer(void *arg)
{
	int i;

	for (i = 0; i < nItems; i++)
	{
		pthread_mutex_lock(&nready.mutex);
		while (nready.nready == 0)
			pthread_cond_wait(&nready.cond, &nready.mutex);
		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);

		if (buf[i] != i)
			fprintf(stderr, "buf[%d] = %d\n", i, buf[i]);
	}

	return NULL;
}

static void usage(char *proc)
{
	fprintf(stdout, "usage: %s <#items> <#threads>\n", proc);
	exit(0);
}

int main(int argc, char *argv[])
{
	int i;
	int nthreads;
	int count[MAXTHREADS];
	pthread_t tid_producers[MAXTHREADS];
	pthread_t tid_consumer;

	if (argc != 3)
		usage(argv[0]);

	nItems = min(atoi(argv[1]), MAXITEMS);
	nthreads = min(atoi(argv[2]), MAXTHREADS);

	pthread_setconcurrency(nthreads + 1);

	for (i = 0; i < nthreads; i++)
	{
		count[i] = 0;
		pthread_create(&tid_producers[i], NULL, producer, &count[i]);
	}

	pthread_create(&tid_consumer, NULL, consumer, NULL);

	/*************************************/
	for (i = 0; i < nthreads; i++)
	{
		pthread_join(tid_producers[i], NULL);
		fprintf(stdout, "count[%d] = %d\n", i, count[i]);
	}

	pthread_join(tid_consumer, NULL);

	return 0;
}


