
/*
 * parse execute progrma's arguments
 * and store them in parameters_t struct
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_params.h"

#define MAX_PARAM_LEN 64
/*
 * you should make sure that the number elements in argv 
 * is equal to atgc
 *
 **/
parameters_t *parse_parameters(int argc, char *argv[])
{
	int i;
	parameters_t *params = NULL;

	if (argc < 1 || !argv || !(params = _new_parameters()))
		return NULL;

	for (i = 0; i < argc; i++) {
		if (argv[i]) {
			char *tmp  = strchr(argv[i], '=');
			if (tmp) {
				char key[MAX_PARAM_LEN];
				int len = (int )(tmp - argv[i]);
				strncpy(key, argv[i], len);
				key[len] = '\0';
				if (!_add_param(params, tmp + 1, key)) {
					fprintf(stderr, "parse params error 1\n");
					exit(1);
				}
			} else if (!_add_param(params, "true", argv[i])){ 
				fprintf(stderr, "parse params error 2\n");
				exit(1);
			} else {
				;
			}
		}
	}
	return params;
}

void clean_parse_parameters(parameters_t *params)
{
	if (params) {
		_destroy_parameters(&params);
	}
}

/*
 * just test parse_params can work or 
 * not,  
 */
/*
int main(int argc, char *argv[])
{
	parameters_t *params = parse_params(argc, argv);
	fprintf(stdout, "parse Ok\n");

	fprintf(stdout, "------------show parameters-------------\n");
	param_t *param = params->first;
	while (param) {
		fprintf(stdout, "key: %s value: %s\n", param->key, param->value);
		param = param->next;
	}
	

	fprintf(stdout, "destroy params... ");
	_destroy_parameters(&params);
	fprintf(stdout, "OK\n");
	
	return 0;
}
*/
