/*************************************
 *
 * filename:	seq.h
 * description:	implement the seq operations
 * 序列，N个数值， 用0 - N-1整数索引;序列的值为指针；
 * 可以在序列两端添加或删除元素；通过索引访问；
 *
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
 * name: MODULE_FUN_NAME(Seq, new)
 * description: create a new seq
 * return value: return pointer to seq
 * args: @hint: suggested seq size, will expand auto
 */
T MODULE_FUN_NAME(Seq, new)(int hint);

/*
 * name: MODULE_FUN_NAME(Seq, seq)
 * description: create a new seq with @x ... args
 * return value: return pointer to seq
 * args: @x: args
 */
T MODULE_FUN_NAME(Seq, seq)(void *x, ...);

/*
 * name: MODULE_FUN_NAME(Seq, free)
 * description: free a seq
 * return value: return void
 * args: @seq: pointer's pointer to seq
 */
void MODULE_FUN_NAME(Seq, free)(T *seq);

/*
 * name: MODULE_FUN_NAME(Seq, length)
 * description: get the length of seq
 * return value: return length of seq
 * args: @seq: pointer to seq
 */
int MODULE_FUN_NAME(Seq, length)(T seq);

/*
 * name: MODULE_FUN_NAME(Seq, get)
 * description: get the @i index element of seq
 * return value: return @i element of seq
 * args: @seq: pointer to seq
 * 		 @i: index
 */
void *MODULE_FUN_NAME(Seq, get)(T seq, int i);

/*
 * name: MODULE_FUN_NAME(Seq, put)
 * description: put the @i index element of seq to be @x
 * return value: return @i element's prev value of seq
 * args: @seq: pointer to seq
 * 		 @i: index
 * 		 @x: value to put
 */
void *MODULE_FUN_NAME(Seq, put)(T seq, int i, void *x);

/*
 * name: MODULE_FUN_NAME(Seq, addlo)
 * description: add @x at head of seq 
 * return value: return pointer to @x
 * args: @seq: pointer to seq
 * 		 @x: value to add
 */
void *MODULE_FUN_NAME(Seq, addlo)(T seq, void *x);

/*
 * name: MODULE_FUN_NAME(Seq, addhi)
 * description: add @x at tail of seq 
 * return value: return pointer to @x
 * args: @seq: pointer to seq
 * 		 @x: value to add
 */
void *MODULE_FUN_NAME(Seq, addhi)(T seq, void *x);

/*
 * name: MODULE_FUN_NAME(Seq, rmlo)
 * description: remove the first element of seq 
 * return value: return pointer to first element
 * args: @seq: pointer to seq
 */
void *MODULE_FUN_NAME(Seq, rmlo)(T seq);

/*
 * name: MODULE_FUN_NAME(Seq, rmhi)
 * description: remove the last element of seq 
 * return value: return pointer to last element
 * args: @seq: pointer to seq
 */
void *MODULE_FUN_NAME(Seq, rmhi)(T seq);

#undef T

#endif


