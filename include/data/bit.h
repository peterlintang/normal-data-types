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


/*
 * name: MODULE_FUN_NAME(Bit, new)
 * description: create a new bit set
 * return value: return new bit set
 * args: @length: total length of bits
 */
T MODULE_FUN_NAME(Bit, new)(int length);

/*
 * name: MODULE_FUN_NAME(Bit, free)
 * description: free a allocated bit set
 * return value: void
 * args: @length: pointer's pointer to bit set
 */
void MODULE_FUN_NAME(Bit, free)(T *set);

/*
 * name: MODULE_FUN_NAME(Bit, length)
 * description: get the length of bit set 
 * return value: length of bit set
 * args: @set: pointer to bit set
 */
int MODULE_FUN_NAME(Bit, length)(T set);

/*
 * name: MODULE_FUN_NAME(Bit, count)
 * description: get the total number of 1 in bit set 
 * return value: total number bits setted to 1 in bit set
 * args: @set: pointer to bit set
 */
int MODULE_FUN_NAME(Bit, count)(T set);

/*
 * name: MODULE_FUN_NAME(Bit, get)
 * description: get the value of @n bit in bit set
 * return value: get the value of @n bit in bit set
 * args: @set: pointer to bit set
 * 		 @n: n bit in bit set
 */
int MODULE_FUN_NAME(Bit, get)(T set, int n);

/*
 * name: MODULE_FUN_NAME(Bit, put)
 * description: put the value of @n bit in bit set to be @bit
 * return value: return the prev value of @n bit in bit set
 * args: @set: pointer to bit set
 * 		 @n: n bit in bit set
 * 		 @bit: value to set
 */
int MODULE_FUN_NAME(Bit, put)(T set, int n, int bit);

/*
 * name: MODULE_FUN_NAME(Bit, clear)
 * description: clear the @lo to @hi bits in bit set
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @lo: the start position of bit set to clear
 * 		 @hi: the end position of bit set to clear
 */
void MODULE_FUN_NAME(Bit, clear)(T set, int lo, int hi);

/*
 * name: MODULE_FUN_NAME(Bit, set)
 * description: set the @lo to @hi bits in bit set to be 1
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @lo: the start position of bit set to set
 * 		 @hi: the end position of bit set to set
 */
void MODULE_FUN_NAME(Bit, set)(T set, int lo, int hi);

/*
 * name: MODULE_FUN_NAME(Bit, not)
 * description: set the @lo to @hi bits in bit set to be ^
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @lo: the start position of bit set to set ^
 * 		 @hi: the end position of bit set to set ^
 */
void MODULE_FUN_NAME(Bit, not)(T set, int lo, int hi);

/*
 * name: MODULE_FUN_NAME(Bit, map)
 * description: for each bit in @set, call @apply
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @apply: function to call
void apply(int n(index of bit), int bit(bit value), void *cl(user's data)),
 * 		 @cl: user's private data
 */
void MODULE_FUN_NAME(Bit, map)(T set, 
				void apply(int n, int bit, void *cl),
				void *cl);
/*
 * name: MODULE_FUN_NAME(Bit, lt)
 * description: compare @s and @t, if @s is sub set of @t,
 * 			return 1, else return 0; 
 * return value: 1:  @s is sub set of @t,
 * 				0: @s is not a sub set of @t
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
int MODULE_FUN_NAME(Bit, lt)(T s, T t);

/*
 * name: MODULE_FUN_NAME(Bit, eq)
 * description: compare @s and @t are equal or not
 * return value: 1: equal; 0 : no equal
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
int MODULE_FUN_NAME(Bit, eq)(T s, T t);

/*
 * name: MODULE_FUN_NAME(Bit, leq)
 * description: compare @s and @t, if @s is sub set of @t or @s=@t,
 * 			return 1, else return 0; 
 * return value: 1:  @s is sub set of @t, or @s = @t
 * 				0: @s is not a sub set of @t
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
int MODULE_FUN_NAME(Bit, leq)(T s, T t);


/*
 * name: MODULE_FUN_NAME(Bit, union)
 * description: @s + @t or @s | @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, union)(T s, T t);

/*
 * name: MODULE_FUN_NAME(Bit, inter)
 * description: @s & @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, inter)(T s, T t);

/*
 * name: MODULE_FUN_NAME(Bit, minus)
 * description: @s &~ @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, minus)(T s, T t);

/*
 * name: MODULE_FUN_NAME(Bit, diff)
 * description: @s ^ @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, diff)(T s, T t);

#undef T

#endif
