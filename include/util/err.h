

#ifndef CI_ERR_H_
#define CI_ERR_H_

#include <errno.h>
#include <stdarg.h>


/*
 *nonfatal error related to a system call.
 *print a message and return.
 */
void err_ret(const char *fmt, ...);

/*
 *fatal error related to a system call.
 *print a message and terminiate.
 */
void err_sys(const char *fmt, ...);

/*
 *fatal error unrelated to a system call.
 *error code passed as explict parameter.
 *print a message and terminate.
 */
void err_exit(int error, const char *fmt, ...);

/*
 *fatal erro related to a system call.
 *print a message dump core and terminate.
 */
void err_dump(const char *fmt, ...);


/*
 *nonfatal error unrelated to a system call.
 *print a message and return.
 */
void err_msg(const char *fmt, ...);

/*
 *fatal error unrelated to a system call.
 *print a message and terminate.
 */
void err_quit(const char *fmt, ...);


#endif // end of CI_ERR_H_



