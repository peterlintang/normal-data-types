/************************************************************
 *
 * filename:	process-lock.c
 * description: implement the interfaces of 
 *              processes shared memory lock
 *              and condition
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * copyright (c) john lin
 **************************************************************/

/*
 * implements the pthread lock in multiple processes
 * system v ipc
 * filename:	process-lock.h
 *
 */

#ifndef CI_PROCESS_LOCK_H__
#define CI_PROCESS_LOCK_H__


#include "module.h"

#define T ProcessLockCond_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(ProcessLockCond, create)
 * description: create a memory shared lock and cond
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessLockCond, create)(const char *pathname, int id);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, destroy)
 * description: destroy a memory shared lock and cond
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, destroy)(T *plc);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, map)
 * description: map a memory shared lock and cond to process address
 * return value: 
 * args: @: 
 */
T MODULE_FUN_NAME(ProcessLockCond, map)(const char *pathname, int id);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, unmap)
 * description: unmap a memory shared lock and cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, unmap)(T plc);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, get)
 * description: get a memory shared lock from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, getLock)(T plc);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, release)
 * description: release a memory shared lock from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, releaseLock)(T plc);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, broadcast)
 * description: broadcast a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int broadcast_process_cond(T plc);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, signal)
 * description: signal a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, signal)(T plc);

/*
 * name: MODULE_FUN_NAME(ProcessLockCond, wait)
 * description: wait a memory shared cond from process address
 * return value: 
 * args: @: 
 */
int MODULE_FUN_NAME(ProcessLockCond, wait)(T plc);

#undef T

#endif // CI_PROCESS_LOCK_H__

