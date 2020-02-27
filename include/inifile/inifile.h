/*
 * handle configuration
 *
 *
 */
#include "configuration.h"
#include "file.h"

category_t *_add_one_category(configuration_t *config, char *string, int *err);

int _add_one_parameter(category_t *category, char *string);

int __parse_file(configuration_t *config, file_t *file);

configuration_t *_parse_inifile(const char *inifile);


