/*************************************
 *
 * filename:	bit.h
 * description:	implement the bit operations
 * author:
 * date:		2019-09-29
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_BIT_INCLUDED
#define CI_BIT_INCLUDED

#include "module.h"

#define T Bit_T
typedef struct T *T;

// < function 172 >

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
extern T MODULE_FUN_NAME(Bit, new)(int length);
extern void MODULE_FUN_NAME(Bit, free)(T *set);

extern int MODULE_FUN_NAME(Bit, length)(T set);
extern int MODULE_FUN_NAME(Bit, count)(T set);

extern int MODULE_FUN_NAME(Bit, get)(T set, int n);
extern int MODULE_FUN_NAME(Bit, put)(T set, int n, int bit);

extern void MODULE_FUN_NAME(Bit, clear)(T set, int lo, int hi);
extern void MODULE_FUN_NAME(Bit, set)(T set, int lo, int hi);
extern void MODULE_FUN_NAME(Bit, not)(T set, int lo, int hi);

extern int MODULE_FUN_NAME(Bit, lt)(T s, T t);
extern int MODULE_FUN_NAME(Bit, eq)(T s, T t);
extern int MODULE_FUN_NAME(Bit, leq)(T s, T t);

extern void MODULE_FUN_NAME(Bit, map)(T set, 
				void apply(int n, int bit, void *cl),
				void *cl);

extern T MODULE_FUN_NAME(Bit, union)(T s, T t);
extern T MODULE_FUN_NAME(Bit, inter)(T s, T t);
extern T MODULE_FUN_NAME(Bit, minus)(T s, T t);
extern T MODULE_FUN_NAME(Bit, diff)(T s, T t);

#undef T

#endif
