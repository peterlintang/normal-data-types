
/*************************************
 *
 * filename:	named-fifo.h
 * description:	implement the named fifo for processes 
 * 				communicate
 * author:		john lin
 * date:		2020-02-27
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/
/*
 *
 *
 */

#ifndef CI_NFIFO_H__
#define CI_NFIFO_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "module.h"


#define T NamedFifo_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(NamedFifo, create)
 * description: create a new named fifo for process communication
 * return value: 
 * args: @pathname: path to create named fifo
 */
int MODULE_FUN_NAME(NamedFifo, create)(const char *pathname);

/*
 * name: MODULE_FUN_NAME(NamedFifo, create)
 * description: destroy a named fifo for process communication
 * return value: 
 * args: @pathname: path of named fifo
 */
int MODULE_FUN_NAME(NamedFifo, destroy)(const char *pathname);

/*
 * name: MODULE_FUN_NAME(NamedFifo, open)
 * description: open a named fifo for process communication
 * return value: 
 * args: @pathname: path of named fifo
 * 		 @mode: 
 */
T MODULE_FUN_NAME(NamedFifo, open)(const char *pathname, mode_t mode);

/*
 * name: MODULE_FUN_NAME(NamedFifo, close)
 * description: close named fifo for process communication
 * return value: 
 * args: @pnfifo: 
 */
void MODULE_FUN_NAME(NamedFifo, close)(T *pnfifo);

/*
 * name: MODULE_FUN_NAME(NamedFifo, read)
 * description: read from a named fifo for process communication
 * return value: 
 * args: @nfifo: pointer to named fifo
 * 		 @buf: to store data
 * 		 @len: length of data to read 
 */
int MODULE_FUN_NAME(NamedFifo, read)(T nfifo, char *buf, int len);

/*
 * name: MODULE_FUN_NAME(NamedFifo, write)
 * description: write data to a named fifo for process communication
 * return value: 
 * args: @pfifo: pointer to named fifo
 * 		 @buf: data to write
 * 		 @len: length of data to write
 */
int MODULE_FUN_NAME(NamedFifo, write)(T nfifo, const char *buf, int len);

#undef T

#endif  // CI_NFIFO_H__




