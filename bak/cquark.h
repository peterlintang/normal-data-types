/*
 * this implements the control of quarks
 * hope it is useful ^^
 */
#ifndef __cquark_H__
#define __cquark_H__

#include "quark.h"

#define CQUARK_MAX_NAMELEN	64
#define CQUARK_DEFAULT_TIMEOUT	100

typedef struct __cquark_item_t {
	quark_t *quark;
	struct __cquark_item_t *next;
	struct __cquark_item_t *prev;
} cquark_item_t;

/* cquark item interfaces */
cquark_item_t *_new_cquark_item(quark_t *quark);
void _destroy_cquark_item(cquark_item_t **old_ptr);

typedef struct __cquark_t {
	struct __cquark_item_t *head;
	struct __cquark_item_t *tail;
	unsigned int count;
	char name[CQUARK_MAX_NAMELEN];
} cquark_t;

/* cquark interfaces */
cquark_t *_new_cquark(const char *name);
signed int _count_cquark(cquark_t *cquark);
void _setname_cquark(cquark_t *quark, const char *name);
cquark_item_t *_insert_item_after_cquark(
			cquark_t *cquark, 
			cquark_item_t *new, 
			cquark_item_t *after);

cquark_item_t *_insert_item_before_cquark(
			cquark_t *cquark, 
			cquark_item_t *new, 
			cquark_item_t *before);

cquark_item_t *_del_item_from_cquark(cquark_t *cquark,
				cquark_item_t *old);
cquark_item_t *_get_head_cquark(cquark_t *cquark);
cquark_item_t *_get_tail_cquark(cquark_t *cquark);
cquark_item_t *_search_item_cquark(cquark_t *cquark,
				const char *name,
				cquark_item_t *start);
cquark_item_t *_search_item_by_id_cquark(cquark_t *cquark,
				pthread_t id,
				cquark_item_t *start);
cquark_item_t *_search_item_reverse_cquark(cquark_t *cquark,
				const char *name,
				cquark_item_t *start);
cquark_item_t *_search_item_by_id_reverse_cquark(cquark_t *cquark,
				pthread_t id,
				cquark_item_t *start);
void _destroy_cquark(cquark_t **cquark_ptr);

#endif
