

#ifndef __mysql_H__
#define __mysql_H__


#include <mysql/mysql.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdint.h>

#include "module.h"

extern my_bool my_init(void);

MYSQL *MODULE_FUN_NAME(MySql, connect)(char *host, char *user, char *pass, char *dbname,
								uint32_t port, char *socket, uint32_t flags);
void MODULE_FUN_NAME(MySql, close)(MYSQL *);
int32_t MODULE_FUN_NAME(MySql, query)(MYSQL *, const char *sql);
int32_t MODULE_FUN_NAME(MySql, execute)(MYSQL *, const char *sql, MYSQL_RES **);
int32_t MODULE_FUN_NAME(MySql, executeFunc)(MYSQL *, const char *funcname, MYSQL_RES **, va_list);
int32_t MODULE_FUN_NAME(MySql, executeFunc2)(MYSQL *, const char *funcname, MYSQL_RES **, char **, int);
int32_t MODULE_FUN_NAME(MySql, executeFunc3)(MYSQL *, const char *funcname, MYSQL_RES **, char **, char**, int);
int32_t MODULE_FUN_NAME(MySql, executeProc)(MYSQL *, const char *procname, MYSQL_RES **, va_list);
MYSQL_RES *MODULE_FUN_NAME(MySql, getResult)(MYSQL *);
void MODULE_FUN_NAME(MySql, freeResult)(MYSQL_RES *);
int32_t MODULE_FUN_NAME(MySql, countFields)(MYSQL *);
int32_t MODULE_FUN_NAME(MySql, numRows)(MYSQL_RES *);
MYSQL_ROW  MODULE_FUN_NAME(MySql, getRow)(MYSQL_RES *);
char *MODULE_FUN_NAME(MySql, timestamp)(struct timeval *);
char *MODULE_FUN_NAME(MySql, escapeString)(MYSQL *, const char *string);
int32_t MODULE_FUN_NAME(MySql, snprintf)(MYSQL *, char *, size_t, const char *format,...);
int32_t MODULE_FUN_NAME(MySql, vsnprintf)(MYSQL *, char *, size_t, const char *format, va_list);

#endif	/* __mysql_H__ */


