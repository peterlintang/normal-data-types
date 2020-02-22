/*************************************
 *
 * filename:	thread-signal.h
 * description:	imlement the signals in threads env process
 * 		only one thread process signals, other pthreads
 * 		block signals, and do their own job
 * author:		john lin
 * date:		2012-02-22
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "thread-signal.h"

/*
 * name: MODULE_FUN_NAME(Thread, blockSignal)
 * description: block all signals
 * return value: 0: success
 * 				other: failed
 * args: @newset: new signal set
 * 		@oldset: old signal set
 */
int MODULE_FUN_NAME(Thread, blockSignals)(sigset_t *newset, sigset_t *oldset)
{
	int	i;

	assert(newset && oldset);

	sigemptyset(newset);

	for (i = 1; i < MAX_SIGNALS; i++)
		sigaddset(newset, i);

	return pthread_sigmask(SIG_BLOCK, newset, oldset);
}

/*
 * name: MODULE_FUN_NAME(Thread, unblockSignal)
 * description: unblock signal set @newset, and return the old signal to @oldset
 * return value: 0: success
 * 				other: failed
 * args: @newset: new signal set to unblock
 * 		@oldset: returnning the old signal set
 */
int MODULE_FUN_NAME(Thread, unblockSignals)(sigset_t *newset, sigset_t *oldset)
{
	assert(newset && oldset);

	return pthread_sigmask(SIG_UNBLOCK, newset, oldset);
}

/*
 * name: MODULE_FUN_NAME(Thread, sendSignal)
 * description: send signal @signo to thread @tid
 * return value: 0: send signal success
 * 				!=0: error number
 * args: @signo: signal number to send
 * 		 @tid: pthread id
 */
int MODULE_FUN_NAME(Thread, sendSignal)(int signo, pthread_t tid)
{
	assert(signo > 0 && signo < MAX_SIGNALS);

	return pthread_kill(tid, signo);

}


