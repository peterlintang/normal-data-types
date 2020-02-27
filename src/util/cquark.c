/*
 * this implements the interfaces of control quark
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cquark.h"

/* cquark_item_t interfaces */
cquark_item_t *_new_cquark_item(quark_t *quark)
{
	cquark_item_t *new = calloc(1, sizeof(cquark_item_t));
	if (new) {
		new->quark = quark;
	}
	return new;
}

void _destroy_cquark_item(cquark_item_t **old_ptr)
{
	if (old_ptr && *old_ptr) {
		_stop_quark((*old_ptr)->quark, CQUARK_DEFAULT_TIMEOUT);
		_destroy_quark(&(*old_ptr)->quark);
		free(*old_ptr);
		*old_ptr = NULL;
	}
}

/* cquark interfaces */
cquark_t *_new_cquark(const char *name)
{
	cquark_t *cquark = calloc(1, sizeof(cquark_t));
	if (cquark) {
		strncpy(cquark->name, name, CQUARK_MAX_NAMELEN);
	}
	return cquark;
}

signed int _count_cquark(cquark_t *cquark)
{
	return cquark ? cquark->count : -1;
}

void _setname_cquark(cquark_t *cquark, const char *name)
{
	if (cquark && name) {
		strncpy(cquark->name, name, CQUARK_MAX_NAMELEN);
	}
}

cquark_item_t *_insert_item_after_cquark(
			cquark_t *cquark,
			cquark_item_t *new,
			cquark_item_t *after)
{
	if (cquark && new) {
		after = after ? after : cquark->tail;
		if (after) {
			new->prev = after;
			new->next = after->next;
			if (after->next)
				after->next->prev = new;
			else
				cquark->tail = new;
		} else {
			new->prev = NULL;
			new->next = NULL;
			cquark->head = new;
			cquark->tail = new;
		}
		cquark->count++;
		return new;
	}
	return NULL;
}

cquark_item_t *_insert_item_before_cquark(
			cquark_t *cquark,
			cquark_item_t *new,
			cquark_item_t *before)
{
	if (cquark && new) {
		before = before ? before : cquark->head;
		if (before) {
			new->prev = before->prev;
			new->next = before;
			if (before->prev)
				before->prev->next = new;
			else
				cquark->head = new;
			before->prev = new;
		} else {
			new->prev = NULL;
			new->next = NULL;
			cquark->head = new;
			cquark->tail = new;
		}
		cquark->count++;
		return new;
	}
	return NULL;
}

cquark_item_t *_del_item_from_cquark(cquark_t *cquark, cquark_item_t *old)
{
	if (cquark && old) {
		if (old->prev) {
			old->prev->next = old->next;
		} else {
			cquark->head = old->next;
		}

		if (old->next) {
			old->next->prev = old->prev;
		} else {
			cquark->tail = old->prev;
		}
		cquark->count--;
		return old;
	}
	return NULL;
}

/* the caller should destroy the cquark_item_t returned */
cquark_item_t *_get_head_cquark(cquark_t *cquark)
{
	if (cquark) {
		cquark_item_t *item = cquark->head;
		if (item && !_del_item_from_cquark(cquark, item)) {
			fprintf(stdout, "%s : cannot del item from cquark\n", __func__);
//			return item;
			return NULL;
		}
		return item;
	}
	return NULL;
//	return cquark ? cquark->head : NULL;
}

/* the caller should destroy the cquark_item_t returned */
cquark_item_t *_get_tail_cquark(cquark_t *cquark)
{
	if (cquark) {
		cquark_item_t *item = cquark->tail;
		if (!_del_item_from_cquark(cquark, item))
			return NULL;
		return item;
	}
	return NULL;
//	return cquark ? cquark->tail : NULL;
}

cquark_item_t *_search_item_cquark(
			cquark_t *cquark, 
			const char *name, 
			cquark_item_t *start)
{
	if (cquark && name) {
		unsigned int len = strlen(name);
		start = start ? start : cquark->head;
		while (start) {
			if ((strlen(start->quark->name) == len) &&
				!strcmp(name, start->quark->name)) 
				break;
			start = start->next;
		}
		return start;
	}
	return NULL;
}

cquark_item_t *_search_item_by_id_cquark(cquark_t *cquark,
					pthread_t id,
					cquark_item_t *start)
{
	if (cquark && id) {
		start = start ? start : cquark->head;
		while (start) {
			if (pthread_equal(id, start->quark->life)) 
				break;
			start = start->next;
		}
		return start;
	}
	return NULL;
}

cquark_item_t *_search_item_reverse_cquark(
			cquark_t *cquark, 
			const char *name, 
			cquark_item_t *start)
{
	if (cquark && name) {
		unsigned int len = strlen(name);
		start = start ? start : cquark->tail;
		while (start) {
			if ((strlen(start->quark->name) == len) &&
				!strcmp(name, start->quark->name)) 
				break;
			start = start->prev;
		}
		return start;
	}
	return NULL;
}

cquark_item_t *_search_item_by_id_reverse_cquark(cquark_t *cquark,
					pthread_t id,
					cquark_item_t *start)
{
	if (cquark && id) {
		start = start ? start : cquark->tail;
		while (start) {
			if (pthread_equal(id, start->quark->life)) 
				break;
			start = start->prev;
		}
		return start;
	}
	return NULL;
}

void _destroy_cquark(cquark_t **cquark_ptr)
{
	cquark_item_t *item = NULL;

	if (cquark_ptr && *cquark_ptr) {
		while ((item = _get_head_cquark(*cquark_ptr))) {
			_destroy_cquark_item(&item);
		//	item = NULL;
		}
		free(*cquark_ptr);
		*cquark_ptr = NULL;
	}
}

