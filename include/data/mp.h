/*************************************
 *
 * filename:	mp.h
 * description:	implement the p operations
 * author:
 * date:		2019-10-09
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_MP_INCLUDED
#define CI_MP_INCLUDED

#include <stdarg.h>
#include <stddef.h>

#define T MP_T
typedef unsigned char *T;

// < exported exceptions 359 >
#define RAISE(msg)  
typedef char *Except_T;

extern const Except_T MP_Overflow;
extern const Except_T MP_Dividedbyzero;

// < exported functions 358 >
extern int MODULE_FUN_NAME(MP, set)(int n);
extern T MODULE_FUN_NAME(MP, new)(unsigned long u);
extern T MODULE_FUN_NAME(MP, fromint)(T z, long v);
extern T MODULE_FUN_NAME(MP, fromintu)(T z, unsigned long u);
extern unsigned long MODULE_FUN_NAME(MP, tointu)(T x);
extern long MODULE_FUN_NAME(MP, toint)(T x);
extern T MODULE_FUN_NAME(MP, cvt)(int m, T z, T x);
extern T MODULE_FUN_NAME(MP, cvtu)(int m, T z, T x);

extern T MODULE_FUN_NAME(MP, add)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, sub)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, mul)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, div)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, mod)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, neg)(T z, T x);

extern T MODULE_FUN_NAME(MP, addu)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, subu)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, mulu)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, divu)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, modu)(T z, T x, T y);

extern T MODULE_FUN_NAME(MP, mul2u)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, mul2)(T z, T x, T y);

extern T MODULE_FUN_NAME(MP, addi)(T z, T x, long y);
extern T MODULE_FUN_NAME(MP, subi)(T z, T x, long y);
extern T MODULE_FUN_NAME(MP, muli)(T z, T x, long y);
extern T MODULE_FUN_NAME(MP, divi)(T z, T x, long y);

extern T MODULE_FUN_NAME(MP, addui)(T z, T x, unsigned long y);
extern T MODULE_FUN_NAME(MP, subui)(T z, T x, unsigned long y);
extern T MODULE_FUN_NAME(MP, mului)(T z, T x, unsigned long y);
extern T MODULE_FUN_NAME(MP, divui)(T z, T x, unsigned long y);

extern long MODULE_FUN_NAME(MP, modi)(T x, long y);
extern unsigned long MODULE_FUN_NAME(MP, modui)
		(T x, unsigned long y);

extern int MODULE_FUN_NAME(MP, cmp)(T x, T y);
extern int MODULE_FUN_NAME(MP, cmpi)(T x, long y);

extern int MODULE_FUN_NAME(MP, cmpu)(T x, T y);
extern int MODULE_FUN_NAME(MP, cmpui)(T x, unsigned long y);

extern T MODULE_FUN_NAME(MP, and)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, or)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, xor)(T z, T x, T y);
extern T MODULE_FUN_NAME(MP, not)(T z, T x);

extern T MODULE_FUN_NAME(MP, andi)(T z, T x, unsigned long y);
extern T MODULE_FUN_NAME(MP, ori)(T z, T x, unsigned long y);
extern T MODULE_FUN_NAME(MP, xori)(T z, T x, unsigned long y);

extern T MODULE_FUN_NAME(MP, lshift)(T z, T x, int s);
extern T MODULE_FUN_NAME(MP, rshift)(T z, T x, int s);
extern T MODULE_FUN_NAME(MP, ashift)(T z, T x, int s);

extern T MODULE_FUN_NAME(MP, fromstr)(T z, const char *str, 
				int base, char **end);
extern char *MODULE_FUN_NAME(MP, tostr)(char *str, int size, 
				int base, T x);

extern void MODULE_FUN_NAME(MP, fmt)(int code, va_list *app,
				int put(int c, void *cl), void *cl,
				unsigned char flags[], int width, int precision);
extern void MODULE_FUN_NAME(MP, fmtu)(int code, va_list *app,
				int put(int c, void *cl), void *cl,
				unsigned char flags[], int width, int precision);

#undef T

#endif


