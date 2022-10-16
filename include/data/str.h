/*************************************
 *
 * filename:	str.h
 * description:	implement the str operations
 * author:
 * date:		2019-09-30
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_STR_INCLUDED
#define CI_STR_INCLUDED

#include <stdarg.h>

#include "module.h"
#include "dlist.h"

#define T Str_T

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
extern char *MODULE_FUN_NAME(Str, sub)
		(const char *s, int i, int j);
extern char *MODULE_FUN_NAME(Str, dup)
		(const char *s, int i, int j, int n);
extern char *MODULE_FUN_NAME(Str, cat)
		(const char *s1, int i1, int j1,
				const char *s2, int i2, int j2);
extern char *MODULE_FUN_NAME(Str, catv)
		(const char *s, ...);
extern char *MODULE_FUN_NAME(Str, reverse)
		(const char *s, int i, int j);
extern char *MODULE_FUN_NAME(Str, map)
		(const char *s, int i, int j,
				const char *from, const char *to);

extern int MODULE_FUN_NAME(Str, pos)(const char *s, int i);
extern int MODULE_FUN_NAME(Str, len)
		(const char *s, int i, int j);
extern int MODULE_FUN_NAME(Str, cmp)
		(const char *s1, int i1, int j1,
				const char *s2, int i2, int j2);

extern int MODULE_FUN_NAME(Str, chr)
		(const char *s, int i, int j, int c);
extern int MODULE_FUN_NAME(Str, rchr)
		(const char *s, int i, int j, int c);
extern int MODULE_FUN_NAME(Str, upto)
		(const char *s, int i, int j, const char *set);
extern int MODULE_FUN_NAME(Str, rupto)
		(const char *s, int i, int j, const char *set);
extern int MODULE_FUN_NAME(Str, find)
		(const char *s, int i, int j, const char *str);
extern int MODULE_FUN_NAME(Str, rfind)
		(const char *s, int i, int j, const char *str);

extern int MODULE_FUN_NAME(Str, any)
		(const char *s, int i,
				const char *set);
extern int MODULE_FUN_NAME(Str, many)
		(const char *s, int i, int j,
				const char *set);
extern int MODULE_FUN_NAME(Str, rmany)
		(const char *s, int i, int j,
				const char *set);
extern int MODULE_FUN_NAME(Str, match)
		(const char *s, int i, int j,
				const char *str);
extern int MODULE_FUN_NAME(Str, rmatch)
		(const char *s, int i, int j,
				const char *str);
extern ListD_T MODULE_FUN_NAME(Str, tokenize)(const char *buf, const char *delim);

extern void MODULE_FUN_NAME(Str, fmt)
		(int code, va_list *app, 
				int put(int c, void *cl), void *cl,
				unsigned char flags[], int width, int precision);


#undef T

#endif


