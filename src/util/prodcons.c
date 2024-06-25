
/**
 *
 * 生产者消费者问题，多个生产者，一个消费者
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXITEMS	1000000
#define MAXTHREADS	100

#define min(a, b)	(a) > (b) ? (b) : (a)

int nItems = 0;

struct {
	pthread_mutex_t mutex;
	int buf[MAXITEMS];
	int input;
	int inval;

} shared = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.input = 0,
	.inval = 0,
};

static void *producer(void *arg)
{
	for ( ; ; )
	{
		pthread_mutex_lock(&shared.mutex);

		if (shared.input >= nItems)
		{
			pthread_mutex_unlock(&shared.mutex);
			return NULL;
		}
		shared.buf[shared.input] = shared.inval;
		shared.input++;
		shared.inval++;

		pthread_mutex_unlock(&shared.mutex);

		*((int *)(arg)) += 1;
	}
}

static void *consumer(void *arg)
{
	int i;

	for (i = 0; i < nItems; i++)
	{
		if (shared.buf[i] != i)
			fprintf(stderr, "buf[%d] = %d\n", i, shared.buf[i]);
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

	pthread_setconcurrency(nthreads);

	for (i = 0; i < nthreads; i++)
	{
		count[i] = 0;
		pthread_create(&tid_producers[i], NULL, producer, &count[i]);
	}

	for (i = 0; i < nthreads; i++)
	{
		pthread_join(tid_producers[i], NULL);
		fprintf(stdout, "count[%d] = %d\n", i, count[i]);
	}

	pthread_create(&tid_consumer, NULL, consumer, NULL);
	pthread_join(tid_consumer, NULL);

	return 0;
}


