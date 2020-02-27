/*************************************
 *
 * filename:	mq-process.h
 * description:	implement the interfaces of mq (message queue)
 * 				just used this interfaces in threads, 
 * 				we now want to implement process support
 * 				posix ipc
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <assert.h>

#include "mq-process.h"

#define T Mq_T
typedef struct T *T;

struct T {
//	int			msgs;
//	int			maxmsg;
	mqd_t			fd;
	char			*name;
} ;

/*
 * name: MODULE_FUN_NAME(Mq, create)
 * description: create a new message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, create)(const char *name, long maxmsg, 
			long msgsize, int block)
{
	int	flags = 0;
	mode_t	mode;
	mqd_t	fd;
	struct mq_attr attr;

	assert(name != NULL);

	if (block)
		attr.mq_flags 	= 0;
	else
		attr.mq_flags 	= O_NONBLOCK;

	attr.mq_maxmsg 	= maxmsg > JOHN_MQ_PROCESS_MAX_MSG ? \
				JOHN_MQ_PROCESS_MAX_MSG : maxmsg;
	attr.mq_msgsize = msgsize > JOHN_MQ_PROCESS_MAX_MSG_SIZE ? \
				JOHN_MQ_PROCESS_MAX_MSG_SIZE : msgsize;
	attr.mq_curmsgs = 0;

	flags = (O_CREAT | O_EXCL | O_RDWR);
	mode  = (S_IRUSR | S_IWUSR);
	if ((fd = mq_open(name, flags, mode, &attr)) == -1) {
		return -1;
	}

	if(!mq_getattr(fd, &attr)) {
	}

	if (mq_close(fd)) {
		return -1;
	}

	return 0;
}

/*
 * name: MODULE_FUN_NAME(Mq, destroy)
 * description: destroy a message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, destroy)(const char *pathname)
{
	assert(pathname != NULL);

	if(mq_unlink(pathname)) {
		return -1;
	}

	return 0;
}

/*
 * open an exist mq
 * @pathname	:	id for mq
 * @mode	:	S_IRUSR S_WUSR ....
 */
/*
 * name: MODULE_FUN_NAME(Mq, open)
 * description: open a message queue
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(Mq, open)(const char *pathname, int flags)
{
	T mq = NULL;

	assert(pathname != NULL);

	if (!(mq = calloc(1, sizeof(*mq)))) {
		return NULL;
	}

	if (!(mq->name = strdup(pathname))) {
		free(mq);
		return NULL;
	}

	if ((mq->fd = mq_open(mq->name, flags)) == -1) {
		free(mq->name);
		free(mq);
		return NULL;
	}

	return mq;
}

/*
 * name: MODULE_FUN_NAME(Mq, close)
 * description: close a message queue
 * return value: 
 * args: @: 
 */
void MODULE_FUN_NAME(Mq, close)(T *mqp)
{
	T mq = NULL;

	assert(mqp != NULL);
	assert(*mqp != NULL);

	mq = *mqp;
	mq_close(mq->fd);
	free(mq->name);
	free(mq);
	*mqp = NULL;
}

/*
 * put a message into mq
 * @mq
 * @buf		message stored in the @buf
 * @len		how many bytes in @buf will put th mq
 * @prio	the prio of message
 */
/*
 * name: MODULE_FUN_NAME(Mq, put)
 * description: put a message to message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, put)(T mq, char *buf, int len, unsigned int prio)
{
	int	total = 0;

	assert(mq != NULL);
	assert(buf != NULL);
	assert(len > 0);

	total = mq_send(mq->fd, buf, len, prio);

	return total;
}


/*
 * get a message from the mq
 * @mq
 * @buf		for store message
 * @len		the length of the buf, must greater than mq's size
 * @prio	prio of ms
 */
/*
 * name: MODULE_FUN_NAME(Mq, get)
 * description: get a message from message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, get)(T mq, char *buf, int len, unsigned int *prio)
{
	int	recv;

	assert(mq != NULL);
	assert(buf != NULL);
	assert(len > 0);

	recv = mq_receive(mq->fd, buf, len, prio);

	return recv;
}



