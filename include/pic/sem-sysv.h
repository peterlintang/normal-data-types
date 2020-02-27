
/*************************************
 *
 * filename:	sem-sysv.h
 * description:	implements the system v semaphore interfaces
 * author:		john lin
 * date:		2020-02-26
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_SEM_SYSV_H__
#define CI_SEM_SYSV_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#include "module.h"

/*
#define T SysvSem_T
typedef struct T *T;
struct T {
	char	*pathname;
	int	id;
	int	semid;
};
#undef T
*/

/*
 * name: MODULE_FUN_NAME(SysvSem, create)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, create)(const char *pathname, int id, int res_num);

/*
 * name: MODULE_FUN_NAME(SysvSem, destroy)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, destroy)(const char *pathname, int id);

/*
 * name: MODULE_FUN_NAME(SysvSem, open)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, open)(const char *pathname, int id);

/*
 * name: MODULE_FUN_NAME(SysvSem, close)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
void MODULE_FUN_NAME(SysvSem, close)(int semid);

/*
 * name: MODULE_FUN_NAME(SysvSem, get)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, get)(int semid);

/*
 * name: MODULE_FUN_NAME(SysvSem, release)
 * description: 
 * return value: 
 * args: @: 
 * 		 @: 
 */
int MODULE_FUN_NAME(SysvSem, release)(int semid);


#endif // CI_SEM_SYSV_H__


