
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include "str.h"

#include "dlist.h"

// <macros 251>
#define IDX(i, len)	((i) <= 0 ? (i) + (len) : (i) - 1)
#define CONVERT(s, i, j)  do { \
		int len; \
		assert(s); \
		len = strlen(s); 	\
		i = IDX(i, len); \
		j = IDX(j, len);	\
		if (i > j) 	\
		{ \
			int t = i; i = j; j = t; } \
			assert(i >= 0 && j <= len);  \
		} while (0)

// <functions 252>
char *MODULE_FUN_NAME(Str, sub)(const char *s, int i, int j)
{
	char *str = NULL;
	char *p = NULL;

	CONVERT(s, i, j);
	p = str = (char *)calloc(1, (j - i + 1) * sizeof(char));
	while (i < j)
	{
		*p++ = s[i++];
	}
	*p = '\0';

	return str;
}

char *MODULE_FUN_NAME(Str, dup)(const char *s, int i, int j, int n)
{
	int k = 0;
	char *str = NULL;
	char *p = NULL;

	assert(n >= 0);
	CONVERT(s, i, j);
	p = str = (char *)calloc(1, (n * (j - i) + 1) * sizeof(char));
	if (j - i > 0)
	{
		while (n-- > 0)
		{
			for (k = i; k < j; k++)
				*p++ = s[k];
		}
	}

	*p = '\0';

	return str;
}

char *MODULE_FUN_NAME(Str, reverse)(const char *s, int i, int j)
{
	char *str = NULL;
	char *p = NULL;

	CONVERT(s, i, j);
	p = str = (char *)calloc(1, (j - i + 1) * sizeof(char));
	while (j > i)
	{
		*p++ = s[--j];
	}
	*p = '\0';

	return str;
}

char *MODULE_FUN_NAME(Str, cat)(const char *s1, int i1, int j1,
				const char *s2, int i2, int j2)
{
	char *str = NULL;
	char *p = NULL;

	CONVERT(s1, i1, j1);
	CONVERT(s2, i2, j2);
	p = str = (char *)calloc(1, (j1 - i1 + j2 - i2 + 1) * sizeof(char));
	while (i1 < j1)
		*p++ = s1[i1++];
	while (i2 < j2)
		*p++ = s2[i2++];
	*p = '\0';

	return str;
}

char *MODULE_FUN_NAME(Str, catv)(const char *s, ...)
{
	char *str = NULL;
	char *p = NULL;
	const char *save = s;
	int i;
	int j;
	int len = 0;
	va_list ap;

	va_start(ap, s);
	while (s)
	{
		i = va_arg(ap, int);
		j = va_arg(ap, int);
		CONVERT(s, i, j);
		len += j - i;
		s = va_arg(ap, const char *);
	}
	va_end(ap);

	p = str = (char *)calloc(1, (len + 1) * sizeof(char));
	s = save;
	va_start(ap, s);
	while (s)
	{
		i = va_arg(ap, int);
		j = va_arg(ap, int);
		CONVERT(s, i, j);
		while (i < j)
		{
			*p++ = s[i++];
		}
		s = va_arg(ap, const char *);
	}
	va_end(ap);
	*p = '\0';

	return str;
}

char *MODULE_FUN_NAME(Str, map)(const char *s, int i, int j,
				const char *from, const char *to)
{
	static char map[256] = { 0 };

	if (from && to)
	{
		unsigned c;
		for (c = 0; c < sizeof(map); c++)
		{
			map[c] = c;
		}
		while (*from && *to)
		{
			map[(unsigned char)*from++] = *to++;
		}
		assert(*from == 0 && *to == 0);
	}
	else
	{
		assert(from == NULL && to == NULL && s);
		assert(map['a']);
	}

	if (s)
	{
		char *str = NULL;
		char *p = NULL;

		CONVERT(s, i, j);
		p = str = (char *)calloc(1, (j - i + 1) * sizeof(char));
		while (i < j)
		{
			*p++ = map[(unsigned char)s[i++]];
		}
		*p = '\0';
		return str;
	}
	else
	{
		return NULL;
	}
}

int MODULE_FUN_NAME(Str, pos)(const char *s, int i)
{
	int len = 0;

	assert(s);

	len = strlen(s);
	i = IDX(i, len);

	assert(i >= 0 && i <= len);

	return i + 1;
}

int MODULE_FUN_NAME(Str, len)(const char *s, int i, int j)
{
	CONVERT(s, i, j);

	return j - i;
}

int MODULE_FUN_NAME(Str, cmp)(const char *s1, int i1, int j1,
				const char *s2, int i2, int j2)
{
	int cond = 0;

	CONVERT(s1, i1, j1);
	CONVERT(s2, i2, j2);

	s1 += i1;
	s2 += i2;

	if (j1 - i1 < j2 - i2)
	{
		cond = strncmp(s1, s2, j1 - i1);
		return cond == 0 ? -1 : cond;
	}
	else if (j1 - i1 > j2 - i2)
	{
		cond = strncmp(s1, s2, j2 - i2);
		return cond == 0 ? 1 : cond;
	}
	else
	{
		return strncmp(s1, s2, j1 - i1);
	}
}

