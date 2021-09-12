
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ap.h"
#include "xp.h"


#define T AP_T
struct T {
	int sign;
	int ndigits;
	int size;
	XP_T digits;
};

// < macros 337 >
#define ISZERO(x) ((x)->ndigits == 1 && (x)->digits[0] == 0)
#define MAXDIGITS(x, y) ((x)->ndigits > (y)->ndigits ? \
		(x)->ndigits : (y)->ndigits)
#define ISONE(x)	((x)->ndigits == 1 && (x)->digits[0] == 1)

// < prototypes 336 >
static T normalize(T z, int n);
static int cmp(T x, T y);

// < static functions 335 >
static T mk(int size)
{
	T z = NULL;

	assert(size > 0);

	z = (T)calloc(1, sizeof(*z) + size);
	if (z)
	{
		z->sign = 1;
		z->size = size;
		z->ndigits = 1;
		z->digits = (XP_T)(z + 1);
	}

	return z;
}

static T set(T z, long int n)
{
	if (LONG_MIN == n)
	{
		MODULE_FUN_NAME(XP, fromint)(z->size, z->digits, LONG_MAX + 1UL);
	}
	else if (n < 0)
	{
		MODULE_FUN_NAME(XP, fromint)(z->size, z->digits, -n);
	}
	else
	{
		MODULE_FUN_NAME(XP, fromint)(z->size, z->digits, n);
	}

	z->sign = n < 0 ? -1 : 1;
	return normalize(z, z->size);
}

static T normalize(T z, int n)
{
	z->ndigits = MODULE_FUN_NAME(XP, length)(n, z->digits);

	return z;
}

static T add(T z, T x, T y)
{
	int n = y->ndigits;

	if (x->ndigits < n)
	{
		return add(z, y, x);
	}
	else if (x->ndigits > n)
	{
		int carry = MODULE_FUN_NAME(XP, add)(n, z->digits, x->digits, y->digits, 0);
		z->digits[z->size - 1] = MODULE_FUN_NAME(XP, sum)
				(x->ndigits - n, 
						&z->digits[n], &x->digits[n], carry);
	}
	else
	{
		z->digits[n] = MODULE_FUN_NAME(XP, add)
				(n, z->digits, x->digits, y->digits, 0);
	}

	return normalize(z, z->size);
}

static T sub(T z, T x, T y)
{
	int borrow = 0;
	int n = y->ndigits;

	borrow = MODULE_FUN_NAME(XP, sub)
			(n, z->digits, x->digits, y->digits, 0);
	if (x->ndigits > n)
	{
		borrow = MODULE_FUN_NAME(XP, diff)
				(x->ndigits - n, &z->digits[n],
						&x->digits[n], borrow);
	}

	assert(borrow == 0);

	return normalize(z, z->size);
}

static int cmp(T x, T y)
{
	if (x->ndigits != y->ndigits)
	{
		return x->ndigits - y->ndigits;
	}
	else
	{
		return MODULE_FUN_NAME(XP, cmp)
				(x->ndigits, x->digits, y->digits);
	}
}

static T mulmod(T x, T y, T p)
{
	T z;
	T xy;

	xy = MODULE_FUN_NAME(AP, mul)(x, y);

	z = MODULE_FUN_NAME(AP, mod)(xy, p);
	MODULE_FUN_NAME(AP, free)(&xy);

	return z;
}

// < functions 335 >
T MODULE_FUN_NAME(AP, new)(long int n)
{
	return set(mk(sizeof(long int)), n);
}

void MODULE_FUN_NAME(AP, free)(T *z)
{
	assert(z && *z);
	free(*z);
	*z = NULL;
}

T MODULE_FUN_NAME(AP, neg)(T x)
{
	T z;

	assert(x);

	z = mk(x->ndigits);
	memcpy(z->digits, x->digits, x->ndigits);
	z->ndigits = x->ndigits;
	z->sign = ISZERO(z) ? 1 : -x->sign;

	return z;
}

