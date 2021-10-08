/*************************************
 *
 * filename:	ap.h
 * description:	implement the ap operations
 * 				任意精度有符号整数算法
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
/*
 * 创建一个新的AP_T,并初始化为n
 * @n 	初始化值
 * 返回AP_T
 */
extern T MODULE_FUN_NAME(AP, new)(long int n);

/*
 * 创建一个新的AP_T,以str，base为初始化值
 * @str 	初始化值
 * @base	进制
 * @end		解析str遇到非进制数字的地方
 * 返回AP_T
 */
extern T MODULE_FUN_NAME(AP, fromstr)
		(const char *str, int base, char **end);

/*
 * 释放z的内容并置空
 * @z		待释放的ap
 */
extern void MODULE_FUN_NAME(AP, free)(T *z);

/*
 * 返回～x
 * @x		待处理的ap
 */
extern T MODULE_FUN_NAME(AP, neg)(T x);

/*
 * 返回x+y
 * @x		待处理的ap
 * @y		待处理的ap
 */
extern T MODULE_FUN_NAME(AP, add)(T x, T y);

/*
 * 返回x-y
 * @x		待处理的ap
 * @y		待处理的ap
 */
extern T MODULE_FUN_NAME(AP, sub)(T x, T y);

/*
 * 返回x*y
 * @x		待处理的ap
 * @y		待处理的ap
 */
extern T MODULE_FUN_NAME(AP, mul)(T x, T y);

/*
 * 返回x/y
 * @x		待处理的ap
 * @y		待处理的ap
 */
extern T MODULE_FUN_NAME(AP, div)(T x, T y);

/*
 * 返回x%y
 * @x		待处理的ap
 * @y		待处理的ap
 */
extern T MODULE_FUN_NAME(AP, mod)(T x, T y);

/*
 * 如果p为空，返回x^y; 否则，返回(x^y)%p
 * @x		待处理的ap
 * @y		待处理的ap
 * @p		待处理的ap, p > 0 && p >> 2
 */
extern T MODULE_FUN_NAME(AP, pow)(T x, T y, T p);

/*
 * 返回x+y
 * @x		待处理的ap
 * @y		待处理的int
 */
extern T MODULE_FUN_NAME(AP, addi)(T x, long int y);

/*
 * 返回x-y
 * @x		待处理的ap
 * @y		待处理的int
 */
extern T MODULE_FUN_NAME(AP, subi)(T x, long int y);

/*
 * 返回x*y
 * @x		待处理的ap
 * @y		待处理的int
 */
extern T MODULE_FUN_NAME(AP, muli)(T x, long int y);

/*
 * 返回x/y
 * @x		待处理的ap
 * @y		待处理的int
 */
extern T MODULE_FUN_NAME(AP, divi)(T x, long int y);

/*
 * 返回x%y
 * @x		待处理的ap
 * @y		待处理的int
 */
extern long MODULE_FUN_NAME(AP, modi)(T x, long int y);

/*
 * 返回 x << s
 * @x		待处理的ap
 * @s		待处理的int (s >= 0)
 */
extern T MODULE_FUN_NAME(AP, lshift)(T x, int s);

/*
 * 返回 x >> s
 * @x		待处理的ap
 * @s		待处理的int (s >= 0)
 */
extern T MODULE_FUN_NAME(AP, rshift)(T x, int s);

/*
 * x < y, 返回< 0; x = y, 返回等于0; x > y,返回大于0
 * @x		待处理的ap
 * @y		待处理的ap
 */
extern int MODULE_FUN_NAME(AP, cmp)(T x, T y);

/*
 * x < y, 返回< 0; x = y, 返回等于0; x > y,返回大于0
 * @x		待处理的ap
 * @y		待处理的int
 */
extern int MODULE_FUN_NAME(AP, cmpi)(T x, long int y);

/*
 * 返回一个与x同符号，值为|x| mod (LONG_MAX + 1)的长整型整数
 * @x		待转换的ap
 */
extern long int MODULE_FUN_NAME(AP, toint)(T x);

/*
 * 返回一个以base为进制的字符串，数值等于x
 * @str 	保存转换的数值
 * @size	str的大小
 * @base	进制 base >= 2 && base <= 36
 * @x		待转换的ap
 */
extern char *MODULE_FUN_NAME(AP, tostr)
		(char *str, int size, int base, T x);

/*
 * 格式化输出
 */
extern void MODULE_FUN_NAME(AP, fmt)
			(int code, va_list *app,
			int put(int c, void *cl), void *cl,
			unsigned char flags[], int width, int precision);


#undef T

#endif


