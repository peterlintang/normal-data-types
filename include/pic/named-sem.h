/*************************************
 *
 * filename:	named-sem.h
 * description:	implements the named semphore
 * 			for process usage
 * author:		john lin
 * date:		2020-02-22
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_NAMED_SEM
#define CI_NAMED_SEM


#include "module.h"


#define T NamedSem_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(NamedSem, create)
 * description: create a new named semphore 
 * return value: return the pointer to named semphore
 * args: @pathname: named semphore pathname
 * 		 @flags:
 * 		 @res_num:
 */
int MODULE_FUN_NAME(NamedSem, create)(const char *pathname, int flags, int res_num);

/*
 * name: MODULE_FUN_NAME(NamedSem, destroy)
 * description: destroy a named semphore 
 * return value: 
 * args: @pathname: named semphore pathname
 */
int MODULE_FUN_NAME(NamedSem, destroy)(const char *pathname);

/*
 * name: MODULE_FUN_NAME(NamedSem, open)
 * description: open a named semphore
 * return value: pointer to named semphore
 * args: @pathname: named semphore pathname
 */
T MODULE_FUN_NAME(NamedSem, open)(const char *pathname);

/*
 * name: MODULE_FUN_NAME(NamedSem, close)
 * description: close a named semphore
 * return value: 
 * args: @psem: pointer's pointer to named semphore
 */
void MODULE_FUN_NAME(NamedSem, close)(T *psem);

/*
 * name: MODULE_FUN_NAME(NamedSem, wait)
 * description: wait named semphore to be greater than 1
 * return value: 
 * args: @sem: pointer to named semphore
 * 		 @nonblock: block or not
 */
int MODULE_FUN_NAME(NamedSem, wait)(T sem, int nonblock);

/*
 * name: MODULE_FUN_NAME(NamedSem, post)
 * description: post a named semphore
 * return value: 
 * args: @sem: pointer to named semphore
 */
int MODULE_FUN_NAME(NamedSem, post)(T sem);


#undef T

#endif // CI_NAMED_SEM