T MODULE_FUN_NAME(AP, mul)(T x, T y)
{
	T z;

	assert(x);
	assert(y);

	z = mk(x->ndigits + y->ndigits);
	MODULE_FUN_NAME(XP, mul)
			(z->digits, x->ndigits, x->digits, y->ndigits, y->digits);
	normalize(z, z->size);
	z->sign = ISZERO(z)
			|| ((x->sign ^ y->sign) == 0) ? 1 : -1;

	return z;
}

T MODULE_FUN_NAME(AP, add)(T x, T y)
{
	T z;

	assert(x);
	assert(y);

	if ((x->sign ^ y->sign) == 0)
	{
		z = add(mk(MAXDIGITS(x, y) + 1), x, y);
		z->sign = ISZERO(z) ? 1 : x->sign;
	}
	else
	{
		if (cmp(x, y) > 0)
		{
			z = sub(mk(x->ndigits), x, y);
			z->sign = ISZERO(z) ? 1 : x->sign;
		}
		else
		{
			z = sub(mk(y->ndigits), y, x);
			z->sign = ISZERO(z) ? 1 : -x->sign;
		}
	}

	return z;
}

T MODULE_FUN_NAME(AP, sub)(T x, T y)
{
	T z;

	assert(x);
	assert(y);

	if (!((x->sign ^ y->sign) == 0))
	{
		z = add(mk(MAXDIGITS(x, y) + 1), x, y);
		z->sign = ISZERO(z) ? 1 : x->sign;
	}
	else
	{
		if (cmp(x, y) > 0)
		{
			z = sub(mk(x->ndigits), x, y);
			z->sign = ISZERO(z) ? 1 : x->sign;
		}
		else
		{
			z = sub(mk(y->ndigits), y, x);
			z->sign = ISZERO(z) ? 1 : -x->sign;
		}
	}

	return z;
}

T MODULE_FUN_NAME(AP, div)(T x, T y)
{
	T q;
	T r;

	assert(x);
	assert(y);
	assert(!ISZERO(y));

	q = mk(x->ndigits);
	r = mk(y->ndigits);
	{
		XP_T tmp = (XP_T)calloc(1, (x->ndigits + y->ndigits + 2) * sizeof(char));
		MODULE_FUN_NAME(XP, div)
				(x->ndigits, q->digits, x->digits, 
						y->ndigits, y->digits, r->digits, tmp);
		free(tmp);
	}
	normalize(q, q->size);
	normalize(r, r->size);
	q->sign = ISZERO(q)
			|| ((x->sign ^ y->sign == 0) ? 1 : -1);

	if (!((x->sign ^ y->sign) == 0) && !ISZERO(r))
	{
		int carry = MODULE_FUN_NAME(XP, sum)
				(q->size, q->digits, q->digits, 1);

		assert(carry == 0);
		normalize(q, q->size);
	}

	MODULE_FUN_NAME(AP, free)(&r);
	return q;
}

T MODULE_FUN_NAME(AP, mod)(T x, T y)
{
	T q;
	T r;

	assert(x);
	assert(y);
	assert(!ISZERO(y));

	q = mk(x->ndigits);
	r = mk(y->ndigits);
	{
		XP_T tmp = (XP_T)calloc(1, (x->ndigits + y->ndigits + 2) * sizeof(char));
		MODULE_FUN_NAME(XP, div)
				(x->ndigits, q->digits, x->digits, 
						y->ndigits, y->digits, r->digits, tmp);
		free(tmp);
	}
	normalize(q, q->size);
	normalize(r, r->size);
	q->sign = ISZERO(q)
			|| ((x->sign ^ y->sign == 0) ? 1 : -1);

	if (!((x->sign ^ y->sign) == 0) && !ISZERO(r))
	{
		int borrow = MODULE_FUN_NAME(XP, sub)
				(r->size, r->digits, 
						y->digits, r->digits, 0);

		assert(borrow == 0);
		normalize(r, r->size);
	}

	MODULE_FUN_NAME(AP, free)(&q);
	return r;
}

