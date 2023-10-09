
/*
 * open a file and send it's file description 
 * to another process by unix socket
 *
 * filename: send_fd_common.c
 * date: 2022-09-01
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

ssize_t send_fd(int fd, void *data, size_t bytes, int sendfd)
{
	struct msghdr msghdr_send;
	struct iovec iov[1];
	size_t n;

	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;

	struct cmsghdr *pcmsghdr = NULL;
	msghdr_send.msg_control = control_un.control;
	msghdr_send.msg_controllen = sizeof(control_un.control);

	pcmsghdr = CMSG_FIRSTHDR(&msghdr_send);
	pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
	pcmsghdr->cmsg_level = SOL_SOCKET;
	pcmsghdr->cmsg_type = SCM_RIGHTS;
	*((int*)CMSG_DATA(pcmsghdr)) = sendfd;

	msghdr_send.msg_name = NULL;
	msghdr_send.msg_namelen = 0;

	iov[0].iov_base = data;
	iov[0].iov_len = bytes;

	msghdr_send.msg_iov = iov;
	msghdr_send.msg_iovlen = 1;

	return sendmsg(fd, &msghdr_send, 0);
}

ssize_t recv_fd(int fd, void *data, size_t bytes, int *recvfd)
{
	struct msghdr msghdr_recv;
	struct iovec iov[1];
	size_t n;

	union {
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;

	struct cmsghdr *pcmsghdr = NULL;
	msghdr_recv.msg_control = control_un.control;
	msghdr_recv.msg_controllen = sizeof(control_un.control);

	msghdr_recv.msg_name = NULL;
	msghdr_recv.msg_namelen = 0;

	iov[0].iov_base = data;
	iov[0].iov_len = bytes;

	msghdr_recv.msg_iov = iov;
	msghdr_recv.msg_iovlen = 1;

	if ((n = recvmsg(fd, &msghdr_recv, 0)) <= 0)
	{
		fprintf(stderr, "failed to recv fd: %d\n", (int)n);
		return n;
	}

	pcmsghdr = CMSG_FIRSTHDR(&msghdr_recv);
	if (pcmsghdr != NULL)
	{
		pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int));

		if (pcmsghdr->cmsg_level != SOL_SOCKET)
		{
			fprintf(stderr, "control level != SOL_SOCKET\n");
			return -1;
		}

		if (pcmsghdr->cmsg_type != SCM_RIGHTS)
		{
			fprintf(stderr, "control type != SCM_RIGHTS\n");
			return -1;
		}

		*recvfd = *((int *)CMSG_DATA(pcmsghdr));
	}
	else
	{
		*recvfd = -1;
	}

	return n;
}



