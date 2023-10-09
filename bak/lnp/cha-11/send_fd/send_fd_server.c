
/*
 * test of send fd
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <linux/un.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>


int my_open(const char *pathname, int mode)
{
	int fd = -1;
	int sockfd[2];
	int status;
	pid_t childpid;
	char c;
	char argsockfd[10];
	char argmode[10];

	socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);

	if ((childpid = fork()) == 0)
	{
		close(sockfd[0]);
		snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
		snprintf(argmode, sizeof(argmode), "%d", mode);

		execl("./openfile", "openfile", argsockfd, pathname, argmode, (char *)NULL);
		fprintf(stderr, "execl error\n");
		return -1;
	}

	close(sockfd[1]);

	waitpid(childpid, &status, 0);
	if (WIFEXITED(status))
	{
		if ((status = WEXITSTATUS(status)) == 0)
		{
			recv_fd(sockfd[0], &c, 1, &fd);
		}
		else
		{
			errno = status;
			fd = -1;
		}
		close(sockfd[0]);
		return fd;
	}

	return -1;
}


#define BUFFSIZE 256
int main(int argc, char *argv[])
{
	int fd = -1;
	int n = 0;
	char buff[BUFFSIZE] = { 0 };

	if (argc != 2)
	{
		fprintf(stderr, "%s filename\n", argv[0]);
		return -1;
	}

	if ((fd = my_open(argv[1], O_RDONLY)) < 0)
	{
		fprintf(stderr, "get fd failed\n");
		return -1;
	}

	while ((n = read(fd, buff, BUFFSIZE)) > 0)
		write(1, buff, n);

	return 0;
}




