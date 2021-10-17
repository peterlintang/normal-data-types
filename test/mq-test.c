
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "mq-process.h"

#define TEST_NUM	10
#define MQ_PATH	"/hello.txt"

static int quit_flag = 0;

static void *write_pthread(void *priv)
{
	Mq_T mq = NULL;
	char buf[2048] = { 0 };
	unsigned int prio = 0;
	int ret = 0;

	sleep(5);

	mq = MODULE_FUN_NAME(Mq, open)(MQ_PATH, O_WRONLY);
	if (mq == NULL)
	{
		fprintf(stdout, "write failed to open mq\n");
		return NULL;
	}

	for (int i = 0; i < TEST_NUM; i++)
	{
		memset(buf, 'a' + i, 24);
		ret = MODULE_FUN_NAME(Mq, put)(mq, buf, 24, prio);
		if (ret == 0)
		{
			fprintf(stdout, "write: %s, %d, %d\n", buf, prio, ret);
		}
		else
		{
			fprintf(stdout, "write error: %d, %s\n", ret, strerror(errno));
		}
	}

	MODULE_FUN_NAME(Mq, close)(&mq);
	return NULL;
}

static void *read_pthread(void *priv)
{
	Mq_T mq = NULL;
	char buf[2048] = { 0 };
	unsigned int prio = 0;
	int ret = 0;

	mq = MODULE_FUN_NAME(Mq, open)(MQ_PATH, O_RDONLY);
	if (mq == NULL)
	{
		fprintf(stdout, "read failed to open mq\n");
		return NULL;
	}

	for (int i = 0; i < TEST_NUM * 2; i++)
	{
		ret = MODULE_FUN_NAME(Mq, get)(mq, buf, 2048, &prio);
		if (ret > 0)
		{
			fprintf(stdout, "read: %s, %d, %d\n", buf, prio, ret);
		}
		else
		{
			fprintf(stdout, "read error: %d\n", ret);
		}
	}

	MODULE_FUN_NAME(Mq, close)(&mq);

	quit_flag = 1;

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t pid = 0;

	MODULE_FUN_NAME(Mq, create)(MQ_PATH, 10, 1024, 0);

	pthread_create(&pid, NULL, read_pthread, NULL);
	pthread_create(&pid, NULL, write_pthread, NULL);
	pthread_create(&pid, NULL, write_pthread, NULL);

	while (quit_flag == 0) sleep(1);

	MODULE_FUN_NAME(Mq, destroy)(MQ_PATH);

	return 0;
}


