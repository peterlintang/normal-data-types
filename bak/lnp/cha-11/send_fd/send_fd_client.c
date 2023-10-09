
/*
 * test of client
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


extern ssize_t send_fd(int fd, void *data, size_t bytes, int send_fd);

int main(int argc, char *argv[])
{
	int fd = -1;
	ssize_t n = 0;

	if (argc != 4)
	{
		fprintf(stderr, "socketpair error\n");
		return -1;
	}

	if ((fd = open(argv[2], atoi(argv[3]))) < 0)
	{
		fprintf(stderr, "open %s error: %d\n", argv[2], fd);
		return -1;
	}

	n = send_fd(atoi(argv[1]), "", 1, fd);
	if (n < 0)
	{
		fprintf(stderr, "send fd: %d, failed: %d\n", fd, n);
		return -1;
	}

	return 0;
}





