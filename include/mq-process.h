
/*************************************
 *
 * filename:	mq-process.h
 * description:	implement the interfaces of mq (message queue)
 * 				just used this interfaces in threads, 
 * 				we now want to implement process support
 * 				posix ipc
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_MQ_PROCESS_H__
#define CI_MQ_PROCESS_H__


#include "module.h"

#define JOHN_MQ_PROCESS_MAX_MSG		10
#define JOHN_MQ_PROCESS_MAX_MSG_SIZE 	8192

#define T Mq_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(Mq, create)
 * description: create a new message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, create)(const char *pathname, long maxmsg, 
			long msgsize, int block);

/*
 * name: MODULE_FUN_NAME(Mq, destroy)
 * description: destroy a message queue
 * return value: 
 * args: @: 
 */

int MODULE_FUN_NAME(Mq, destroy)(const char *pathname);
/*
 * name: MODULE_FUN_NAME(Mq, open)
 * description: open a message queue
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(Mq, open)(const char *pathname, int flags);

/*
 * name: MODULE_FUN_NAME(Mq, close)
 * description: close a message queue
 * return value: 
 * args: @: 
 */
void MODULE_FUN_NAME(Mq, close)(T *mqp);

/*
 * name: MODULE_FUN_NAME(Mq, put)
 * description: put a message to message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, put)(T mq, char *buf, int len, unsigned int prio);

/*
 * name: MODULE_FUN_NAME(Mq, get)
 * description: get a message from message queue
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(Mq, get)(T mq, char *buf, int len, unsigned int *prio);

#undef T

#endif // CI_MQ_PROCESS_H__

