/**************************************************************************
 *
 * Copyright: Copyright (c) 2018
 * All rights reserved. 
 *
 * Create On: 	2019-02-14
 * Filename: 	priv-signal.c
 * Description:	process signal 
 * Author: 		xiaojie.lin
 * Modified by:
 *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "macro.h"
#include "debug.h"
#include "func.h"
#include "priv-signal.h"

static sigset_t s_set;

static void *private_signal_thread(void *arg)
{
	int ret = 0;
	int sig_nu = 0;
	sigset_t *set = NULL;

	set = (sigset_t *)arg;
	for ( ; ; )
	{
		ret = sigwait(set, &sig_nu);
		if (0 != ret)
		{
			DEBUGP(DERROR, __func__, "wait sig failed: %d", ret);
			exit(2);
		}
		DEBUGP(DINFO, __func__, "get signal: %d", sig_nu);
	}

	return NULL;
}

/****************************************************************
 *
 * should be at the first of main before any pthreads created
 *
 ****************************************************************/
int mozi_signal_init(void)
{
	int ret = 0;
	pthread_t pid;

	sigfillset(&s_set);
	ret = pthread_sigmask(SIG_BLOCK, &s_set, NULL);
	if (0 != ret)
	{
		DEBUGP(DERROR, __func__, "set sigmask failed: %d", ret);
		return ret;
	}

	ret = pthread_create(&pid, NULL, private_signal_thread, &s_set);
	if (0 != ret)
	{
		DEBUGP(DERROR, __func__, "create signal thread failed: %d", ret);
		return ret;
	}

	return ret;
}

