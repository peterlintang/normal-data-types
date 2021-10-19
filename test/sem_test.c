
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "named-sem.h"

#define TEST_NUM	10
#define MQ_PATH	"/hello.txt"

static int quit_flag = 0;

static void *write_pthread(void *priv)
{
	unsigned int prio = 0;
	int ret = 0;
	NamedSem_T sem = (NamedSem_T)priv;

	fprintf(stdout, "%s waiting...%p\n", __func__, sem);
	ret = MODULE_FUN_NAME(NamedSem, wait)(sem, 0);
	fprintf(stdout, "%s waiting...get %d\n", __func__, ret);
	sleep(3);
	fprintf(stdout, "%s posting...%d\n", __func__, ret);
	ret = MODULE_FUN_NAME(NamedSem, post)(sem);
	return NULL;
}

static void *read_pthread(void *priv)
{
	unsigned int prio = 0;
	int ret = 0;
	NamedSem_T sem = (NamedSem_T)priv;

	sleep(1);

	fprintf(stdout, "%s waiting...%p\n", __func__, sem);
	ret = MODULE_FUN_NAME(NamedSem, wait)(sem, 0);
	fprintf(stdout, "%s waiting...done %d\n", __func__, ret);


	quit_flag = 1;

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	pthread_t pid = 0;
	NamedSem_T sem = NULL;

	MODULE_FUN_NAME(NamedSem, destroy)(MQ_PATH);
	ret = MODULE_FUN_NAME(NamedSem, create)(MQ_PATH, 0, 1);
	if (ret != 0)
	{
		fprintf(stdout, "create: %d, %s\n", ret, strerror(errno));
		return 0;
	}

	sem = MODULE_FUN_NAME(NamedSem, open)(MQ_PATH);
	if (sem == NULL) 
	{
		fprintf(stdout, "open failed\n");
		goto out;
	}
	
	pthread_create(&pid, NULL, read_pthread, (void *)sem);
	pthread_create(&pid, NULL, write_pthread, (void *)sem);

	while (quit_flag == 0) sleep(1);

	MODULE_FUN_NAME(NamedSem, close)(&sem);
out:
	MODULE_FUN_NAME(NamedSem, destroy)(MQ_PATH);

	return 0;
}


