
/*
 * implements the interfaces of mq
 * just used this interfaces in threads in the same process
 * we just not support process now,
 * filename:	mq-shared.c
 */

#include <string.h>
#include "mq-shared.h"

mq_t *create_mq_t(const char *name, int oflag, mode_t mode, 
			long maxmsg, long msgsize)
{
	mq_t *mq = NULL;
	struct mq_attr attr;

	if (!name) return NULL;
	if (!(mq = calloc(1, sizeof(*mq)))) return NULL;

	if(!(mq->name = strdup(name))) {
		free(mq);
		return NULL;
	}

	pthread_mutex_init(&mq->lock, NULL);
	pthread_cond_init(&mq->rcond, NULL);
	pthread_cond_init(&mq->wcond, NULL);
	mq->msgs = 0;
	mq->exit = 0;
	mq->maxmsg = maxmsg > JOHN_MQ_PTHREAD_MAX_MSG ? JOHN_MQ_PTHREAD_MAX_MSG : maxmsg;

//	attr.mq_flags 	= 0;
	attr.mq_flags 	= O_NONBLOCK;
	attr.mq_maxmsg 	= maxmsg > JOHN_MQ_PTHREAD_MAX_MSG ? JOHN_MQ_PTHREAD_MAX_MSG : maxmsg;
	attr.mq_msgsize = msgsize > JOHN_MQ_PTHREAD_MAX_MSG_SIZE ? JOHN_MQ_PTHREAD_MAX_MSG_SIZE : msgsize;
	attr.mq_curmsgs = 0;
	if ((mq->fd = mq_open(mq->name, oflag, mode, &attr)) == -1) {
		perror("mq_open");
		pthread_mutex_destroy(&mq->lock);
		pthread_cond_destroy(&mq->rcond);
		pthread_cond_destroy(&mq->wcond);
		free(mq->name);
		free(mq);
		mq = NULL;
		return NULL;
	}

	if(!mq_getattr(mq->fd, &attr)) {
		fprintf(stdout, "mq_flags: %ld mq_maxmsg: %ld mq_msgsize: %ld mq_curmsgs: %ld\n", 
			attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	
	}

	fprintf(stdout, "create mq : %s ok\n", name);
	return mq;
}

void destroy_mq_t(mq_t **mqpp)
{
	mq_t *mq = NULL;

	if (mqpp && (mq = *mqpp)) {
		fprintf(stdout, "destroying mq\n");
		pthread_mutex_lock(&mq->lock);
		mq->exit = 1;
		pthread_mutex_unlock(&mq->lock);
		pthread_cond_broadcast(&mq->rcond);
		pthread_cond_broadcast(&mq->wcond);
		usleep(100);
	
		mq_close(mq->fd);
		pthread_mutex_destroy(&mq->lock);
		pthread_cond_destroy(&mq->rcond);
		pthread_cond_destroy(&mq->wcond);
		mq_unlink(mq->name);	// make sure no others is accessing
		free(mq->name);
		free(mq);
		*mqpp = NULL;
	}
}

/*
 * put a message into mq
 * @mq
 * @buf		message stored in the @buf
 * @len		how many bytes in @buf will put th mq
 * @prio	the prio of message
 */
int mq_put(mq_t *mq, char *buf, int len, unsigned int prio)
{
	int	total = 0;

	if (!mq || !buf || len < 0) {
		fprintf(stderr, "mq: %p, buf: %p, len: %d\n", mq, buf, len);
		return -2;
	}

	pthread_mutex_lock(&mq->lock);
	fprintf(stdout, "sending message\n");
	while ((mq->msgs > mq->maxmsg - 1) && !mq->exit) {
		fprintf(stdout, "mq is full\n");
		pthread_cond_wait(&mq->wcond, &mq->lock);
		fprintf(stdout, "wake up writer by other\n");
	}
	total = mq_send(mq->fd, buf, len, prio);
	mq->msgs++;
	fprintf(stdout, "send message %d ok\n", mq->msgs);
	pthread_mutex_unlock(&mq->lock);
	pthread_cond_signal(&mq->rcond);

	if (total == -1) {
		perror("mq_send");
	}

	fprintf(stdout, "send total %d\n", total);
	return total;
}


/*
 * get a message from the mq
 * @mq
 * @buf		for store message
 * @len		the length of the buf, must greater than mq's size
 * @prio	prio of ms
 */
int mq_get(mq_t *mq, char *buf, int len, unsigned int *prio)
{
	int	recv;

	if (!mq || !buf || len < 0) {
		fprintf(stderr, "arguments error, mq: %p, buf: %p, len: %d\n",
				mq, buf, len);
		return -2;
	}

	pthread_mutex_lock(&mq->lock);
	while (!mq->msgs && !mq->exit) {
		fprintf(stdout, "mq is empty\n");
		pthread_cond_wait(&mq->rcond, &mq->lock);
		fprintf(stdout, "wake up reader by other\n");
	}
	fprintf(stdout, "receving msg\n");
	recv = mq_receive(mq->fd, buf, len, prio);
	mq->msgs--;
	pthread_mutex_unlock(&mq->lock);
	pthread_cond_signal(&mq->wcond);
	if (recv == -1) {
		perror("mq_receive");
	}

	return recv;
}

