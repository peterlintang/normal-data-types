

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <syslog.h>

#include "log.h"

#define MAXLINE	1024

static void log_doit(int, int, const char *, va_list);

/*
 *caller must define and set this: nonzero if 
 *interactive, zero if daemon
 */

extern int log_to_stderr;
/*for compile no error*/
//log_to_stderr = 0;

/*
 *initialize syslog(), if running as daemon
 */
void
log_open(const char *ident, int option, int facility)
{
	if (log_to_stderr == 0)
		openlog(ident, option, facility);
}

/*
 *nonfatal error related to a system call
 *print a message with the system's errno value and return
 */
void 
log_ret(const char *fmt, ...)
{
	va_list 	ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
}

/*
 *fatal error ratlated to a system call.
 *print a message and terminate.
 */
void
log_sys(const char *fmt, ...)
{
	va_list 	ap;

	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

/*
 *nonfatal error unrelated to a system call.
 *print a message and return.
 */
void
log_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
}

/*
 *fatal error unrelated to a system call.
 *print a message and terminate.
 */
void
log_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

/*
 *print a message and return to caller
 *caller specifies "errnoflag" and "priority"
 *
 */

static void
log_doit(int errnoflag, int priority, const char *fmt, va_list ap)
{
	int	errno_save;
	char 	buf[MAXLINE];

	errno_save = errno;
	vsnprintf(buf, MAXLINE, fmt, ap);
	if (errnoflag)
		snprintf(buf + strlen(buf), MAXLINE - strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	if (log_to_stderr)
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	else
	{
		syslog(priority, "%s", buf);
	}
}
