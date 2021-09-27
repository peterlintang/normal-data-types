
/*
 * 文件名称: sentinel-linked-list.h
 * 功能描述：带哨兵的双向链表
 * 作者：lxj
 * 创建时间: 2021-09-26
 *
 */
#ifndef SENTINEL_LINKED_LIST_H
#define SENTINEL_LINKED_LIST_H

#include "module.h"

#define NODE SenDlinkNode_t
typedef struct NODE *NODE;

#define T SenDlink_T
typedef struct T *T;


/**
 * * _SenDlink_create
 * *@brief 创建带哨兵的双向链表结构，申请必要的内存;
 * *@param [in] 无
 * *@param [out] 无
 * *@return 是否成功
 * * - 成功：返回相应的结构指针
 * * - 其他：失败,返回空指针
 * */
T MODULE_FUN_NAME(SenDlink, create)(void);

/**
 * * _SenDlink_destroy
 * *@brief 销毁带哨兵的双向链表结构，释放必要的内存;
 * *@param [in] pl 双向链表头部的指针的指针
 * *@param [out] 无
 * *@return 无返回值
 * * - 成功：返回相应的结构指针
 * * - 其他：失败,返回空指针
 * */
void MODULE_FUN_NAME(SenDlink, destroy)(T *pl);

/**
 * * _SenDlink_search
 * *@brief 在带哨兵的双向链表结构中，根据@key关键字查找;
 * *@param [in] pl 双向链表头部的指针的指针, key 关键字
 * *@param [out] 无
 * *@return 返回值 0 成功
 * * - 成功：0
 * * - 其他：失败,
 * */
int MODULE_FUN_NAME(SenDlink, search)(T l, void *key);

/**
 * * _SenDlink_search
 * *@brief 在带哨兵的双向链表结构中，将@key关键字插入链表;
 * *@param [in] pl 双向链表头部的指针的指针, key 关键字
 * *@param [out] 无
 * *@return 返回值 0 成功
 * * - 成功：0
 * * - 其他：失败,
 * */
int MODULE_FUN_NAME(SenDlink, insert)(T l, void *key);

/**
 * * _SenDlink_search
 * *@brief 在带哨兵的双向链表结构中，根据@key关键字删除元素;
 * *@param [in] pl 双向链表头部的指针的指针, key 关键字
 * *@param [out] 无
 * *@return 返回值 0 成功
 * * - 成功：0
 * * - 其他：失败,
 * */
int MODULE_FUN_NAME(SenDlink, delete)(T l, void *key);

/**
 * * _SenDlink_search
 * *@brief 在带哨兵的双向链表结构中，根据@index关键字删除元素;
 * *@param [in] pl 双向链表头部的指针的指针, key 关键字
 * *@param [out] 无
 * *@return 返回值 0 成功
 * * - 成功：0
 * * - 其他：失败,
 * */
int MODULE_FUN_NAME(SenDlink, delete_by_index)(T l, unsigned int index);

/**
 * * _SenDlink_search
 * *@brief 在带哨兵的双向链表结构中，统计元素个数;
 * *@param [in] pl 双向链表头部的指针的指针, 
 * *@param [out] 无
 * *@return 返回值链表元素个数
 * * - 成功：> 0
 * * - 其他：失败,
 * */
unsigned int MODULE_FUN_NAME(SenDlink, count)(T l);

/**
 * * _SenDlink_search
 * *@brief 在带哨兵的双向链表结构中，遍历每个元素，并对每个元素调用fun
 * *@param [in] pl 双向链表头部的指针的指针, fun 用户回调函数
 * *@param [out] 无
 * *@return 返回值 0 成功
 * * - 成功：0
 * * - 其他：失败, -1
 * */
int MODULE_FUN_NAME(SenDlink, map)(T l, int (*fun)(void *arg));


#undef NODE
#undef T

#endif // end of SENTINEL_LINKED_LIST_H

