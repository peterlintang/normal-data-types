

/*
 * log函数
 */
#ifndef CI_LOG_H_
#define CI_LOG_H_


/*
 *initialize syslog(), if running as daemon
 */
void log_open(const char *ident, int option, int facility);

/*
 *nonfatal error related to a system call
 *print a message with the system's errno value and return
 */
void log_ret(const char *fmt, ...);

/*
 *fatal error ratlated to a system call.
 *print a message and terminate.
 */
void log_sys(const char *fmt, ...);

/*
 *nonfatal error unrelated to a system call.
 *print a message and return.
 */
void log_msg(const char *fmt, ...);

/*
 *fatal error unrelated to a system call.
 *print a message and terminate.
 */
void log_quit(const char *fmt, ...);


#endif // end of CI_LOG_H_


