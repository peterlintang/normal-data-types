
#include "category.h"

typedef struct _category_list {
	category_t *category;
	struct _category_list *prev;
	struct _category_list *next;
} category_list_t;

typedef struct _configuration_t {
	char	*name;
	category_list_t *categories;
	unsigned int	count;
} configuration_t;


/* configuration interfaces */
category_list_t *
_get_category_from_configuration(category_list_t *list, category_list_t *start, char *name);

configuration_t *_new_configuration(const char *name);

int _add_category_to_list(configuration_t *config, category_list_t *new);
void _destroy_category_list(category_list_t *list);


int __add_category_to_configuration(configuration_t *config, category_t *c);

void _destroy_configuration(configuration_t **config);

category_t *_add_category_to_configuration2(configuration_t *config, const char *category_name);

int _del_category_from_configuration(configuration_t *config, const char *category_name);

category_t *_search_category_by_name(configuration_t *config, const char *name);


//category_t *_get_category_from_configuration(configuration_t *config, const char *category_name);
