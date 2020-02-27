/*
 * handle category
 *
 */

#include <stdint.h>	/* for uint32_t */
#include "parameter.h"


typedef struct _category {
	char *name;
	parameters_t *params;
	uint32_t dup_count;
	struct _category *next;
} category_t;


/* category interfaces */
category_t *_new_category(const char *name);

void _destroy_category(category_t **cpp);

int _add_duplicate_category(category_t *category, category_t *dup);

int _set_parameter_category(category_t *category, const char *key, const char *value);

param_t *_get_parameter_category(category_t *category, const char *key);

parameters_t *_get_category_parameters(category_t *category);

char *_get_param_value_category(category_t *catgory, const char *key);

category_t *_next_category(category_t *category);

category_t *_copy_category(category_t *category);

//char *_category_to_string(category_t *category);
