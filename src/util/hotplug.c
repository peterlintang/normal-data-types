
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <errno.h>

int hotPlutInit(void)
{
	struct sockaddr_nl snl;
	const int bufferSize = 1 * 1024 * 1024;
	int ret = 0;
	int sock = -1;

	memset(&snl, 0, sizeof(snl));
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid();
	snl.nl_groups = 1;

	sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (sock < 0)
	{
		fprintf(stderr, "create socket failed: %s\n", strerror(errno));
		return -1;
	}

	setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
	ret = bind(sock, (struct sockaddr *)&snl, sizeof(snl));
	if (ret < 0)
	{
		fprintf(stderr, "bind failed: %s\n", strerror(errno));
		close(sock);
		return -1;
	}

	return sock;
}

int hotPlugDeinit(int sock)
{
	if (sock > 0)
		close(sock);
	return 0;
}

int main(int argc, char *argv[])
{
	int hotPlugSock = -1;
	int len = 0;
	char buf[1024 * 2] = { 0 };
	int ret = 0;
	int events_count = 0;

	hotPlugSock = hotPlutInit();

	while (events_count < 5)
	{
		len = recv(hotPlugSock, buf, sizeof(buf), 0);
		if (len <= 0)
		{
			fprintf(stderr, "recv error: %s\n", strerror(errno));
			break;
		}
		else
		{
			events_count++; 
		}
		fprintf(stdout, "buf: %s\n", buf);
	}

	hotPlugDeinit(hotPlugSock);

	return 0;
}


