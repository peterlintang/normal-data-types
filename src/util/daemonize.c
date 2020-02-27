#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

/*
 *can be called by process which want to be
 *a daemon process
 */
void 
daemonize(const char *cmd)
{
	int		 	i, fd0, fd1, fd2;
	pid_t		 	pid;
	struct rlimit		rl;
	struct sigaction 	sa;

	/*
	 *clear file creation mask
	 */
	umask(0);

	/*
	 *get maximum number of file descriptors
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
	{
		fprintf(stderr, "%s: can't get file limit\n", cmd);
		exit(1);
	}

	/*
	 *become a session leader to lose controlling tty
	 */
	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "%s: can't fork\n", cmd);
		exit(1);
	}
	else if (pid != 0)
		exit(0);
	setsid();
	
	/*
	 *ensure future opens won't allocate controlling ttys
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
	{
		fprintf(stderr, "%s: can't ignore SIGHUP\n", cmd);
		exit(1);
	}
	if ((pid = fork()) < 0)
	{
		fprintf(stderr, "%s: can't fork\n", cmd);
		exit(1);
	}
	else if (pid != 0)
		exit(0);

	/*
	 *change the current working dir to the root so we won't prevent file system from being unmounted
	 */
	if (chdir("/") < 0)
	{
		fprintf(stderr, "%s: can't change dire to /\n", cmd);
		exit(1);
	}

	/*
	 *close all open file descriptors
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);
	
	/*
	 *attach file descriptor 0, 1 and 2 to /dev/null
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 *initialize the log file
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}





