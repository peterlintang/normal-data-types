
/*
 *
 * 实现环形缓冲区
 * 在num数组中，当write index + 1 == read index时，
 * 数组满， 此时数组有一个空元素，所以真正的大小式num-1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "ring_buf.h"


#define T Ringbuf_T

struct T {
	unsigned char *buf;
	unsigned int buf_max;
	unsigned int read_index;
	unsigned int write_index;
};



static int ring_buf_print(T r, int len, char *s)
{
#if TEST_RING
	fprintf(stdout, "buf: %p, max: %d, read: %d, write: %d, %s, len: %d\n",
			r->buf, r->buf_max, r->read_index, r->write_index,
			s, len);
#endif
	return 0;
}

/*
 * 以ptr为存储空间初始化一个ringbuf，ptr长度为len
 * 成功返回一个ringbuf的指针,失败null
 * ptr: ringbuf的空间，
 * len: ptr的长度
 */
T MODULE_FUN_NAME(Ringbuf, init)(unsigned char *ptr, unsigned int len)
{
	assert(ptr && len > 0);

	T r = NULL;

	r = (T)calloc(1, sizeof(*r));
	if (r)
	{
		r->buf_max = len;
		r->read_index = 0;
		r->write_index = 0;
		r->buf = ptr;
	}

	return r;
}

T MODULE_FUN_NAME(Ringbuf, create)(unsigned int num)
{
	assert(num > 0);

	T r = NULL;

	r = (T)calloc(1, sizeof(*r) + num);
	if (r)
	{
		r->buf_max = num;
		r->read_index = 0;
		r->write_index = 0;
		r->buf = ((unsigned char *)r) + sizeof(*r);
	}

	return r;
}

int MODULE_FUN_NAME(Ringbuf, destroy)(T *pr)
{
	assert(pr && *pr);

	free(*pr);
	*pr = NULL;
	return 0;
}


int MODULE_FUN_NAME(Ringbuf, is_empty)(T r)
{
	assert(r);

	if (r->read_index == r->write_index)
		return 1;
	return 0;
}

int MODULE_FUN_NAME(Ringbuf, is_full)(T r)
{
	assert(r);

	if (((r->write_index + 1) % r->buf_max) == r->read_index)
		return 1;
	return 0;
}

int MODULE_FUN_NAME(Ringbuf, left_space)(T r)
{
	assert(r);

	if (r->write_index >= r->read_index)
		return (r->buf_max - (r->write_index - r->read_index) - 1);
	else
		return r->read_index - r->write_index - 1;
}

int MODULE_FUN_NAME(Ringbuf, available_data)(T r)
{
	assert(r);

	if (r->write_index >= r->read_index)
	{
		return r->write_index - r->read_index;
	}
	else
	{
		return r->buf_max - (r->read_index - r->write_index);
	}
}

int MODULE_FUN_NAME(Ringbuf, put)(T r, unsigned char *data, unsigned int len)
{
	int ret = 0;
	int left = 0;

	assert(r != NULL);
	assert(data != NULL);
	assert(len > 0);

	if (MODULE_FUN_NAME(Ringbuf, is_full)(r))
	{
		return -1;
	}

	ring_buf_print(r, len, "put b");

	left = MODULE_FUN_NAME(Ringbuf, left_space)(r);
	len = (left >= len ? len : left);
	ret = len;

	if (r->write_index + len > r->buf_max)
	{
		memcpy(r->buf + r->write_index, data, r->buf_max - r->write_index);
		len = len - (r->buf_max - r->write_index);

		memcpy(r->buf, data + (r->buf_max - r->write_index), len);
		r->write_index = len;
	}
	else
	{
		memcpy(r->buf + r->write_index, data, len);
		r->write_index = (r->write_index + len) % r->buf_max;
	}

	ring_buf_print(r, len, "put a");

	return ret;
}

