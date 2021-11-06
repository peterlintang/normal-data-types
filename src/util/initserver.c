
#include <errno.h>
#include <unistd.h>	// for close
#include <sys/types.h>
#include <sys/socket.h>

#include "initserver.h"

int initserver(
		int type, 
		const struct sockaddr *addr, 
		socklen_t alen, 
		int qlen)
{
	int	fd;
	int	err = 0;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return (-1);
	if (bind(fd, addr, alen) < 0)
	{
		err = errno;
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) 
	{
		if (listen(fd, qlen) < 0)
		{
			err = errno;
			goto errout;
		}
	}

	return (fd);

errout:
	close(fd);
	errno = err;
	return (-1);
}

int initserver_reuseaddr(
		int type, 
		const struct sockaddr *addr, 
		socklen_t alen, 
		int qlen)
{
	int	fd;
	int	err = 0;
	int 	reuse = 1;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return (-1);
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
	{
		err = errno;
		goto errout;
	}
	if (bind(fd, addr, alen) < 0)
	{
		err = errno;
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) 
	{
		if (listen(fd, qlen) < 0)
		{
			err = errno;
			goto errout;
		}
	}

	return (fd);

errout:
	close(fd);
	errno = err;
	return (-1);
}


