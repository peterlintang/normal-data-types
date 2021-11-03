/************************************************************
 *
 * filename:	process-cond.c
 * description: implement the interfaces of 
 *              processes shared memory 
 *              condition
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * copyright (c) john lin
 **************************************************************/

/*
 * implements the pthread cond in multiple processes
 * system v ipc
 * filename:	process-cond.h
 *
 */

#ifndef CI_PROCESS_COND_H__
#define CI_PROCESS_COND_H__


#include "module.h"

#define T ProcessCond_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(ProcessCond, create)
 * description: create a memory shared lock and cond
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessCond, create)(void *ptr, int size);

/*
 * name: MODULE_FUN_NAME(ProcessCond, destroy)
 * description: destroy a memory shared lock and cond
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, destroy)(T *pc);

/*
 * name: MODULE_FUN_NAME(ProcessCond, broadcast)
 * description: broadcast a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, broadcast)(T pc);

/*
 * name: MODULE_FUN_NAME(ProcessCond, signal)
 * description: signal a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, signal)(T pc);

/*
 * name: MODULE_FUN_NAME(ProcessCond, wait)
 * description: wait a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessCond, wait)(T pc, ProcessLock_T pl);

#undef T

#endif // CI_PROCESS_COND_H__