int MODULE_FUN_NAME(Ringbuf, get)(T r, unsigned char *data, unsigned int len)
{
	int ret = 0;
	int available = 0;

	assert(r != NULL);
	assert(data != NULL);
	assert(len > 0);

	if (MODULE_FUN_NAME(Ringbuf, is_empty)(r))
	{
		return -1;
	}

	ring_buf_print(r, len, "get b");

	available = MODULE_FUN_NAME(Ringbuf, available_data)(r);
	len = available >= len ? len : available;
	ret = len;

	if (r->read_index + len > r->buf_max)
	{
		memcpy(data, r->buf + r->read_index, r->buf_max - r->read_index);
		len = len - (r->buf_max - r->read_index);

		memcpy(data + r->buf_max - r->read_index, r->buf, len);
		r->read_index = len;
	}
	else
	{
		memcpy(data, r->buf + r->read_index, len);
		r->read_index = (r->read_index + len) % r->buf_max;
	}

	ring_buf_print(r, len, "get a");

	return ret;
}




/***********************************************************************/
#if 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>




/********************************/

static pthread_mutex_t buffer_lock;
static T r_buf;
static int quit_flag = 0;

static void SleepMs(int msecs) {
	struct timespec short_wait;
	struct timespec remainder;
	short_wait.tv_sec = msecs / 1000;
	short_wait.tv_nsec = (msecs % 1000) * 1000 * 1000;
	nanosleep(&short_wait, &remainder);

}

static void *write_thread(void *arg)
{
	int ret = 0;
	int count = (int )arg;
	char buf[2048] = { 0 };
	FILE *fp = NULL;

	fp = fopen("output_write", "w");

	fprintf(stdout, "%s: count: %d\n", __func__, count);


	while (quit_flag == 0)
	{

	for (int i = 0; i < 2048; i++)
	{
		if ((i + 1) % 16 == 0)
			buf[i] = '\n';
		else
			buf[i] = 'a' + rand() % 26;
	}
		pthread_mutex_lock(&buffer_lock);
		ret = MODULE_FUN_NAME(Ringbuf, put)(r_buf, buf, 2048);
		pthread_mutex_unlock(&buffer_lock);
		if (ret > 0)
		{
			fwrite(buf, 1, ret, fp);
			fflush(fp);
			count--;
			if (count == 0) break;

			}
		else
		{
			SleepMs(10);
		}
	}

	fclose(fp);
	fp = NULL;
	fprintf(stdout, "%s: exit\n", __func__);

	return NULL;
}

static void *read_thread(void *arg)
{
	int ret = 0;
	char buf[320] = { 0 };
	FILE *fp = NULL;

	fp = fopen("output_read", "w");

	while (quit_flag == 0)
	{
		pthread_mutex_lock(&buffer_lock);
		ret = MODULE_FUN_NAME(Ringbuf, get)(r_buf, buf, 320);
		pthread_mutex_unlock(&buffer_lock);
		if (ret > 0)
		{
			fwrite(buf, 1, ret, fp);
			fflush(fp);
		}
	}

	fclose(fp);
	fp = NULL;
	fprintf(stdout, "%s: exit\n", __func__);

	return NULL;
}

/*
 * ring buf的测试函数，开两个线程，一个往buf丢数据，一个从buf读数据，并将两者的数据保存到文件，然后可以用比较工具看数据是否一致
 *
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	pthread_t pid;
	int count = atoi(argv[1]);

	r_buf = MODULE_FUN_NAME(Ringbuf, create)(8 * 1024);

	pthread_mutex_init(&buffer_lock, NULL);

	pthread_create(&pid, NULL, read_thread, NULL);
	pthread_create(&pid, NULL, write_thread, (void *)count);

	while (count != 0)
	{
		count--;
	}


	sleep(2);
	quit_flag = 1;

	pthread_mutex_destroy(&buffer_lock);
	MODULE_FUN_NAME(Ringbuf, destroy)(&r_buf);
	fprintf(stdout, "%s: exit\n", __func__);

	return 0;
}


#endif