T MODULE_FUN_NAME(AP, pow)(T x, T y, T p)
{
	T z;

	assert(x);
	assert(y);
	assert(y->sign == 1);
	assert(!p || p->sign == 1 && !ISZERO(p) && !ISONE(p));

	if (ISZERO(x))
	{
		return MODULE_FUN_NAME(AP, new)(0);
	}
	if (ISZERO(y))
	{
		return MODULE_FUN_NAME(AP, new)(1);
	}
	if (ISONE(x))
	{
		return MODULE_FUN_NAME(AP, new)(((y)->digits[0] & 1) == 0 ? 1 : x->sign);
	}

	if (p)
	{
		if (ISONE(y))
		{
			z = MODULE_FUN_NAME(AP, mod)(x, p);
		}
		else
		{
			T y2;
			T t;

			y2 = MODULE_FUN_NAME(AP, rshift)(y, 1);
			t = MODULE_FUN_NAME(AP, pow)(x, y2, p);
			z = mulmod(t, t, p);
			MODULE_FUN_NAME(AP, free)(&y2);
			MODULE_FUN_NAME(AP, free)(&t);
			if (!((y->digits[0] & 1) == 0))
			{
				z = mulmod(y2 = MODULE_FUN_NAME(AP, mod)(x, p), t = z, p);
				MODULE_FUN_NAME(AP, free)(&y2);
				MODULE_FUN_NAME(AP, free)(&t);
			}
		}
	}
	else
	{
		if (ISONE(y))
		{
			z = MODULE_FUN_NAME(AP, addi)(x, 0);
		}
		else
		{
			T y2;
			T t;

			y2 = MODULE_FUN_NAME(AP, rshift)(y, 1);
			t = MODULE_FUN_NAME(AP, pow)(x, y2, NULL);
			z = MODULE_FUN_NAME(AP, mul)(t, t);

			MODULE_FUN_NAME(AP, free)(&y2);
			MODULE_FUN_NAME(AP, free)(&t);

			if (!((y->digits[0] & 1) == 0))
			{
				z = MODULE_FUN_NAME(AP, mul)(x, t = z);
				MODULE_FUN_NAME(AP, free)(&t);
			}
		}
	}

	return z;
}


int MODULE_FUN_NAME(AP, cmp)(T x, T y)
{
	assert(x);
	assert(y);

	if (!((x->sign ^ y->sign) == 0))
		return x->sign;
	else if (x->sign == 1)
		return cmp(x, y);
	else
		return cmp(y, x);
}

T MODULE_FUN_NAME(AP, addi)(T x, long int y)
{
	unsigned char d[sizeof(unsigned long)];
	struct T t;

	t.size = sizeof(d);
	t.digits = d;

	return MODULE_FUN_NAME(AP, add)(x, set(&t, y));
}

T MODULE_FUN_NAME(AP, subi)(T x, long int y)
{
	unsigned char d[sizeof(unsigned long)];
	struct T t;

	t.size = sizeof(d);
	t.digits = d;

	return MODULE_FUN_NAME(AP, sub)(x, set(&t, y));
}

T MODULE_FUN_NAME(AP, muli)(T x, long int y)
{
	unsigned char d[sizeof(unsigned long)];
	struct T t;

	t.size = sizeof(d);
	t.digits = d;

	return MODULE_FUN_NAME(AP, mul)(x, set(&t, y));
}

T MODULE_FUN_NAME(AP, divi)(T x, long int y)
{
	unsigned char d[sizeof(unsigned long)];
	struct T t;

	t.size = sizeof(d);
	t.digits = d;

	return MODULE_FUN_NAME(AP, div)(x, set(&t, y));
}

int MODULE_FUN_NAME(AP, cmpi)(T x, long int y)
{
	unsigned char d[sizeof(unsigned long)];
	struct T t;

	t.size = sizeof(d);
	t.digits = d;

	return MODULE_FUN_NAME(AP, cmp)(x, set(&t, y));
}

long int MODULE_FUN_NAME(AP, modi)(T x, long int y)
{
	long int rem;
	T r;
	unsigned char d[sizeof(unsigned long)];
	struct T t;

	t.size = sizeof(d);
	t.digits = d;

	r = MODULE_FUN_NAME(AP, mod)(x, set(&t, y));
	rem = MODULE_FUN_NAME(XP, toint)(r->ndigits, r->digits);
	MODULE_FUN_NAME(AP, free)(&r);

	return rem;
}

