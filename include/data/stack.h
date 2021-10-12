
/*
 * 10-1 
 * implement the stack described by this sector
 */

#ifndef CI_STACK_HEADER_
#define CI_STACK_HEADER_

#include "module.h"


#define T Stack_T

typedef struct T *T;

/*
 * 初始化一个堆栈,成功返回指针，失败返回空指针
 * size: 堆栈大小
 */
T MODULE_FUN_NAME(Stack, new)(int size);

/*
 * 销毁一个堆栈结构
 * sp: 堆栈的指针
 */
void MODULE_FUN_NAME(Stack, free)(T *sp);

/*
 * 判断堆栈是否为空，是返回1，否返回0
 * sp: 堆栈的指针
 */
int MODULE_FUN_NAME(Stack, isEmpty)(T s);

/*
 * 往堆栈压入数据, 成功返回0，失败返回-1
 * sp: 堆栈的指针
 * x: 待压入的数据
 */
int MODULE_FUN_NAME(Stack, push)(T s, void *x);

/*
 * 从堆栈取出数据, 成功返回0，失败返回-1
 * sp: 堆栈的指针
 * xp: 存放数据指针的地方
 */
int MODULE_FUN_NAME(Stack, pop)(T s, void **xp);

#undef T

#endif // end of CI_STACK_HEADER_


