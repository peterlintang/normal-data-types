/*
 *
 * handle configuration file
 *
 */

#include <string.h>		// for strchr()
#include "str.h"	/* for _trim_string */
#include "inifile.h"

category_t *_add_one_category(configuration_t *config,
				char *string,
				int  *err
				)
{
	if (config && string) {
		category_t 	*category = NULL;
		char 		*end = strchr(string, ']');
		
		if (!end) {
			*err = 1;
			return NULL;
		}

		*end = '\0';
		category = _add_category_to_configuration2(config, 
							_trim_string(string + 1));
		if (!category)
			fprintf(stderr, "%s: error while adding \
					category to config\n", 
					__func__);
		if (*(end + 1) != '\0')
			fprintf(stderr, "%s: ignore trailing garbage\n", 
					__func__);
		return category;
	}
	return NULL;
}

int _add_one_parameter(category_t *category, char *string)
{
	char 		*sep    = strchr(string, '=');

	if (!sep) {
		return 0;
	}

	*sep = '\0';
	if (_set_parameter_category(
			category, 
			_trim_string(string), 
			_trim_string(sep + 1)))
		return 1;
	else 
		return 0;
}

int __parse_file(configuration_t *config, file_t *file)
{
	char 	*line = NULL;
	char 	*ptr  = NULL;
	int32_t line_num = 0;

	category_t *category = _add_category_to_configuration2(config, "global");
	while ((line = _getline_file(file))) {

		char *string = _trim_string(line);
		line_num++;

		ptr = strchr(string, ';');
		if (ptr) *ptr = '\0';
		ptr = strchr(string, '#');
		if (ptr) *ptr = '\0';
		if (strlen(string) == 0) continue;

		if (*string == '[') {
			int err = 0;
			category = _add_one_category(config, string, &err);
			if (!category && err) {
				fprintf(stderr, "%s: missing ']' detected \
						on line #%d\n", 
						__func__, 
						line_num);
				return 0;	/* parse fail */
			}
		} else if (_add_one_parameter(category, string)){
			;
		} else {
			fprintf(stderr, "%s: error adding parameter \
					string %s. continue\n",
					__func__,
					string);
		}
	}
	return 1;
}

configuration_t *_parse_inifile(const char *file_name)
{
	configuration_t *config = _new_configuration(file_name);
	if (config) {
		file_t *file = _new_file(file_name, "ro");
		if (file && __parse_file(config, file)) {
			_destroy_file(&file);
			return config;
		} else {
			_destroy_file(&file);
			_destroy_configuration(&config);
		}
	}
	return NULL;
}
































/**********************************************************************************
 *
 * never mind *
 *
 ***********************************************************************************/
/*
                        char *end = strchr(string, ']');
                        if (!end) {
                                fprintf(stderr, "%s: missing \
                                        ']' detected on line #%d\n",
                                        __func__,
                                        line_num);
                                return 0;
                        }
                        *end = '\0';
                        category = _add_category_to_configuration2(config,
                                                        _trim_string(string+1));
                        if (!category)
                                fprintf(stderr, "%s: error while \
                                                adding category to config\n",
                                                __func__);
                        if (*(end+1) != '\0')
                                fprintf(stderr, "%s: ignore trailing garbage\n",
                                                __func__);

 */

/*
                        char *sep = strchr(string, '=');
                        if (!sep) {
                                fprintf(stderr, "%s: invalid line '%s' \
                                        detected on line #%d.\n",
                                        __func__, line, line_num);
                                return 0;
                        }
                        *sep = '\0';
                        _set_parameter_category(category,               // what should we do if it cannot set 
                                        _trim_string(string),
                                        _trim_string(sep+1))


 */
