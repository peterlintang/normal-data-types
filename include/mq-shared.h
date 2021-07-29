
/*
 * implement the interfaces of mq
 * just used this interfaces in threads, 
 * we now not implement process support
 * filename:	mq-shared.h
 *
 * posix ipc
 * compare with mq-process.c
 */

#ifndef JOHN_LIN_PTHREAD_MQ_H__
#define JOHN_LIN_PTHREAD_MQ_H__

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>



#define JOHN_MQ_PTHREAD_MAX_MSG		10
#define JOHN_MQ_PTHREAD_MAX_MSG_SIZE 	8192

typedef struct __mq_t {
	int			exit;
	int			msgs;
	int			maxmsg;
	mqd_t			fd;
	char			*name;
	pthread_mutex_t 	lock;
	pthread_cond_t		rcond;
	pthread_cond_t		wcond;
} mq_t;

mq_t *create_mq_t(const char *name, int oflag, mode_t mode, 
			long maxmsg, long msgsize);
void destroy_mq_t(mq_t **mqpp);

int mq_put(mq_t *mq, char *buf, int len, unsigned int prio);
int mq_get(mq_t *mq, char *buf, int len, unsigned int *prio);

#endif // JOHN_LIN_PTHREAD_MQ_H__
