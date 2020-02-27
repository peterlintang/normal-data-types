/*
 * parameters lib
 * handle parameters
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parameter.h"

void _destroy_param(param_t *param)
{
	if (!param) return;
	if (param->next) 
		_destroy_param(param->next);
	free(param->key); 
	free(param->value);
	free(param);
}

int _compare_param_key(param_t *one, param_t *two)
{
	return strcmp(one->key, two->key);
}

int _compare_param_val(param_t *one, param_t *two)
{
	return strcmp(one->value, two->value);
}

parameters_t *_new_parameters(void)
{
	parameters_t *params;
	
	params = (parameters_t *)calloc(1, sizeof(parameters_t));
	return params;
}

param_t *_get_param_by_key(parameters_t *params, const char *key)
{
	if (params) {
		unsigned int len = strlen(key);
		param_t *p = params->first;
		while (p) {
			if ((strlen(p->key) == len) && !strcmp(key, p->key))
				return p;
			p = p->next;
		}
	}
	return NULL;
}

param_t *_get_param_by_val(parameters_t *params, const char *val)
{
	if (params) {
		unsigned int len = strlen(val);
		param_t *p = params->first;
		while (p) {
			if ((strlen(p->value) == len) && !strcmp(val, p->value))
				return p;
			p = p->next;
		}
	}
	return NULL;
}

char *_get_param_val_by_key(parameters_t *params, const char *key)
{
	param_t *p = _get_param_by_key(params, key);
	return p ? p->value : NULL;
}

char *_get_param_val(param_t *param)
{
	return param ? param->value : NULL;
}

char *_get_param_key_by_val(parameters_t *params, const char *val)
{
	param_t *p = _get_param_by_val(params, val);
	return p ? p->key : NULL;
}

char *_get_param_key(param_t *param)
{
	return param ? param->key : NULL;
}

param_t *_add_param(parameters_t *params, const char *val, const char *key)
{
	if (params && !_get_param_by_key(params, key)) {
		param_t *p = calloc(1, sizeof(param_t));
		if (p) {
			p->key = strdup(key);
			p->value = strdup(val);

			if (params->first) {
				params->last->next = p;
				params->last = p;
			} else {
				params->first = params->last = p;
			}
			params->count++;
			return p;
		}
	}
	return NULL;
}

param_t *_add_param_after(parameters_t *params, param_t *new, param_t *after)
{
	if (new) {
		after = after ? after : params->last;
		if (after) {
			new->next = after->next;
			after->next = new;
		} else {
			params->first = params->last = new;
		}
		params->count++;
	}
	return new;
}


param_t *_add_param_before(parameters_t *params,
			param_t *new,
			param_t *before)
{
	if (new) {
		before = before ? before : params->first;
		if (before) {
			int found;
			param_t *p = params->first;
			while (p) {
				if (p->next == before) {
					found = 1;
					break;
				}
				p = p->next;
			}
			if (found) {
				new->next = before;
				p->next = new;
			} else {
				new->next = params->first;
				params->first = new;
			}
		} else {
			params->first = params->last = new;
		}
		params->count++;
	}
	return new;
}

parameters_t *_copy_parameters(parameters_t *params)
{
	parameters_t *copy = _new_parameters();
	if (copy && params) {
		param_t *p = params->first;
		while (p) {
			_add_param(copy, p->value, p->key);
			p = p->next;
		}
	}
}

param_t *_update_param(parameters_t *params, const char *key, const char *val)
{
	if (params) {
		param_t *p = _get_param_by_key(params, key);
		if (p) {
			if (p->value) free(p->value);
			p->value = strdup(val);
			return p;
		} else {
			return _add_param(params, val, key);
		}
	}
	return NULL;
}

void _destroy_parameters(parameters_t **paramsPtr)
{
	if (paramsPtr && *paramsPtr) {
		_destroy_param((*paramsPtr)->first);
		free(*paramsPtr);
		paramsPtr = NULL;
	}
}
