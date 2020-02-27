/*
 * handle category
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "category.h"

/*
 * create a new category,
 */
category_t *_new_category(const char *name)
{
	if (name) {
		category_t *c = (category_t *)calloc(1, sizeof(category_t));
		if (c) {
			c->name = strdup(name);
			c->params = _new_parameters();
			if (!c->name || !c->params) {
				free(c->name);
				_destroy_parameters(&c->params);;
				free(c);
				c = NULL;
				/* or only bellow one line */
				/** _destroy_category(&c); **/
			}
		}
		return c;
	}
	return NULL;
}

/*
 * destroy a category @@cpp
 */
void _destroy_category(category_t **cpp)
{
	if (cpp && *cpp) {
		if ((*cpp)->next) _destroy_category(&((*cpp)->next));
		_destroy_parameters(&((*cpp)->params));
		free((*cpp)->name);
		free(*cpp);
		*cpp = NULL;
	}
}

/*
 * add a duplicated category to the end of the category list
 * return 1, on success; 0 on failure
 *
 */
int _add_duplicate_category(category_t *c, category_t *dup)
{
	if (c && dup) {
		category_t *last = c;
		while (last->next) last = last->next;
		last->next = dup;
		c->dup_count++;
		return 1;
	}
	return 0;
}

/*
 * set the parameter @key's value to be @value.
 * if the paramter doesn't exist, add it to 
 * the @c->params.
 */
int _set_parameter_category(category_t *c, const char *key, const char *value)
{
	if (_update_param(c->params, key, value))
		return 1;
	return 0;
}

/*
 * get the parameter indicated by @key from category @c.
 */
param_t *_get_parameter_category(category_t *c, const char *key)
{
	if (c) {
		if (c->params && key)
			return _get_param_by_key(c->params, key);
		/* or bellow */
/*	if (c && key)
		return _get_param_by_key(c->params, key)
	return NULL;
 */
	}
	return NULL;
}

parameters_t *_get_category_parameters(category_t *c)
{
	return c ? c->params : NULL;
}

/*
 * get a parameter (which indicated by @key) 's value
 * from category @c.
 *
 */
char *_get_param_value_category(category_t *c, const char *key)
{
	if (c && key)
		return _get_param_val_by_key(c->params, key);
	return NULL;
}

category_t *_next_category(category_t *c)
{
	return c ? c->next : NULL;
}

/*
 * this will support in future 
 * now for easy code, we don't use
 * it. but later we will support this
 */
category_t *_copy_category(category_t *c)
{
	if (c) {
		category_t *copy, *new;
		copy = new = (category_t *)calloc(1, sizeof(category_t));
		while (c && new) {
			new->name = strdup(c->name);
			new->params = _copy_parameters(c->params);
			if (c->next) {
				new->next = (category_t *)calloc(1, sizeof(category_t));
				new = new->next;
			}
		}
		return copy;
	}
	return NULL;
}

/* for future
char *_category_to_string(category_t *c)
{

}
*/

