/*
 * handle configuration
 * 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"

category_list_t *
_get_category_from_configuration(category_list_t *list,
				 category_list_t *start,
 				 char *category_name)
{
	if (list && category_name) {
		category_t *c;
		int len = strlen(category_name);

		if (!start) start = list;

		while (start) {
			c = start->category;
			if ((strlen(c->name) == len) && !strcmp(category_name, c->name))
				return start;
			start = start->next;
		}
	}
	return NULL;
}

configuration_t *_new_configuration(const char *name)
{
	if (name) {
		configuration_t *new = \
		   (configuration_t *)calloc(1, sizeof(configuration_t));
		if (new) {
			new->name = strdup(name);
			return new;
		}
		return NULL;
	}
	return NULL;
}

/*
 * the caller must be sure that @config , @new must 
 * be valuable.
 * 
 */
int _add_category_to_list(configuration_t *config, category_list_t *new)
{
	if (config->categories) {
		category_list_t *last = config->categories;
		while (last->next) last = last->next;
		last->next = new;
		new->prev = last;
	} else {
		config->categories = new;
	}
	config->count++;
	return 1;
}

int __add_category_to_configuration(configuration_t *config, category_t *c)
{
//	if (config && c && !_get_category_from_configuration(config, NULL, c->name)) {
	if (config && c) {
		category_list_t *new = (category_list_t *)calloc(1, sizeof(category_list_t));
		if (new) {
			new->category = c;
			return _add_category_to_list(config, new);
		}

		return 0;
	} 
	/*else {
		DPRINTF("category %s exist\n", c->name);
	}
	*/
	return 0;
}

category_t * _add_category_to_configuration2(configuration_t *config, const char *category_name)
{
	if (config && category_name) {
		category_t *c = _new_category(category_name);
		if (c && 
		    __add_category_to_configuration(config, c)) {
			return c;
		}
		return NULL;
	}
	return NULL;
}

/* 
 *  to deep, not good, we try to keep 
 *  the code simple and clear. so try hard
 *  not to embebed more than 3 deeper

int _add_category_to_configuration(configuration_t *config, category_t *c)
{
	if (config && c) {
		category_list_t *new = (category_list_t *)calloc(1, sizeof(category_list_t));
		if (new) {
			new->category = c;
			if (config->categories) {
				category_list_t *last = config->categories;
				while (last->next) last = last->next;
				if (last == config->categories) {
					config->categories = new;
				}
				else {
					last->next = new;
					new->prev = last;
				}
			} else {
				config->categories = new;
			}
			config->count++;
			return 1;
		}
		return 0;
	}
	return 0;
}
*/

void _destroy_category_list(category_list_t *list)
{
	if (list) {
		if (list->next) _destroy_category_list(list->next);
		_destroy_category(&list->category);
		free(list);
	}
}

void _destroy_configuration(configuration_t **config)
{
	if (config && *config) {
		_destroy_category_list((*config)->categories);
		free((*config)->name);
		free(*config);
		*config = NULL;
	}
}


category_t *_search_category_by_name(configuration_t *config, const char *name)
{
	category_list_t *item	= NULL;

	if (!config || !name) {
		fprintf(stderr, "arguments error\n");
		return NULL;
	}

	item = config->categories;
	while (item) {
		if (item->category && 
			item->category->name && 
			!strcmp(item->category->name, name)) 
			return item->category;
		item = item->next;
	}

	return NULL;
}

















