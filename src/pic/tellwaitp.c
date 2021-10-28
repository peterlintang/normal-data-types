/*
 * 父子进程同步原语
 */
#include "tellwaitp.h"

static int pfd1[2], pfd2[2];

void
TELL_WAIT_P(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
		err_sys("pipe error");
}

void
TELL_PARENT_P(pid_t pid)
{
	if (write(pfd2[1], "c", 1) != 1)
		err_sys("pipe error");
}

void
WAIT_PARENT_P(void)
{
	char 	c;

	if (read(pfd1[0], &c, 1) != 1)
		err_sys("read error");

	if (c != 'p')
		err_quit("WAIT_PARENT: incorrect data");
}

void
TELL_CHILD_P(pid_t pid)
{
	if (write(pfd1[1], "p", 1) != 1)
		err_sys("write error");
}

void
WAIT_CHILD_P(void)
{
	char	c;

	if (read(pfd2[0], &c, 1) != 1)
		err_sys("read error");

	if (c != 'c')
		err_quit("WAIT_CHILD: incorrect data");
}