int MODULE_FUN_NAME(Str, chr)(const char *s, int i, int j, int c)
{
	CONVERT(s, i, j);
	for (; i < j; i++)
	{
		if (s[i] == c)
			return i + 1;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, rchr)(const char *s, int i, int j, int c)
{
	CONVERT(s, i, j);
	while (j > i)
	{
		if (s[--j] == c)
			return j + 1;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, upto)(const char *s, int i, int j, 
				const char *set)
{
	assert(set);

	CONVERT(s, i, j);
	for (; i < j; i++)
	{
		if (strchr(set, s[i]))
			return i + 1;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, rupto)(const char *s, int i, int j,
				const char *set)
{
	assert(set);
	CONVERT(s, i, j);
	while (j > i)
	{
		if (strchr(set, s[--j]))
			return j + 1;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, find)(const char *s, int i, int j,
				const char *str)
{
	int len = 0;

	CONVERT(s, i, j);

	assert(str);

	len = strlen(str);
	if (0 == len)
	{
		return i + 1;
	}
	else if (1 == len)
	{
		for (; i < j; i++)
		{
			if (s[i] == *str)
				return i + 1;
		}
	}
	else
	{
		for (; i + len <= j; i++)
		{
			if (strncmp(&s[i], str, len) == 0)
				return i + 1;
		}
	}

	return 0;
}

int MODULE_FUN_NAME(Str, rfind)(const char *s, int i, int j,
				const char *str)
{
	int len = 0;

	CONVERT(s, i, j);

	assert(str);

	len = strlen(str);

	if (0 == len)
	{
		return j + 1;
	}
	else if (1 == len)
	{
		while (j > i)
		{
			if (s[--j] == *str)
				return j + 1;
		}
	}
	else
	{
		for (; j - len >= i; j--)
		{
			if (strncmp(&s[j - len], str, len) == 0)
				return j - len + 1;
		}
	}

	return 0;
}

int MODULE_FUN_NAME(Str, any)(const char *s, int i, const char *set)
{
	int len = 0;

	assert(s);
	assert(set);

	len = strlen(s);
	i = IDX(i, len);

	assert(i >= 0 && i <= len);
	if (i < len && strchr(set, s[i]))
		return i + 2;

	return 0;
}

int MODULE_FUN_NAME(Str, many)(const char *s, int i, int j, const char *set)
{
	assert(set);

	CONVERT(s, i, j);
	if (i < j && strchr(set, s[i]))
	{
		do {
			i++;
		} while (i < j && strchr(set, s[i]));

		return i + 1;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, rmany)(const char *s, int i, int j, const char *set)
{
	assert(set);

	CONVERT(s, i, j);
	if (j > i && strchr(set, s[j - 1]))
	{
		do {
			--j;
		} while (j >= i && strchr(set, s[j]));
		return j + 2;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, match)(const char *s, int i, int j,
				const char *str)
{
	int len = 0;

	CONVERT(s, i, j);

	assert(str);

	len = strlen(str);
	if (0 == len)
	{
		return i + 1;
	}
	else if (1 == len)
	{
		if (i < j && s[i] == *str)
			return i + 2;
	}
	else if ( i + len <= j && strncmp(&s[i], str, len) == 0)
	{
		return i + len + 1;
	}

	return 0;
}

int MODULE_FUN_NAME(Str, rmatch)(const char *s, int i, int j, 
				const char *str)
{
	int len = 0;

	CONVERT(s, i, j);
	
	assert(str);

	len = strlen(str);
	if (0 == len)
	{
		return j + 1;
	}
	else if (1 == len)
	{
		if (j > 1 && s[j - 1] == *str)
			return j;
	}
	else if (j - len >= i && strncmp(&s[j - len], str, len) == 0)
	{
		return j - len + 1;
	}

	return 0;
}

ListD_T MODULE_FUN_NAME(Str, tokenize)(const char *buf, const char *delim)
{
	char *p = NULL;
	char *oldp = NULL;
	char *copy = NULL;
	ListD_T list = NULL;

	if (buf == NULL || delim == NULL)
		return list;

	list = MODULE_FUN_NAME(ListD, new)();
	if (list == NULL)
		return list;

	copy = strdup(buf);
	p = copy;

	while ((oldp = strsep(&p, delim)) != NULL)
	{
		MODULE_FUN_NAME(ListD, insert)( list, MODULE_FUN_NAME(ListDNode, new)(strdup(oldp)) );
	}
	free(copy);

	return list;
}

void MODULE_FUN_NAME(Str, fmt)(int code, va_list *app,
			int put(int c, void *cl), void *cl,
			unsigned char flags[], int width, int precision)
{
	char *s = NULL;
	int i = 0;
	int j = 0;

	assert(app && flags);
	s = va_arg(*app, char *);
	i = va_arg(*app, int);
	j = va_arg(*app, int);

	CONVERT(s, i, j);
//	Fmt_puts(s + i, j - i, put, cl, flags, width, precision);
}


