#ifndef PARAMETERS_H
#define PARAMETERS_H


typedef struct _param {
	char *key;
	char *value;
	struct _param *next;
} param_t;

typedef struct _parameters {
	param_t *first;
	param_t *last;
	unsigned int count;
} parameters_t;

/* param interfaces */
void _destroy_param(param_t *param);

int _compare_param_key(param_t *one, param_t *two);

int _compare_param_val(param_t *one, param_t *two);

/* parameters interfaces */
parameters_t *_new_parameters(void);

param_t *_get_param_by_key(parameters_t *params, const char *key);

param_t *_get_param_by_val(parameters_t *params, const char *val);

char *_get_param_val_by_key(parameters_t *params, const char *key);

char *_get_param_val(param_t *param);

char *_get_param_key_by_val(parameters_t *params, const char *val);

char *_get_param_key(param_t *param);

param_t *_add_param(parameters_t *params, const char *val, const char *key);

param_t *_add_param_after(parameters_t *params, param_t *new, param_t *after);


param_t *_add_param_before(parameters_t *params, param_t *new, param_t *before);

parameters_t *_copy_parameters(parameters_t *params);

param_t *_update_param(parameters_t *params, const char *key, const char *val);

void _destroy_parameters(parameters_t **);

#endif