T MODULE_FUN_NAME(AP, lshift)(T x, int s)
{
	T z;

	assert(x);
	assert(s >= 0);

	z = mk(x->ndigits + ((s + 7) & ~7) / 8);
	MODULE_FUN_NAME(XP, lshift)(z->size, z->digits, x->ndigits,
					x->digits, s, 0);
	z->sign = x->sign;
	return normalize(z, z->size);
}

T MODULE_FUN_NAME(AP, rshift)(T x, int s)
{
	assert(x);
	assert(s >= 0);

	if (s >= 8 * x->ndigits)
		return MODULE_FUN_NAME(AP, new)(0);
	else
	{
		T z = mk(x->ndigits - s / 8);
		MODULE_FUN_NAME(XP, rshift)
				(z->size, z->digits, x->ndigits, x->digits, s, 0);
		normalize(z, z->size);
		z->sign = ISZERO(z) ? 1 : x->sign;
		return z;
	}
}

T MODULE_FUN_NAME(AP, fromstr)
	(const char *str, int base, char **end)
{
	T z;
	const char *p = NULL;
	char *endp = NULL;
	char sign = '\0';
	int carry;
	const char *start = NULL;
	int k;
	int n = 0;

	assert(p);
	assert(base >= 2 && base <= 36);

	p = str;
	while (*p && isspace(*p))
		p++;

	if (*p == '-' || *p == '+')
		sign = *p++;

	for (; *p == '0' && p[1] == '0'; p++)
		;

	start = p;

	for (; '0' <= *p && *p <= '9' && *p < '0' + base
				|| 'a' <= *p && *p <= 'z' && *p < 'a' + base - 10
				|| 'A' <= *p && *p <= 'Z' && *p < 'A' + base - 10; p++)
		n++;

	for (k = 1; (1 << k) < base; k++)
		;

	z = mk(((k * n + 7) & ~7) / 8);

	p = start;

	carry = MODULE_FUN_NAME(XP, fromstr)
			(z->size, z->digits, p, base, &endp);

	assert(carry == 0);

	normalize(z, z->size);
	if (endp == p)
	{
		endp = (char *)str;
		z = MODULE_FUN_NAME(AP, new)(0);
	}
	else
	{
		z->sign = ISZERO(z) || sign != '-' ? 1 : -1;
	}

	if (end)
	{
		*end = (char*)endp;
	}

	return z;
}

char *MODULE_FUN_NAME(AP, tostr)(char *str, int size, int base, T x)
{
	XP_T q;

	assert(x);
	assert(base >= 2 && base <= 36);
	assert(str == NULL || size > 1);

	if (NULL == str)
	{
		int k; 

		for (k = 5; (1 << k) > base; k--)
				;

		size = (8 * x->ndigits) / k + 1 + 1;
//		if (x->sign == 1) // ?? -1
		if (x->sign == -1) 
			size++;

		str = (char *)calloc(1, size * sizeof(char));
	}

	q = (XP_T)calloc(1, x->ndigits * sizeof(unsigned char));
	memcpy(q, x->digits, x->ndigits);

	if (x->sign == -1)
	{
		str[0] = '-';
		MODULE_FUN_NAME(XP, tostr)(str + 1, size - 1, base, x->ndigits, q);
	}
	else
	{
		MODULE_FUN_NAME(XP, tostr)(str, size, base, x->ndigits, q);
	}
	free(q);

	return str;
}

void MODULE_FUN_NAME(AP, fmt)
				(int code, va_list *app,
				int put(int c, void *cl), void *cl,
				unsigned char flags[], int width, int precision)
{
	T x;
	char *buf;

	assert(app && flags);

	x = va_arg(*app, T);

	assert(x);

	buf = MODULE_FUN_NAME(AP, tostr)(NULL, 0, 10, x);
//	Fmt_putd(buf, strlen(buf), put, cl, flags, width, precision);
	free(buf);
}


