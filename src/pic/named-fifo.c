
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
 * implement the named fifo interfaces
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		// for access 
#include <string.h>		// for strdup

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#include "named-fifo.h"

#define JOHN_LIN_NFIFO_ACCESS_MODE	(S_IRUSR | S_IWUSR)

#define T NamedFifo_T

struct T {
	char	*pathname;
	int	fd;
};

/*
 * name: MODULE_FUN_NAME(NamedFifo, create)
 * description: create a new named fifo for process communication
 * return value: 
 * args: @pathname: path to create named fifo
 */
int MODULE_FUN_NAME(NamedFifo, create)(const char *pathname)
{
	assert(pathname != NULL);

	if (mkfifo(pathname, JOHN_LIN_NFIFO_ACCESS_MODE) == -1) {
		return -1;
	}

	return 0;
}

/*
 * name: MODULE_FUN_NAME(NamedFifo, destroy)
 * description: destroy a named fifo for process communication
 * return value: 
 * args: @pathname: path of named fifo
 */
int MODULE_FUN_NAME(NamedFifo, destroy)(const char *pathname)
{
	struct stat sbuf;

	assert(pathname != NULL);

	if (access(pathname, F_OK) == -1) {
		return -2;
	}

	if (stat(pathname, &sbuf) == -1) {
		return -1;
	}

	if (S_ISFIFO(sbuf.st_mode)) {
		unlink(pathname);
	} else {
	}

	return 0;
}
	

/*
 * name: MODULE_FUN_NAME(NamedFifo, open)
 * description: open a named fifo for process communication
 * return value: 
 * args: @pathname: path of named fifo
 * 		 @mode: 
 */
T MODULE_FUN_NAME(NamedFifo, open)(const char *pathname, mode_t mode)
{
	T nfifo = NULL;

	assert(pathname != NULL);

	if (!(nfifo = calloc(1, sizeof(*nfifo)))) {
		return NULL;
	}

	if (!(nfifo->pathname = strdup(pathname))) {
		free(nfifo);
		return NULL;
	}

	if ((nfifo->fd = open(pathname, mode)) == -1) {
		free(nfifo->pathname);
		free(nfifo);
		return NULL;
	}

	return nfifo;
}

/*
 * name: MODULE_FUN_NAME(NamedFifo, close)
 * description: close named fifo for process communication
 * return value: 
 * args: @pnfifo: 
 */
void MODULE_FUN_NAME(NamedFifo, close)(T *pnfifo)
{
	T nfifo = NULL;

	assert(pnfifo != NULL);
	assert(*pnfifo != NULL);

	nfifo = *pnfifo;
	close(nfifo->fd);
	free(nfifo->pathname);
	free(nfifo);
	*pnfifo = NULL;
}

/*
 * name: MODULE_FUN_NAME(NamedFifo, read)
 * description: read from a named fifo for process communication
 * return value: 
 * args: @nfifo: pointer to named fifo
 * 		 @buf: to store data
 * 		 @len: length of data to read 
 */
int MODULE_FUN_NAME(NamedFifo, read)(T nfifo, char *buf, int len)
{
	int	read_len  = 0;
	int	total_len = 0;
	
	assert(nfifo != NULL);
	assert(buf != NULL);
	assert(len > 0);

	if (nfifo->fd < 0) {
		return -2;
	}

	if ((read_len = read(nfifo->fd, &total_len, sizeof(int))) == -1) {
		return -1;
	}

	if (total_len > len) {
		return -4;
	}

	len = total_len;
	total_len = 0;
	while (total_len < len) {
		read_len = read(nfifo->fd, buf + total_len, len - total_len);
		if (read_len == -1) {	// ignore intre, just return -1
			return -1;
		}
		if (read_len == 0) break;
		total_len  += read_len;
	}

	return total_len;
}

/*
 * name: MODULE_FUN_NAME(NamedFifo, write)
 * description: write data to a named fifo for process communication
 * return value: 
 * args: @pfifo: pointer to named fifo
 * 		 @buf: data to write
 * 		 @len: length of data to write
 */
int MODULE_FUN_NAME(NamedFifo, write)(T nfifo, const char *buf, int len)
{
	int	write_len = 0;
	int	total_len = 0;

	assert(nfifo != NULL);
	assert(buf != NULL);
	assert(len > 0);

	if (nfifo->fd < 0) {
		return -2;
	}

	if ((write_len = write(nfifo->fd, &len, sizeof(int))) == -1) {
		return -1;
	}

	while (total_len < len) {
		write_len = write(nfifo->fd, buf + total_len, len - total_len);
//		if (write_len == -1) {
		if (write_len <= 0) {	// just ignore intr and return -1
			return -1;
		}

//		if (write_len == 0) break;

		total_len += write_len;
	}

	return total_len;
}
















