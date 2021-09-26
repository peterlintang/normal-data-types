

#ifndef SIMPLE_BIT_INCLUDED
#define SIMPLE_BIT_INCLUDED

#include "module.h"

#define T SBit_T	// simple bit type
typedef struct T *T;



T MODULE_FUN_NAME(sbit, create)(unsigned int num);
int MODULE_FUN_NAME(sbit, destroy)(T *sb);
int MODULE_FUN_NAME(sbit, set)(T sb, unsigned int index);
int MODULE_FUN_NAME(sbit, clear)(T sb, unsigned int index);
int MODULE_FUN_NAME(sbit, get)(T sb, unsigned int index);

#undef T

#endif
