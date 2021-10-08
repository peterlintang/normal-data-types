/*************************************
 *
 * filename:	xp.h
 * description:	implement the xp operations
 * 				扩展精度算法 无符号整数算法
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
/*
 * x + y + borrow 结果保存在z，并返回最终进位的最高有效位
 * @n:		@z的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @y:		待处理的数据
 * @carry: 进位0或1
 */
extern int MODULE_FUN_NAME(XP, add)
		(int n, T z, T x, T y, int carry);

/*
 * x - y - borrow 结果保存在z，并返回最终借位的最高有效位
 * @n:		@z的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @y:		待处理的数据
 * @borrow: 借位0或1
 */
extern int MODULE_FUN_NAME(XP, sub)
		(int n, T z, T x, T y, int borrow);

/*
 * x * y 结果保存在z(写z)，
 * @z:		保存结果
 * @n:		@x的长度，
 * @x:		乘数
 * @m:		@y的长度，
 * @y:		乘数
 */
extern int MODULE_FUN_NAME(XP, mul)
		(T z, int n, T x, int m, T y);

/*
 * q = x / y, r = x % y, q 和x有n位数，r 和 y有m位数，
 * 如果y == 0， 返回0，并保持q、r不变，否则返回1,
 * tmp 必须至少能够有n+m+2位数.
 * @n:		@x的长度，
 * @x:		被除数
 * @m:		@y的长度，
 * @y:		除数
 * @q:		保存x/y的值
 * @r:		保存x%y的值
 * @tmp:	临时值？
 */
extern int MODULE_FUN_NAME(XP, div)
		(int n, T q, T x, int m, T y, T r, T tmp);

/*
 * x + y 结果保存在z，并返回最终进位的最高有效位
 * @n:		@z的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @y:		待乘数	( y > 0 && y < 2^8 )
 */
extern int MODULE_FUN_NAME(XP, sum)
		(int n, T z, T x, int y);

/*
 * x - y 结果保存在z，并返回最终借位的最高有效位
 * @n:		@z的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @y:		待乘数	( y > 0 && y < 2^8 )
 */
extern int MODULE_FUN_NAME(XP, diff)
		(int n, T z, T x, int y);


/*
 * x * y 结果保存在z，并返回最高有效位
 * @n:		@z的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @y:		待乘数	( y < 2^8 )
 */
extern int MODULE_FUN_NAME(XP, product)
		(int n, T z, T x, int y);

/*
 * x / y 结果保存在z，并返回x % y
 * @n:		@x的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @y:		除数	( y < 2^8 )
 */
extern int MODULE_FUN_NAME(XP, quotient)(int n, T z, T x, int y);

/*
 * 取反@x并加carry，结果保存在z,并返回最高有效位
 * @n:		@x的长度，
 * @z:		保存结果
 * @x:		待处理的数据
 * @carry:	待加数据
 */
extern int MODULE_FUN_NAME(XP, neg)
		(int n, T z, T x, int carry);


/*
 * 比较@x与@y，如果x > y 返回1， 等于返回0，小于返回-1
 * @n:		@x的长度，
 * @x:		待比较的数据
 * @y:		待比较的数据
 */
extern int MODULE_FUN_NAME(XP, cmp)
		(int n, T x, T y);

/*
 * 将@x左移@s位，并在新移出的位置填充@fill
 * @n:		@z的长度，
 * @z:		保存左移的结果
 * @m:		@x的长度，
 * @x:		待左移的数据,
 * @s:		左移的位数
 * @fill:	是否在新移出的位置填充值;1为填充1,0为填充0
 */
extern void MODULE_FUN_NAME(XP, lshift)
		(int n, T z, int m, T x, int s, int fill);

/*
 * 将@x右移@s位，并在新移出的位置填充@fill
 * @n:		@z的长度，
 * @z:		保存左移的结果
 * @m:		@x的长度，
 * @x:		待左移的数据,
 * @s:		右移的位数
 * @fill:	是否在新移出的位置填充值;1为填充1,0为填充0
 */
extern void MODULE_FUN_NAME(XP, rshift)
		(int n, T z, int m, T x, int s, int fill);

/*
 * 获取@x的非0数字的长度
 * @n:		@x的长度，
 * @x:		待统计的数据
 */
extern int MODULE_FUN_NAME(XP, length)(int n, T x);

/*
 * 将@u转换为以BASE为进制的数值,并返回不能存放在z里的u的位数
 * @n:		@z的长度，
 * @z:		保存转化的结构
 * @u:		待转换的数据
 */
extern unsigned long MODULE_FUN_NAME(XP, fromint)(int n, T z, unsigned long u);

/*
 * 将以BASE为进制的数值@x转换为unsigned long,并返回x % (ULONG_MAX + 1)
 * @n:		@x的长度，
 * @x:		待转化的数据
 */
extern unsigned long MODULE_FUN_NAME(XP, toint)(int n, T x);

/*
 * 将@str的数字转化为以BASE为进制的数据并保存在@z中，
 * 字符串@str中的数字按@base进制计算
 * @str:	待转化的数字串,以base为底的无符号整数字符串
 * @base:	@str字符串的进制，
 * @end:	用于返回给用户，标识转换结束的字符；
 * @n:		@z的长度，
 * @z:		转化结果
 */
extern int MODULE_FUN_NAME(XP, fromstr)(int n, T z, const char *str, int base, char **end);

/*
 * 将@x的值转化为以base为基的字符串并保存在@str中，
 * 会修改@x的内容
 * @str:	用于保存转换后的结果，
 * @size:	@str的长度字节数,
 * @base:	用于转换的进制,
 * @n:		@x的长度，
 * @x:		待转换的数据
 */
extern char *MODULE_FUN_NAME(XP, tostr)
		(char *str, int size, int base, int n, T x);


#undef T

#endif


