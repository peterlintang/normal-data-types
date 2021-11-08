#include "apue.h"
#include <signal.h>
#include <errno.h>
/*
#define SIGBAD(signo) ((signo) <=0) || (signo) >= NSIG)

int sigaddset(sigset_t *set, int signo)
{
	if (SIGBAD(signo))
	{
		errno = EINVAL; 
		return (-1);
	}

	*set |= 1 << (signo - 1);
	return (0);
}

int sigdelset(sigset_t *set, int signo)
{
	if (SIGBAD(signo))
        {
                errno = EINVAL;
                return (-1);
        }

	*set &= ~(1 << (signo -1 ))
	return (0);
}

int sigismember(const sigset_t *set, int signo)
{
	if (SIGBAD(signo))
        {
                errno = EINVAL;
                return (-1);
	}

	return ((*set $ (1 << (signo - 1))) != 0);
}
*/

Sigfunc *
signal(int signo, Sigfunc* func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM)
	{
    #ifdef SA_INTERRUPT
    		act.sa_flags |= SA_INTERRUPT;
    #endif
	}
	else
	{
    #ifdef SA_RESTART
    		act.sa_flags |= SA_RESTART;
    #endif
	}
	if (sigaction(signo, &act, &oact) < 0)
	{
		return (SIG_ERR);
	}
	return (oact.sa_handler);
	
}


/*
 *
 *another implementation of signal function call
 *which will try hard to avoid restart system call
 */
Sigfunc *
signal_intr(int signo, Sigfunc *func)
{
	struct sigaction 	act, oact;

	act.sa_handler  =  func;
	sigemptyset(&act.sa_mask);
	act.sa_flags  =  0;
#ifdef SA_INTERRUPT
	act.sa_flags |=  SA_INTERRUPT;
#endif
	if (sigaction(signo, &act, &oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}





