
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "process-lock.h"

#define MQ_PATH	"./hello"

static int quit_flag = 0;

static void *write_pthread(void *priv)
{
	unsigned int prio = 0;
	int ret = 0;
	ProcessLockCond_T lock = (ProcessLockCond_T)priv;

	fprintf(stdout, "%s waiting...%p\n", __func__, lock);
	ret = MODULE_FUN_NAME(ProcessLockCond, getLock)(lock);
	ret = MODULE_FUN_NAME(ProcessLockCond, wait)(lock);
	ret = MODULE_FUN_NAME(ProcessLockCond, releaseLock)(lock);
	fprintf(stdout, "%s waiting...done %d\n", __func__, ret);
	return NULL;
}

static void *read_pthread(void *priv)
{
	unsigned int prio = 0;
	int ret = 0;
	ProcessLockCond_T lock = (ProcessLockCond_T)priv;

	sleep(1);

	fprintf(stdout, "%s signal...%p\n", __func__, lock);
	ret = MODULE_FUN_NAME(ProcessLockCond, getLock)(lock);
	ret = MODULE_FUN_NAME(ProcessLockCond, signal)(lock);
	ret = MODULE_FUN_NAME(ProcessLockCond, releaseLock)(lock);
	fprintf(stdout, "%s waiting...done %d\n", __func__, ret);


	quit_flag = 1;

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	pthread_t pid = 0;
	ProcessLockCond_T lock = NULL;

	lock = MODULE_FUN_NAME(ProcessLockCond, create)(MQ_PATH, 0);
	if (lock == NULL)
	{
		fprintf(stdout, "create: %d, %s\n", ret, strerror(errno));
		return 0;
	}

	/*
	lock = MODULE_FUN_NAME(ProcessLockCond, map)(MQ_PATH, 0);
	if (lock == NULL) 
	{
		fprintf(stdout, "open failed\n");
		goto out;
	}
	*/
	
	fprintf(stdout, "lock: %p\n", lock);
	pthread_create(&pid, NULL, read_pthread, (void *)lock);
	pthread_create(&pid, NULL, write_pthread, (void *)lock);

	while (quit_flag == 0) sleep(1);

//	ret = MODULE_FUN_NAME(ProcessLockCond, unmap)(lock);
	fprintf(stdout, "unmap: %d\n", ret);
out:
	ret = MODULE_FUN_NAME(ProcessLockCond, destroy)(&lock);
	fprintf(stdout, "destroy: %d\n", ret);

	return 0;
}


