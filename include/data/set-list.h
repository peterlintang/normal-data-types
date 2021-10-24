
/*
 * 用双向链表实现集合数据结构
 * 集合中的元素应该唯一
 */

#ifndef CI_SET_LIST_H__
#define CI_SET_LIST_H__

#include "module.h"
#include "dlist.h"

#define T SetL_T

typedef struct T *T;

struct T {
	ListD_T l;
	int n;
	int (*cmp)(void *priv, void *arg);
};

/*
 * 新建一个链表集合，成功返回指针，失败NULL
 */
T MODULE_FUN_NAME(SetL, new)(int (*cmp)(void *, void *));

/*
 * 释放一个链表集合内容
 */
void MODULE_FUN_NAME(SetL, free)(T *setp);

/*
 * 将新元素priv添加到集合set中
 * 成功返回0，失败-1
 */
int MODULE_FUN_NAME(SetL, add)(T set, void *priv);

/*
 * 将元素priv从集合set中删除
 * 成功返回指针，失败NULL
 */
void *MODULE_FUN_NAME(SetL, remove)(T set, void *priv);

/*
 * 集合set中查找元素,
 * 成功返回指针，失败NULL
 */
void *MODULE_FUN_NAME(SetL, search)(T set, void *priv);

/*
 * 判断priv是否在集合set中，
 * 是返回1，不是0
 */
int MODULE_FUN_NAME(SetL, isMember)(T set, void *priv);

/*
 * 返回集合set中元素的数目
 */
int MODULE_FUN_NAME(SetL, count)(T set);

/*
 * 根据索引返回集合中的第index个元素
 * 超出元素个数返回NULL,成功返回指针
 */
void *MODULE_FUN_NAME(SetL, get)(T set, int index);

/********************集合高级操作*******************/
/*
 * 新的集合为s U t，失败NULL
 * 集合s与t的并集
 */
T MODULE_FUN_NAME(SetL, union)(T s, T t);

/*
 * 新的集合为s * t，失败NULL
 * 集合s与t的集
 */
T MODULE_FUN_NAME(SetL, inter)(T s, T t);

/*
 * 新的集合为s / t，失败NULL
 * 新集合的元素：仅在集合s,或仅在集合t
 */
T MODULE_FUN_NAME(SetL, diff)(T s, T t);

/*
 * 新的集合为s - t，失败NULL
 * 新集合的元素：在集合s,但不在t
 */
T MODULE_FUN_NAME(SetL, minus)(T s, T t);

#undef T

#endif // CI_SET_LIST_H__


