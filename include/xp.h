/*************************************
 *
 * filename:	xp.h
 * description:	implement the xp operations
 * author:
 * date:		2019-10-09
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_XP_INCLUDED
#define CI_XP_INCLUDED

#include "module.h"

#define T XP_T

typedef unsigned char *T;

// < exported functions 299 >
extern int MODULE_FUN_NAME(XP, add)
		(int n, T z, T x, T y, int carry);
extern int MODULE_FUN_NAME(XP, sub)
		(int n, T z, T x, T y, int borrow);
extern int MODULE_FUN_NAME(XP, mul)
		(T z, int n, T x, int m, T y);
extern int MODULE_FUN_NAME(XP, div)
		(int n, T q, T x, int m, T y, T r, T tmp);

extern int MODULE_FUN_NAME(XP, sum)
		(int n, T z, T x, int y);
extern int MODULE_FUN_NAME(XP, diff)
		(int n, T z, T x, int y);
extern int MODULE_FUN_NAME(XP, product)
		(int n, T z, T x, int y);
extern int MODULE_FUN_NAME(XP, quotient)(int n, T z, T x, int y);

extern int MODULE_FUN_NAME(XP, neg)
		(int n, T z, T x, int carry);
extern int MODULE_FUN_NAME(XP, cmp)
		(int n, T x, T y);
extern void MODULE_FUN_NAME(XP, lshift)
		(int n, T z, int m, T x, int s, int fill);
extern void MODULE_FUN_NAME(XP, rshift)
		(int n, T z, int m, T x, int s, int fill);

extern int MODULE_FUN_NAME(XP, length)(int n, T x);
extern unsigned long MODULE_FUN_NAME(XP, fromint)(int n, T z, unsigned long u);
extern unsigned long MODULE_FUN_NAME(XP, toint)(int n, T x);

extern int MODULE_FUN_NAME(XP, fromstr)(int n, T z, const char *str, int base, char **end);
extern char *MODULE_FUN_NAME(XP, tostr)
		(char *str, int size, int base, int n, T x);


#undef T

#endif


