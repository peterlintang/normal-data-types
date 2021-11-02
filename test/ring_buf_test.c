

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "ring_buf.h"
#include "share-mem.h"



/********************************/

static pthread_mutex_t buffer_lock;
static Ringbuf_T r_buf = NULL;
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
