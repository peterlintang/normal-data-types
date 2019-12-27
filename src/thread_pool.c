/*************************************
 *
 * filename:	thread_pool.c
 * description:	implement the thread pool operations
 * author:
 * date:		2019-12-26
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

#include "queue_link.h"

#include "thread_pool.h"

/* threads defines */
#define THREADS_MAX	20000
#define COND_ATTR_T	pthread_condattr_t
#define ATTR_T		pthread_attr_t
#define MUTEX_T 	pthread_mutex_t
#define COND_T 		pthread_cond_t

#define MUTEX_INIT(mutex)		pthread_mutex_init(mutex, NULL)
#define MUTEX_DESTROY(mutex)	pthread_mutex_destroy(mutex)
#define MUTEX_LOCK(mutex)		pthread_mutex_lock(mutex)
#define MUTEX_UNLOCK(mutex)		pthread_mutex_unlock(mutex)

#define COND_INIT(cond, attr)		pthread_cond_init(cond, attr)
#define COND_DESTROY(cond)			pthread_cond_destroy(cond)
#define COND_WAIT(cond, mutex)		pthread_cond_wait(cond, mutex)
#define COND_SIGNAL(cond)			pthread_cond_signal(cond)
#define COND_BROADCAST(cond)		pthread_cond_broadcast(cond)

/**********************************************************/
/* defines */
#define QUEUE	Queue_T
#define T 		ThreadPool_T

#define TASK	Task_T
typedef struct TASK *TASK;

struct TASK {
	MODULE_FUN_NAME(ThreadPool, Cb_T) cb;
	void *cl;
};

struct T {
	int size;		// record how many threads in this pool

	MUTEX_T	mutex;	// protect task queue
	COND_T	cond;	// protect task queue
	QUEUE	queue;	// task queue; TODO we now use queue_link to implement


};

/*
 * local routines
 */
static void thread_exit_handler(void *p);
static void *thread_cycle(void *pirv);

/*
 * name: MODULE_FUN_NAME(ThreadPool, new)
 * description: create a thread pool
 * return value: return the pointer to thread pool
 * args: size: thread pool size
 */
T MODULE_FUN_NAME(ThreadPool, new)(int size)
{
	T p = NULL;

	assert(size > 0);
	assert(size <= THREADS_MAX);

	p = (T)calloc(1, sizeof(*p));
	if (NULL == p)
	{
		return NULL;
	}

	p->size = size;
	p->queue = MODULE_FUN_NAME(Queue, new)(0);
	if (NULL == p->queue)
	{
		free(p);
		return NULL;
	}

	return p;
}

/*
 * name: MODULE_FUN_NAME(ThreadPool, free)
 * description: free the thread pool;
 * TODO: make sure queue is empty before call free
 * return value: return void
 * args: @p: pointer's pointer to thread pool
 */
void MODULE_FUN_NAME(ThreadPool, free)(T *p)
{
	assert(p);
	assert(*p);

	MODULE_FUN_NAME(Queue, free)(&(*p)->queue);

	free(*p);
	*p = NULL;
}

/*
 * name: MODULE_FUN_NAME(ThreadPool, init)
 * description: init a thread pool, create threads for the pool
 * return value: 0: success; 
 * 				<0 : error;
 * args: p: pointer to pool 
 */
int MODULE_FUN_NAME(ThreadPool, init)(T p)
{
	int i;
	int ret = 0;
	pthread_attr_t attr;
	pthread_t tid;

	assert(p);
	assert(p->size > 0);
	assert(p->size <= THREADS_MAX);

	COND_INIT(&p->cond, NULL);
	MUTEX_INIT(&p->mutex);

	ret = pthread_attr_init(&attr);
	if (0 != ret)
		return ret;

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (0 != ret)
	{
		pthread_attr_destroy(&attr);
		return ret;
	}

	for (i = 0; i < p->size; i++)
	{
		ret = pthread_create(&tid, &attr, thread_cycle, p);
		if (0 != ret)
		{
			p->size = i;
			pthread_attr_destroy(&attr);
			return ret;
		}
	}

	pthread_attr_destroy(&attr);

	return 0;
}

/*
 * name: MODULE_FUN_NAME(ThreadPool, destroy)
 * description: stop threads for the pool
 * return value: 0: success;
 * 				<0: error
 * args: p: pointer to pool 
 */
int MODULE_FUN_NAME(ThreadPool, destroy)(T p)
{
	int i;
	int flag = 0;

	assert(p);

	for (i = 0; i < p->size; i++)
	{
		flag = 0;

		if (MODULE_FUN_NAME(ThreadPool, post)(
						p, 
						thread_exit_handler, 
						&flag))
		{
			return -1;
		}

		while (0 == flag)
			pthread_yield();

	}

	COND_DESTROY(&p->cond);
	MUTEX_DESTROY(&p->mutex);

	return 0;
}

/*
 * name: MODULE_FUN_NAME(ThreadPool, post)
 * description: post a task to thread pool
 * return value: 0: success;
 * 				-1: no mem
 * args: @p: pointer to thread pool
 * 		 @cb: user's callback
 * 		 @cl: user's private data, will passed to cb
 */
int MODULE_FUN_NAME(ThreadPool, post)(
		T p, 
		MODULE_FUN_NAME(ThreadPool, Cb_T) cb, 
		void *cl)
{
	TASK task = NULL;

	assert(p);

	task = (TASK)calloc(1, sizeof(*task));
	if (NULL == task)
		return -1;

	task->cb = cb;
	task->cl = cl;

	MUTEX_LOCK(&p->mutex);

	MODULE_FUN_NAME(Queue, put)(p->queue, task);

	COND_SIGNAL(&p->cond);

	MUTEX_UNLOCK(&p->mutex);

	return 0;
}



/*
 * local routines
 */
static void thread_exit_handler(void *p)
{
	int *flag = (int *)p;

	*flag = 1;

	pthread_exit(NULL);
}

static void *thread_cycle(void *priv)
{
	void *cl = NULL;
	MODULE_FUN_NAME(ThreadPool, Cb_T) cb = NULL;
	TASK task = NULL;
	T p = NULL; 

	p = (T)priv;

	for ( ; ; )
	{
		MUTEX_LOCK(&p->mutex);
try_again:
		if (MODULE_FUN_NAME(Queue, get)(p->queue, &task) != 0)
		{
			COND_WAIT(&p->cond, &p->mutex);
			goto try_again;
		}
		MUTEX_UNLOCK(&p->mutex);

		cl = task->cl;
		cb = task->cb;
		free(task);	// must free the mem we allocated
		task = NULL;

		if (cb)
			cb(cl);
	}

	return NULL;
}

