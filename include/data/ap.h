/*************************************
 *
 * filename:	ap.h
 * description:	implement the ap operations
 * author:
 * date:		2019-10-09
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_AP_INCLUDED
#define CI_AP_INCLUDED

#include <stdarg.h>

#include "module.h"

#define T AP_T
typedef struct T *T;

// <functions 324>
extern T MODULE_FUN_NAME(AP, new)(long int n);
extern T MODULE_FUN_NAME(AP, fromstr)
		(const char *str, int base, char **end);
extern void MODULE_FUN_NAME(AP, free)(T *z);

extern T MODULE_FUN_NAME(AP, neg)(T x);
extern T MODULE_FUN_NAME(AP, add)(T x, T y);
extern T MODULE_FUN_NAME(AP, sub)(T x, T y);
extern T MODULE_FUN_NAME(AP, mul)(T x, T y);
extern T MODULE_FUN_NAME(AP, div)(T x, T y);
extern T MODULE_FUN_NAME(AP, mod)(T x, T y);
extern T MODULE_FUN_NAME(AP, pow)(T x, T y, T p);

extern T MODULE_FUN_NAME(AP, addi)(T x, long int y);
extern T MODULE_FUN_NAME(AP, subi)(T x, long int y);
extern T MODULE_FUN_NAME(AP, muli)(T x, long int y);
extern T MODULE_FUN_NAME(AP, divi)(T x, long int y);
extern long MODULE_FUN_NAME(AP, modi)(T x, long int y);

extern T MODULE_FUN_NAME(AP, lshift)(T x, int s);
extern T MODULE_FUN_NAME(AP, rshift)(T x, int s);

extern int MODULE_FUN_NAME(AP, cmp)(T x, T y);
extern int MODULE_FUN_NAME(AP, cmpi)(T x, long int y);

extern long int MODULE_FUN_NAME(AP, toint)(T x);
extern char *MODULE_FUN_NAME(AP, tostr)
		(char *str, int size, int base, T x);
extern void MODULE_FUN_NAME(AP, fmt)
			(int code, va_list *app,
			int put(int c, void *cl), void *cl,
			unsigned char flags[], int width, int precision);


#undef T

#endif


