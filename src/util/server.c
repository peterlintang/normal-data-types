#include "apue.h"
#include <fcntl.h>
#include <stropts.h>
#include <sys/socket.h>

/* pipe permissions: user rw, group rw others rw */
#define FIFO_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

/*
 * establish an endpoint to listen for connect requests.
 * returns fd if all ok, < 0 on error
 */
int
serv_listen(const char *name)
{
	int	tempfd;
	int	fd[2];

	/*
	 * create a file: mount point for fattach(),
	 */
	unlink(name);
	if ((tempfd = creat(name, FIFO_MODE)) < 0)
		return (-1);
	if (close(tempfd) < 0)
		return (-2);
	if (pipe(fd) < 0)
		return (-3);
	/*
	 * push connld & fattach() on fd[1].
	 */
	if (ioctl(fd[1], I_PUSH, "connld") < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-4);
	}
	if (fattach(fd[1], name) < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-5);
	}
	close(fd[1]);                    /* fattach holds this end open */
	return (fd[0]);			 /* fd[0] is where client connections arrive */
}

/*
 * wait for a client connection to arrive, and accept it.
 * we also obtain the client's user ID.
 * returns new fd if all ok, < 0 on error
 */
int
serv_accept(int listenfd, uid_t *uidptr)
{
	struct 	strrecvfd	recvfd;

	if (ioctl(listenfd, I_RECVFD, &recvfd) < 0)
		return (-1); 	/* could be EINTR if signal caught */
	if (uidptr != NULL)
		*uidptr = recvfd.uid;	/* effective uid of caller */
	return (recvfd.fd);	/* return the new descriptor */
}



/*
 * create a client endpoint and connect to a server.
 * returns fd if all ok, < 0 on error
 */
int
cli_conn(const char *name)
{
	int	fd;

	/* open the mounted stream */
	if ((fd = open(name, O_RDWR)) < 0)
		return (-1);
	if (isastream(fd) == 0)
	{
		close(fd);
		return (-2);
	}
	return (fd);
}

#ifndef USE_UNIX_DOMAIN_SOCKET
/*
 * pass a file descriptor to another process.
 *if fd < 0, then -fd is sent back instead as the error status
 *
 */
int
send_fd(int fd, int fd_to_send)
{
	char	buf[2];		/* send_fd() /recv_fd() 2-byte protocol */

	buf[0] = 0;		/* null byte flag to recv_fd() */
	if (fd_to_send < 0)
	{
		buf[1] = -fd_to_send;	/* nonzero status means errer */
		if (buf[1] == 0)
			buf[1] = 1;	/* -256, etc, would screw up protocol */
	}
	else
	{
		buf[1] = 0;	/* zero status means ok */
	}

	if (write(fd, buf, 2) != 2)
		return(-1);
	
	if (fd_to_send >= 0)
		if (ioctl(fd, I_SENDFD, fd_to_send) < 0)
			return(-1);
	return (0);
}

/*
 * receive a file descriptor from another proces (a server).
 * in addition, any data received from the server is passed
 * to (*userfunc) (STDERR_FILENO, buf, nbytes). we have a
 * 2-byte protocol for receiving the fd from send_fd().
 */
int
recv_fd(int fd, ssize_t (*userfunc)(int, const void *, size_t))
{
	int			newfd, nread, flag, status;
	char 			*ptr;
	char 			buf[MAXLINE];
	struct strbuf		dat;
	struct strrecvfd	recvfd;

	status = -1;
	for ( ; ; )
	{
		dat.buf = buf;
		dat.maxlen = MAXLINE;
		flag = 0;
		if (getmsg(fd, NULL, &dat, &flag) < 0)
			err_sys("getmsg error");
		nread = dat.len;
		if (nread == 0)
		{
			err_ret("connection closed by server");
			return (-1);
		}

		/* 
 		 * see if this is the final data with null & status. 
		 * Null must be next to last byte of buffer, status
		 * byte is last byte. zero status means there must 
		 * be a file descriptor to receive.
		 */
		for (ptr = buf; ptr < &buf[nread]; )
		{
			if (*ptr++ == 0)
			{
			
				if (ptr != &buf[nread - 1])
					err_dump("message format error");
				status = *ptr & 0xff;
				if (status == 0)
				{
					if (ioctl(fd, I_RECVFD, &recvfd) < 0)
						return (-1);
					newfd = recvfd.fd;
				}
				else
				{
					newfd = -status;
				}
				nread -= 2;
			}
		}
		if (nread > 0)
			if ((*userfunc)(STDERR_FILENO, buf, nread) != nread)
				return (-1);
		if (status >= 0) 		/* final data has arrived */
			return (newfd);		/* descriptor, or -status */
	}
}


/*
 * used when we had planned to send an fd using send_fd(),
 * but encountered an error instead. we send the error back
 * using the send_fd()/recv_fd() protocol
 */
int
send_err(int fd, int errcode, const char *msg)
{
	int	n;

	if ((n = strlen(msg)) > 0)
		if (write(fd, msg, n) != n)
			return(-1);

	if (errcode >= 0)
		errcode = -1;  	/* must be negative */

	if (send_fd(fd, errcode) < 0)
		return (-1);
	
	return (0);
}
#else 		/* use unix domain socket to implement send_fd send_err recv_fd */
/* size of control buffer, to send / recv one file descriptor */
#define CONTROLLEN	CMSG_LEN(sizeof(int))

static struct cmsghdr	*cmptr = NULL;	/* malloc'ed first time */

/*
 * pass a file descriptor to another process.
 * if fd < 0, then -fd is sent back instead as the error status.
 */
int
send_fd(int fd, int fd_to_send)
{
	struct iovec	iov[1];
	struct msghdr	msg;
	char 		buf[2]; /* send_fd()/recv_fd() 2-byte protocol */

	iov[0].iov_base = buf;
	iov[0].iov_len	= 2;
	msg.msg_iov	= iov;
	msg.msg_lovlen  = 1;
	msg.msg_name	= NULL;
	msg.msg_namelen = 0;
	
	if (fd_to_send < 0)
	{
		msg.msg_control		= NULL;
		msg.msg_controllen	= 0;
		buf[1]	= -fd_to_send;
		if (buf[1] == 0)
			buf[1] = 1;
	}
	else
	{
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return (-1);
		cmptr->cmsg_level	= SOL_SOCKET;
		cmptr->cmsg_type	= SCM_RIGHTS;
		cmptr->cmsg_len		= CONTROLLEN;
		msg.msg_control		= cmptr;
		msg.msg_controllen	= CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = fd_to_send;
		buf[1] = 0;
	}
	buf[0] = 0;
	if (sendmsg(fd, &msg, 0) != 2)
		return (-1);
	return (0);
}
#endif		//end of USE_UNIX_DOMAIN_SOCKET

