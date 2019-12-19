/*************************************
 *
 * filename:	text.h
 * description:	implement the text operations
 * author:
 * date:		2019-10-08
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_TEXT_INCLUDED
#define CI_TEXT_INCLUDED

#include <stdarg.h>

#include "module.h"

#define T Text_T

// < exported types 270 >
typedef struct T {
	int len;
	const char *str;
} T;

typedef struct Text_save_T *Text_save_T;

// < exported data 274 >
extern const T Text_cset;
extern const T Text_ascii;
extern const T Text_ucase;
extern const T Text_lcase;
extern const T Text_digits;
extern const T Text_null;

// < exported functions 271 >
extern T MODULE_FUN_NAME(Text, put)(const char *str);
extern char *MODULE_FUN_NAME(Text, get)
		(char *str, int size, T s);
extern T MODULE_FUN_NAME(Text, box)(const char *str, int len);
extern T MODULE_FUN_NAME(Text, sub)(T s, int i, int j);
extern int MODULE_FUN_NAME(Text, pos)(T s, int i);
extern T MODULE_FUN_NAME(Text, cat)(T s1, T s2);
extern T MODULE_FUN_NAME(Text, dup)(T s, int n);
extern T MODULE_FUN_NAME(Text, reverse)(T s);
extern T MODULE_FUN_NAME(Text, map)
		(T s, const T *from, const T *to);
extern int MODULE_FUN_NAME(Text, cmp)(T s1, T s2);
extern int MODULE_FUN_NAME(Text, chr)
		(T s, int i, int j, int c);
extern int MODULE_FUN_NAME(Text, rchr)
		(T s, int i, int j, int c);
extern int MODULE_FUN_NAME(Text, upto)
		(T s, int i, int j, T set);
extern int MODULE_FUN_NAME(Text, rupto)
		(T s, int i, int j, T set);
extern int MODULE_FUN_NAME(Text, many)
		(T s, int i, int j, T set);
extern int MODULE_FUN_NAME(Text, rmany)
		(T s, int i, int j, T set);
extern int MODULE_FUN_NAME(Text, find)
		(T s, int i, int j, T str);
extern int MODULE_FUN_NAME(Text, rfind)
		(T s, int i, int j, T str);
extern int MODULE_FUN_NAME(Text, match)
		(T s, int i, int j, T str);
extern int MODULE_FUN_NAME(Text, rmatch)
		(T s, int i, int j, T str);
extern int MODULE_FUN_NAME(Text, any)(T s, int i, T set);
extern void MODULE_FUN_NAME(Text, fmt)
		(int code, va_list *app,
				int put(int c, void *cl), void *cl,
				unsigned char flags[], int width, int precision);

extern Text_save_T MODULE_FUN_NAME(Text, save)(void);
extern void MODULE_FUN_NAME(Text, restore)(Text_save_T *save);

#undef T

#endif


