
#ifndef _string_H
#define _string_H

#define STRING_MAX_LEN	1024

//pthread_mutex_t 	random_string_lock;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h> /* for int64_t type  */


int _is_white(char c);

/* string interfaces */
char *_new_string(const char *format, ...);
char *_new_random_string(unsigned int len);
char *_copy_string(const char *str);
char *_skip_white(const char *str);
char *_rskip_white(const char *str);
char *_trim_string(const char *str);
ssize_t _join_string(char **to, const char *with);
int _equal_string(const char *str1, const char *str2);
char *itoaze2_string(int64_t value);
char *utoa2_string(uint64_t value);
char *_first_in_string(const char *string, const char *match);
char *_last_in_string(const char *string, const char *match);
char *_translate_string(const char *string, const char match, const char with);

#endif
