
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "named-sem.h"

#define TEST_NUM	10
#define MQ_PATH	"./hello.txt"

static int quit_flag = 0;

static void *write_pthread(void *priv)
{
	unsigned int prio = 0;
	int ret = 0;
	int sem_fd = (int)priv;

	fprintf(stdout, "%s waiting...%d\n", __func__, sem_fd);
	ret = MODULE_FUN_NAME(SysvSem, get)(sem_fd, 0);
	fprintf(stdout, "%s waiting...get %d\n", __func__, ret);
	sleep(3);
	fprintf(stdout, "%s posting...%d\n", __func__, ret);
	ret = MODULE_FUN_NAME(SysvSem, release)(sem_fd);
	return NULL;
}

static void *read_pthread(void *priv)
{
	unsigned int prio = 0;
	int ret = 0;
	int sem_fd = (int)priv;

	sleep(1);

	fprintf(stdout, "%s waiting...%d\n", __func__, sem_fd);
	ret = MODULE_FUN_NAME(SysvSem, get)(sem_fd, 0);
	fprintf(stdout, "%s waiting...done %d\n", __func__, ret);


	quit_flag = 1;

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int sem_fd = -1;
	pthread_t pid = 0;

	ret = MODULE_FUN_NAME(SysvSem, destroy)(MQ_PATH, 1);
	ret = MODULE_FUN_NAME(SysvSem, create)(MQ_PATH, 1, 1);
	if (ret != 0)
	{
		fprintf(stdout, "create: %d, %s\n", ret, strerror(errno));
		return 0;
	}

	sem_fd = MODULE_FUN_NAME(SysvSem, open)(MQ_PATH, 1);
	if (sem_fd == -1) 
	{
		fprintf(stdout, "open failed\n");
		goto out;
	}
	
	pthread_create(&pid, NULL, read_pthread, (void *)sem_fd);
	pthread_create(&pid, NULL, write_pthread, (void *)sem_fd);

	while (quit_flag == 0) sleep(1);

	ret = MODULE_FUN_NAME(SysvSem, close)(sem_fd);
	if (ret != 0)
	{
		fprintf(stdout, "close: %d, %s\n", ret, strerror(errno));
		return 0;
	}
out:
	ret = MODULE_FUN_NAME(SysvSem, destroy)(MQ_PATH, 1);
	if (ret != 0)
	{
		fprintf(stdout, "destroy: %d, %s\n", ret, strerror(errno));
		return 0;
	}

	return 0;
}


