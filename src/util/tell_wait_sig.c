

/*
 *  process sync implemented by signal
 *  进程 同 步, 用 信号 实现 
 *  used by parent & child process mainly
 */
#include "apue.h"

static volatile sig_atomic_t	sigflag;        /* set nonzero by sig handler, will be set to zero by compiler ? */
static sigset_t			newmask;        /* for new sig mask */
static sigset_t			oldmask;        /* for save old sig mask */
static sigset_t			zeromask;       /* for zero sig mask, use by sigsuspend */

static void sig_usr(int signo)
{
	sigflag	= 1;
}

void TELL_WAIT(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("signal (SIGUSR1) error");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("signal (SIGUSR2) error");

	sigemptyset(&newmask);
	sigemptyset(&zeromask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);

	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);
}

void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);
}

void WAIT_PARENT(void)
{
	while (sigflag == 0) 
		sigsuspend(&zeromask);

	sigflag = 0;

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
}

void WAIt_CHILD(void)
{
	while (sigflag == 0)
		sigsuspend(&zeromask);

	sigflag = 0;

	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_RETMASK error");
}
