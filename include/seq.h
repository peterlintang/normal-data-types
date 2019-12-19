/*************************************
 *
 * filename:	seq.h
 * description:	implement the seq operations
 * author:
 * date:		2019-09-29
 * version:		0.0.1
 *
 * interfaces:
 *
 *************************************/

#ifndef CI_SEQ_INCLUDED
#define CI_SEQ_INCLUDED

#include "module.h"

#define T Seq_T
typedef struct T *T;

// < function 172 >

/* 
 * interface: 
 * description:  
 * args: 
 * return value: 
 */
extern T MODULE_FUN_NAME(Seq, new)(int hint);
extern T MODULE_FUN_NAME(Seq, seq)(void *x, ...);
extern void MODULE_FUN_NAME(Seq, free)(T *seq);

extern int MODULE_FUN_NAME(Seq, length)(T seq);
extern void *MODULE_FUN_NAME(Seq, get)(T seq, int i);
extern void *MODULE_FUN_NAME(Seq, put)(T seq, int i, void *x);
extern void *MODULE_FUN_NAME(Seq, addlo)(T seq, void *x);
extern void *MODULE_FUN_NAME(Seq, addhi)(T seq, void *x);
extern void *MODULE_FUN_NAME(Seq, rmlo)(T seq);
extern void *MODULE_FUN_NAME(Seq, rmhi)(T seq);

#undef T

#endif
