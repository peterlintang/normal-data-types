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

#ifndef CI_THREAD_SIGNALS_H__
#define CI_THREAD_SIGNALS_H__


#include <signal.h>

#include "module.h"

#define MAX_SIGNALS	32


/*
 * name: MODULE_FUN_NAME(Thread, blockSignal)
 * description: 
 * return value: 
 * args: 
 */
int MODULE_FUN_NAME(Thread, blockSignals)(sigset_t *newset, sigset_t *oldset);

/*
 * name: MODULE_FUN_NAME(Thread, unblockSignal)
 * description: 
 * return value: 
 * args: 
 */
int MODULE_FUN_NAME(Thread, unblockSignals)(sigset_t *newset, sigset_t *oldset);

/*
 * name: MODULE_FUN_NAME(Thread, sendSignal)
 * description: send signal @signo to thread @tid
 * return value: 0: send signal success
 * 				other: error number
 * args: @signo: signal number to send
 * 		 @tid: the thread id to send signal @signo
 */
int MODULE_FUN_NAME(Thread, sendSignal)(int signo, pthread_t tid);


#endif // CI_THREAD_SIGNALS_H__


