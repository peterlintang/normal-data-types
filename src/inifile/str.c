/*
 *
 * handle with string
 *
 */

#include <sys/time.h>

#include "str.h"

char *_new_string(const char *format, ...)
{
	va_list ap;
	char buf[STRING_MAX_LEN];
	char *copy = NULL;

	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);
	
	copy = _copy_string(buf);
	return copy;
}

char *_new_random_string(unsigned int len)
{
	static char buf[STRING_MAX_LEN + 1];
	const char *chars = "abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"0123456789";
	unsigned int max = strlen(chars);
	unsigned int i = 0;
	struct timeval tv;
	char *copy = NULL;

//	pthread_mutex_lock();

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec);
	
	if (len > STRING_MAX_LEN)
		len = STRING_MAX_LEN;

	/* NOTE: rand() is not reentrant or thread-safe, so
	 * we need think of another way to generate the randdom
	 * string
	 */
	for (; i < len; ++i)
		buf[i] = chars[rand() % max]; 
	buf[i] = '\0';

	copy = _copy_string(buf);
//	pthread_mutex_unlock();

	return copy;
}

char *_copy_string(const char *str)
{
	if (str)
		return strdup(str);
	return NULL;
}

int _is_white(char c)
{
	switch (c) {
		case ' ':
		case '\t':
		case '\n':
			return 1;
	}
	return 0;
}

char *_skip_white(const char *str)
{
	while ((*str != '\0') && (_is_white(*str))) {
		str++;
	}
	return (char *) str;
}

char *_rskip_white(const char *str)
{
	char *end_str = (char *)str + strlen(str);
	/* str[0] how */
	while ((--end_str > str) && (_is_white(*end_str)))
		*end_str = '\0';

	return (char *) str;
}

char *_trim_string(const char *str)
{
	str = _skip_white(str);
	str = _rskip_white(str);
	return (char *) str;
}

ssize_t _join_string(char **to, const char *with)
{
	size_t orig_len = 0;
	size_t with_len = strlen(with);
	if (!with_len) return 0;
	if (*to) orig_len = strlen(*to);
	*to = realloc(*to, sizeof(char) * (orig_len + with_len + 2));
	if (!*to) return 0;
	memcpy(&(*to)[orig_len], with, sizeof(char) * (with_len + 1));
	return (orig_len + with_len + 1);
}


int _equal_string(const char *str1, const char *str2)
{
	if (str1 && str2) {
		int len1 = strlen(str1);
		int len2 = strlen(str2);
		
		if ((len1 == len2) && !strcmp(str1, str2))
			return 1;  /* one : true */
	}
	return 0; /* zero : false */
}

/* unreentriable */
char *itoaze2_string(int64_t value)
{
	register char *p;
	register int32_t minus;
	static char buf[36];
	char *yo;

//should lock here
	fprintf(stdout, "in itoaze2...\n");
	p = &buf[36];
//	fprintf(stdout, "p : %p\n", p);
	*--p = '\0';
//	fprintf(stdout, "char : %c\n", buf[35]);
//	fprintf(stdout, "p : %p\n", p);

	if (value < 0) {
		minus = 1;
		value = -value;
	} else {
		minus = 0;
	}

	if (value == 0)
		*--p = '0';
	else
		while (value > 0) {
			*--p = "0123456789"[value % 10];
			value /= 10;
		}

	if (minus)
		*--p = '-';

	yo = strdup(p);

//	fprintf(stdout, "leave now: %s address %p\n", yo, yo);

/* should unlock here */
	return yo;
}

char *utoa2_string(uint64_t value)
{
	register char *p;
	/* why must be static */
//	static char buf[36];
	char buf[36];
	char *yo = NULL;

	p = &buf[36];
	*--p = '\0';

	if (value == 0)
		*--p = '0';
	else
		while (value > 0) {
			*--p = "0123456789"[value % 10];
			value /= 10;
		}
	yo = strdup(p); /* caller should free the memeroy */

	return yo;
}

char *_first_in_string(const char *string, const char *match)
{
	if (string && match) {
		return strstr(string, match);
	}
	return NULL;
}

char *_last_in_string(const char *string, const char *match)
{
	if (string && match) {
		char *ptr = (char *)string;
		char *last = NULL;
		while ((ptr = strstr(ptr, match))) {
			last = ptr;
			ptr += 1;
		}
		return last;
	}
	return NULL;
}





