
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "xp.h"
#include "mp.h"

#define T MP_T


// < macros 374 >
#define SIGN(x) ((x)[nbytes - 1] >> shift)
#define ONES(n) (~(~0UL << (((n) - 1) % 8 + 1)))
#define ISZERO(x) (MODULE_FUN_NAME(XP, length)(nbytes, (x)) == 1 && (x)[0] == 0)
#define BASE	(1 << 8)
#define BITOP(op)	\
		int i;	\
		assert(z);	\
		assert(x);	\
		assert(y);	\
		for (i = 0; i < nbytes; i++)	\
			z[i] = x[i] op y[i];	\
		return z
#define BITOPI(op)	\
		assert(z); assert(x);	\
		applyu(op, z, x, y);	\
		return z
#define SHFT(fill, op)	\
		assert(x); assert(z); assert(s >= 0);	\
		if (s >= nbits) memset(z, fill, nbytes);	\
		else op(nbytes, z, nbytes, x, s, fill);		\
		z[nbytes - 1] &= msb;	\
		return z


// < data 373 >
//#define RAISE(msg)	
//typedef char *Except_T;
const Except_T MP_Dividebyzero = { "Division by zero" };
const Except_T MP_Overflow = { "Overflow" };

static int nbits = 32;
static int nbytes = (32 - 1) / 8 + 1;
static int shift = (32 - 1) % 8;
static unsigned char msb = 0xFF;

static unsigned char temp[16 + 16 + 16 + 2 * 16 + 2];
static T tmp[] = { temp, temp + 1 * 16, temp + 2 * 16,
		temp + 3 * 16 };

// < static functions 389 >
static int applyu(T op(T, T, T), T z, T x, unsigned long u)
{
	unsigned long carry;

	{
		T z = tmp[2];

		carry = MODULE_FUN_NAME(XP, fromint)(nbytes, z, u);
		carry |= z[nbytes - 1] & ~msb;
		z[nbytes - 1] &= msb;
	}

	op(z, x, tmp[2]);
	return carry != 0;
}

static int apply(T op(T, T, T), T z, T x, long v)
{
	{ 
		T z = tmp[2];

		if (v == LONG_MIN)
		{
			MODULE_FUN_NAME(XP, fromint)(nbytes, z, LONG_MAX + 1UL);
			MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
		}
		else if (v < 0)
		{
			MODULE_FUN_NAME(XP, fromint)(nbytes, z, -v);
			MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
		}
		else
		{
			MODULE_FUN_NAME(XP, fromint)(nbytes, z, v);
		}
		z[nbytes - 1] &= msb;
	}

	op(z, x, tmp[2]);
	return (nbits < 8 * (int) sizeof(v) && 
	    (v < -(1L << (nbits - 1)) || v >= (1L << (nbits - 1))));
}

// < functions 374 >
int MODULE_FUN_NAME(MP, set)(int n)
{
	int prev = 0;

	assert(n > 1);

	prev = nbits;

	nbits = n;
	nbytes = (n - 1) / 8 + 1;
	shift = (n - 1) % 8;
	msb = ONES(n);

	if (tmp[0] != temp)
	{
		free(tmp[0]);
	}
	if (nbytes <= 16)
	{
		tmp[0] = temp;
	}
	else
	{
		tmp[0] = (T)calloc(1, (3 * nbytes + 2 * nbytes + 2) * sizeof(char));
	}
	tmp[1] = tmp[0] + 1 * nbytes;
	tmp[2] = tmp[0] + 2 * nbytes;
	tmp[3] = tmp[0] + 3 * nbytes;
}

T MODULE_FUN_NAME(MP, new)(unsigned long u)
{
	return MODULE_FUN_NAME(MP, fromintu)
			(calloc(1, nbytes * sizeof(char)), u);
}

T MODULE_FUN_NAME(MP, fromintu)(T z, unsigned long u)
{
	unsigned long carry;

	assert(z);

	carry = MODULE_FUN_NAME(XP, fromint)(nbytes, z, u);
	carry |= z[nbytes - 1] & ~msb;
	z[nbytes - 1] &= msb;

	if (carry)
	{ // TODO
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, fromint)(T z, long v)
{
	assert(z);

	if (v == LONG_MIN)
	{
		MODULE_FUN_NAME(XP, fromint)(nbytes, z, LONG_MAX + 1UL);
		MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
	}
	else if (v < 0)
	{
		MODULE_FUN_NAME(XP, fromint)(nbytes, z, -v);
		MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
	}
	else
	{
		MODULE_FUN_NAME(XP, fromint)(nbytes, z, v);
	}
	z[nbytes - 1] &= msb;

	if (nbits < 8 * (int)sizeof(v) &&
			(v < -(1L << (nbits - 1)) || v >= (1L << (nbits - 1))))
	{ // TODO
		RAISE(MP_Overflow);
	}

	return z;
}

long MODULE_FUN_NAME(MP, toint)(T x)
{
	unsigned char d[sizeof(unsigned long)];

	assert(x);

	MODULE_FUN_NAME(MP, cvt)(8 * sizeof(d), d, x);
	return MODULE_FUN_NAME(XP, toint)(sizeof(d), d);
}

T MODULE_FUN_NAME(MP, cvt)(int m, T z, T x)
{
	int fill;
	int i;
	int mbytes = (m - 1) / 8 + 1;

	assert(m > 1);
	assert(x);
	assert(z);

	fill = SIGN(x) ? 0xFF : 0;
	if (m < nbits)
	{
		int carry = (x[mbytes - 1] ^ fill) & ~ONES(m);

		for (i = mbytes; i < nbytes; i++)
		{
			carry |= x[i] ^ fill;
		}
		memcpy(z, x, mbytes);
		z[mbytes - 1] &= ONES(m);
		if (carry)
		{ // TODO
			RAISE(MP_Overflow);
		}
	}
	else
	{
		memcpy(z, x, nbytes);
		z[nbytes - 1] |= fill & ~msb;
		for (i = nbytes; i < mbytes; i++)
		{
			z[i] = fill;
		}
		z[mbytes - 1] &= ONES(m);
	}

	return z;
}

unsigned long MODULE_FUN_NAME(MP, tointu)(T x)
{
	unsigned char d[sizeof(unsigned long)];

	assert(x);
	MODULE_FUN_NAME(MP, cvtu)(8 * sizeof(d), d, x);
	return MODULE_FUN_NAME(XP, toint)(sizeof(d), d);
}

T MODULE_FUN_NAME(MP, cvtu)(int m, T z, T x)
{
	int i;
	int mbytes = (m - 1) / 8 + 1;

	assert(m > 1);
	assert(x);
	assert(z);

	if (m < nbits)
	{
		int carry = x[mbytes - 1] & ~ONES(m);

		for (i = mbytes; i < nbytes; i++)
		{
			carry |= x[i];
		}
		memcpy(z, x, mbytes);
		z[mbytes - 1] &= ONES(m);

		if (carry)
		{ // TODO
			RAISE(MP_Overflow);
		}
	}
	else
	{
		memcpy(z, x, nbytes);
		for (i = nbytes; i < mbytes; i++)
			z[i] = 0;
	}

	return z;
}

T MODULE_FUN_NAME(MP, addu)(T z, T x, T y)
{
	int carry;

	assert(x);
	assert(y);
	assert(z);

	carry = MODULE_FUN_NAME(XP, add)(nbytes, z, x, y, 0);
	carry |= z[nbytes - 1] & ~msb;
	z[nbytes - 1] &= msb;

	if (carry)
	{ // TODO
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, subu)(T z, T x, T y)
{
	int borrow;

	assert(x);
	assert(y);
	assert(z);

	borrow = MODULE_FUN_NAME(XP, sub)(nbytes, z, x, y, 0);
	borrow |= z[nbytes - 1] & ~msb;
	z[nbytes - 1] &= msb;

	if (borrow)
	{ // TODO
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, mul2u)(T z, T x, T y)
{
	assert(x);
	assert(y);
	assert(z);

	memset(tmp[3], '\0', 2 * nbytes);
	MODULE_FUN_NAME(XP, mul)(tmp[3], nbytes, x, nbytes, y);
	memcpy(z, tmp[3], (2 * nbits - 1) / 8 + 1);

	return z;
}

T MODULE_FUN_NAME(MP, mulu)(T z, T x, T y)
{
	int i;

	assert(x);
	assert(y);
	assert(z);

	memset(tmp[3], '\0', 2 * nbytes);
	MODULE_FUN_NAME(XP, mul)(tmp[3], nbytes, x, nbytes, y);
	memcpy(z, tmp[3], nbytes);
	z[nbytes - 1] &= msb;

	if (tmp[3][nbytes - 1] & ~msb)
	{
		RAISE(MP_Overflow);
	}

	for (i = 0; i < nbytes; i++)
		if (tmp[3][i + nbytes] != 0)
			RAISE(MP_Overflow);

	return z;
}

T MODULE_FUN_NAME(MP, divu)(T z, T x, T y)
{
	assert(x);
	assert(y);
	assert(z);
	
	memcpy(tmp[1], y, nbytes);
	y = tmp[1];

	if (!MODULE_FUN_NAME(XP, div)(nbytes, z, x, nbytes, y, tmp[2], tmp[3]))
		RAISE(MP_Dividebyzero);

	return z;
}

T MODULE_FUN_NAME(MP, modu)(T z, T x, T y)
{
	assert(x);
	assert(y);
	assert(z);

	memcpy(tmp[1], y, nbytes);
	y = tmp[1];

	if (!MODULE_FUN_NAME(XP, div)(nbytes, tmp[2], x, nbytes, y, z, tmp[3]))
		RAISE(MP_Dividebyzero);

	return z;
}

T MODULE_FUN_NAME(MP, add)(T z, T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);
	assert(z);

	sx = SIGN(x);
	sy = SIGN(y);

	MODULE_FUN_NAME(XP, add)(nbytes, z, x, y, 0);
	z[nbytes - 1] &= msb;

	if (sx == sy && sy != SIGN(z))
		RAISE(MP_Overflow);

	return z;
}

T MODULE_FUN_NAME(MP, sub)(T z, T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);
	assert(z);

	sx = SIGN(x);
	sy = SIGN(y);

	MODULE_FUN_NAME(XP, sub)(nbytes, z, x, y, 0);
	z[nbytes - 1] &= msb;

	if (sx == sy && sy != SIGN(z))
		RAISE(MP_Overflow);

	return z;
}

T MODULE_FUN_NAME(MP, neg)(T z, T x)
{
	int sx;

	assert(z);
	assert(x);

	sx = SIGN(x);
	MODULE_FUN_NAME(XP, neg)(nbytes, z, x, 1);
	z[nbytes - 1] &= msb;

	if (sx && sx == SIGN(z))
		RAISE(MP_Overflow);

	return z;
}

T MODULE_FUN_NAME(MP, mul2)(T z, T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);
	assert(z);

	sx = SIGN(x);
	sy = SIGN(y);

	if (sx)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
		x = tmp[0];
		x[nbytes -1 ] &= msb;
	}

	if (sy)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], y, 1);
		y = tmp[0];
		y[nbytes -1 ] &= msb;
	}

	memset(tmp[3], '\0', 2 * nbytes);
	MODULE_FUN_NAME(XP, mul)(tmp[3], nbytes, x, nbytes, y);

	if (sx != sy)
	{
		MODULE_FUN_NAME(XP, neg)((2 * nbits - 1) / 8 + 1, z, tmp[3], 1);
	}
	else
	{
		memcpy(z, tmp[3], (2 * nbits - 1) / 8 + 1);
	}

	return z;
}

T MODULE_FUN_NAME(MP, mul)(T z, T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);
	assert(z);

	sx = SIGN(x);
	sy = SIGN(y);

	if (sx)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
		x = tmp[0];
		x[nbytes -1 ] &= msb;
	}

	if (sy)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], y, 1);
		y = tmp[0];
		y[nbytes -1 ] &= msb;
	}

	memset(tmp[3], '\0', 2 * nbytes);
	MODULE_FUN_NAME(XP, mul)(tmp[3], nbytes, x, nbytes, y);

	if (sx != sy)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, z, tmp[3], 1);
	}
	else
	{
		memcpy(z, tmp[3], nbytes);
	}
	z[nbytes - 1] &= msb;

	{
	int i;

	if (tmp[3][nbytes - 1] & ~msb)
	{
		RAISE(MP_Overflow);
	}
	for (i = 0; i < nbytes; i++)
		if (tmp[3][i + nbytes] != 0)
			RAISE(MP_Overflow);
	}

	if (sx == sy && SIGN(z))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, div)(T z, T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);
	assert(z);

	sx = SIGN(x);
	sy = SIGN(y);

	if (sx)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
		x = tmp[0];
		x[nbytes -1 ] &= msb;
	}

	if (sy)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], y, 1);
		y = tmp[0];
		y[nbytes -1 ] &= msb;
	}
	else
	{
		memcpy(tmp[1], y, nbytes);
		y = tmp[1];
	}

	if (!MODULE_FUN_NAME(XP, div)(nbytes, z, x, nbytes, y, tmp[2], tmp[3]))
		RAISE(MP_Dividebyzero);

	if (sx != sy)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
		if (!ISZERO(tmp[2]))
			MODULE_FUN_NAME(XP, diff)(nbytes, z, z, 1);
		z[nbytes - 1] &= msb;
	}
	else if (sx && SIGN(z))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, mod)(T z, T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);
	assert(z);

	sx = SIGN(x);
	sy = SIGN(y);

	if (sx)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
		x = tmp[0];
		x[nbytes -1 ] &= msb;
	}

	if (sy)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], y, 1);
		y = tmp[0];
		y[nbytes -1 ] &= msb;
	}
	else
	{
		memcpy(tmp[1], y, nbytes);
		y = tmp[1];
	}

	if (!MODULE_FUN_NAME(XP, div)(nbytes, tmp[2], x, nbytes, y, z, tmp[3]))
		RAISE(MP_Dividebyzero);

	if (sx != sy)
	{
		if (!ISZERO(z))
			MODULE_FUN_NAME(XP, sub)(nbytes, z, y, z, 0);
	}
	else if (sx && SIGN(tmp[2]))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, addui)(T z, T x, unsigned long y)
{
	assert(z);
	assert(x);

	if (y < BASE)
	{
		int carry = MODULE_FUN_NAME(XP, sum)(nbytes, z, x, y);

		carry |= z[nbytes - 1] & ~msb;
		z[nbytes - 1] &= msb;

		if (carry)
			RAISE(MP_Overflow);
	}
	else if (applyu(MODULE_FUN_NAME(MP, addu), z, x, y))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, subui)(T z, T x, unsigned long y)
{
	assert(z);
	assert(x);
	
	if (y < BASE)
	{
		int borrow = MODULE_FUN_NAME(XP, diff)(nbytes, z, x, y);
		borrow |= z[nbytes - 1] & ~msb;
		z[nbytes - 1] &= msb;

		if (borrow)
			RAISE(MP_Overflow);
	}
	else if (applyu(MODULE_FUN_NAME(MP, subu), z, x, y))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, mului)(T z, T x, unsigned long y)
{
	assert(z);
	assert(x);
	
	if (y < BASE)
	{
		int carry= MODULE_FUN_NAME(XP, product)(nbytes, z, x, y);
		carry |= z[nbytes - 1] & ~msb;
		z[nbytes - 1] &= msb;

		if (carry)
			RAISE(MP_Overflow);
		if (nbits < 8 && y >= (1U << nbits))
			RAISE(MP_Overflow);
	}
	else if (applyu(MODULE_FUN_NAME(MP, mulu), z, x, y))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, divui)(T z, T x, unsigned long y)
{
	assert(z);
	assert(x);

	if (0 == y)
		RAISE(MP_Dividebyzero);
	else if (y < BASE)
	{
		MODULE_FUN_NAME(XP, quotient)(nbytes, z, x, y);

		if (nbits < 8 && y >= (1U << nbits))
			RAISE(MP_Overflow);
	}
	else if (applyu(MODULE_FUN_NAME(MP, divu), z, x, y))
		RAISE(MP_Overflow);

	return z;
}

unsigned long MODULE_FUN_NAME(MP, modui)(T x, unsigned long y)
{
	assert(x);

	if (0 == y)
		RAISE(MP_Dividebyzero);
	else if (y < BASE)
	{
		int r = MODULE_FUN_NAME(XP, quotient)(nbytes, tmp[2], x, y);

		if (nbits < 8 && y >= (1U << nbits))
			RAISE(MP_Overflow);

		return r;
	}
	else if (apply(MODULE_FUN_NAME(MP, modu), tmp[2], x, y))
		RAISE(MP_Overflow);

	return MODULE_FUN_NAME(XP, toint)(nbytes, tmp[2]);
}

T MODULE_FUN_NAME(MP, addi)(T z, T x, long y)
{
	assert(z);
	assert(x);

	if (-BASE < y && y < BASE)
	{
		int sx = SIGN(x);
		int sy = y < 0;

		if (sy)
			MODULE_FUN_NAME(XP, diff)(nbytes, z, x, -y);
		else
			MODULE_FUN_NAME(XP, sum)(nbytes, z, x, y);
		z[nbytes - 1] &= msb;
		
		if (sx != sy && sy == SIGN(z))
			RAISE(MP_Overflow);
		if (nbits < 8
			 && (y < -(1 << (nbits - 1)) || y >= (1 << (nbits - 1))))
			RAISE(MP_Overflow);
	}
	else if (apply(MODULE_FUN_NAME(MP, add), z, x, y))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, subi)(T z, T x, long y)
{
	assert(z);
	assert(x);

	if (-BASE < y && y < BASE)
	{
		int sx = SIGN(x);
		int sy = y < 0;

		if (sy)
			MODULE_FUN_NAME(XP, sum)(nbytes, z, x, -y);
		else
			MODULE_FUN_NAME(XP, diff)(nbytes, z, x, y);
		z[nbytes - 1] &= msb;
		if (sx != sy && sy == SIGN(z))
			RAISE(MP_Overflow);
		if (nbits < 8
			 && (y < -(1 << (nbits - 1)) || y >= (1 << (nbits - 1))))
			RAISE(MP_Overflow);
	}
	else if (apply(MODULE_FUN_NAME(MP, sub), z, x, y))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, muli)(T z, T x, long y)
{
	assert(z);
	assert(x);

	if (-BASE < y && y < BASE)
	{
		int sx = SIGN(x);
		int sy = y < 0;

		if (sx)
		{
			MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
			x = tmp[0];
			x[nbytes -1 ] &= msb;
		}

		MODULE_FUN_NAME(XP, product)(nbytes, z, x, sy ? -y : y);
		if (sx != sy)
			MODULE_FUN_NAME(XP, neg)(nbytes, z, x, 1);
		z[nbytes - 1] &= msb;

		if (sx == sy && SIGN(z))
			RAISE(MP_Overflow);
		if (nbits < 8
			 && (y < -(1 << (nbits - 1)) || y >= (1 << (nbits - 1))))
			RAISE(MP_Overflow);
	}
	else if (apply(MODULE_FUN_NAME(MP, mul), z, x, y))
	{
		RAISE(MP_Overflow);
	}

	return z;
}

T MODULE_FUN_NAME(MP, divi)(T z, T x, long y)
{
	assert(z);
	assert(x);

	if (0 == y)
		RAISE(MP_Dividebyzero);
	else if (-BASE < y && y < BASE)
	{
		int r;

		// <395>;
		int sx = SIGN(x);
		int sy = y < 0;

	//	<386>;
		if (sx)
		{
			MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
			x = tmp[0];
			x[nbytes -1 ] &= msb;
		}

		r = MODULE_FUN_NAME(XP, quotient)(nbytes, z, x, sy ? -y : y);
		if (sx != sy)
		{
			MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
			if (0 != r)
			{
				MODULE_FUN_NAME(XP, diff)(nbytes, z, z, 1);
				r = y - r;
			}
			z[nbytes - 1] &= msb;
		}
		else if (sx && SIGN(z))
			RAISE(MP_Overflow);

		if (nbits < 8
			 && (y < -(1 << (nbits - 1)) || y >= (1 << (nbits - 1))))
			RAISE(MP_Overflow);
	}
	else if (apply(MODULE_FUN_NAME(MP, div), z, x, y))
		RAISE(MP_Overflow);
	return z;
}

long MODULE_FUN_NAME(MP, modi)(T x, long y)
{
	assert(x);

	if (0 == y)
		RAISE(MP_Dividebyzero);
	else if (-BASE < y && y < BASE)
	{
		T z = tmp[2];
		int r;

		//<395>;
		int sx = SIGN(x);
		int sy = y < 0;

	//	<386>;
		if (sx)
		{
			MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
			x = tmp[0];
			x[nbytes -1 ] &= msb;
		}

		r = MODULE_FUN_NAME(XP, quotient)(nbytes, z, x, sy ? -y : y);
		if (sx != sy)
		{
			MODULE_FUN_NAME(XP, neg)(nbytes, z, z, 1);
			if (0 != r)
			{
				MODULE_FUN_NAME(XP, diff)(nbytes, z, z, 1);
				r = y - r;
			}
			z[nbytes - 1] &= msb;
		}
		else if (sx && SIGN(z))
			RAISE(MP_Overflow);

		if (nbits < 8
			 && (y < -(1 << (nbits - 1)) || y >= (1 << (nbits - 1))))
			RAISE(MP_Overflow);

		return r;
	}
	else if (apply(MODULE_FUN_NAME(MP, mod), tmp[2], x, y))
		RAISE(MP_Overflow);

	return MODULE_FUN_NAME(MP, toint)(tmp[2]);
}


int MODULE_FUN_NAME(MP, cmpu)(T x, T y)
{
	assert(x);
	assert(y);

	return MODULE_FUN_NAME(XP, cmp)(nbytes, x, y);
}

int MODULE_FUN_NAME(MP, cmp)(T x, T y)
{
	int sx;
	int sy;

	assert(x);
	assert(y);

	sx = SIGN(x);
	sy = SIGN(y);

	if (sx != sy)
		return sy - sx;
	else
		return MODULE_FUN_NAME(XP, cmp)(nbytes, x, y);
}

int MODULE_FUN_NAME(MP, cmpui)(T x, unsigned long y)
{
	assert(x);

	if ((int)sizeof(y) >= nbytes)
	{
		unsigned long v = MODULE_FUN_NAME(XP, toint)(nbytes, x);
		if (v < y)
			return -1;
		else if (v > y)
			return 1;
		else
			return 0;
	}
	else
	{
		MODULE_FUN_NAME(XP, fromint)(nbytes, tmp[2], y);
		return MODULE_FUN_NAME(XP, cmp)(nbytes, x, tmp[2]);
	}
}

int MODULE_FUN_NAME(MP, cmpi)(T x, long y)
{
	int sx;
	int sy;

	assert(x);

	sx = SIGN(x);
	sy = y < 0;

	if (sx != sy)
	{
		return sy - sx;
	}
	else if ((int)sizeof(y) >= nbytes)
	{
		long v = MODULE_FUN_NAME(MP, toint)(x);

		if (v < y)
			return -1;
		else if (v > y)
			return 1;
		else
			return 0;
	}
	else
	{
		MODULE_FUN_NAME(MP, fromint)(tmp[2], y);
		return MODULE_FUN_NAME(XP, cmp)(nbytes, x, tmp[2]);
	}
}


T MODULE_FUN_NAME(MP, and)
		(T z, T x, T y) { BITOP(&); }
T MODULE_FUN_NAME(MP, or)
		(T z, T x, T y) { BITOP(|); }
T MODULE_FUN_NAME(MP, xor)
		(T z, T x, T y) { BITOP(^); }

T MODULE_FUN_NAME(MP, not)(T z, T x)
{
	int i;

	assert(z);
	assert(x);

	for (i = 0; i < nbytes; i++)
	{
		z[i] = ~x[i];
	}

	z[nbytes - 1] &= msb;

	return z;
}

T MODULE_FUN_NAME(MP, andi)
		(T z, T x, unsigned long y) 
{ BITOPI(MODULE_FUN_NAME(MP, and)); }

T MODULE_FUN_NAME(MP, ori)
		(T z, T x, unsigned long y) 
{ BITOPI(MODULE_FUN_NAME(MP, or)); }

T MODULE_FUN_NAME(MP, xori)
		(T z, T x, unsigned long y) 
{ BITOPI(MODULE_FUN_NAME(MP, xor)); }

T MODULE_FUN_NAME(MP, lshift)
		(T z, T x, int s) { SHFT(0, MODULE_FUN_NAME(XP, lshift)); }
T MODULE_FUN_NAME(MP, rshift)
		(T z, T x, int s) { SHFT(0, MODULE_FUN_NAME(XP, rshift)); }
T MODULE_FUN_NAME(MP, ashift)
		(T z, T x, int s) { SHFT(SIGN(x), MODULE_FUN_NAME(XP, rshift)); }

T MODULE_FUN_NAME(MP, fromstr)
	(T z, const char *str, int base, char **end)
{
	int carry;

	assert(z);

	memset(z, '\0', nbytes);
	carry = MODULE_FUN_NAME(XP, fromstr)(nbytes, z, str, base, end);
	carry |= z[nbytes - 1] & ~msb;
	z[nbytes - 1] &= msb;

//	<376>;
	if (carry)
		RAISE(MP_Overflow);
	return z;
}

char *MODULE_FUN_NAME(MP, tostr)
	(char *str, int size, int base, T x)
{
	assert(x);
	assert(base >= 2 && base <= 36);
	assert(str == NULL || size > 1);

	if (NULL == str)
	{
		int k;
		for (k = 5; (1 << k) > base; k--)
			;
		size = nbits / k + 1 + 1;

		str = calloc(1, size * sizeof(char));
	}

	memcpy(tmp[1], x, nbytes);
	MODULE_FUN_NAME(XP, tostr)(str, size, base, nbytes, tmp[1]);

	return str;
}

void MODULE_FUN_NAME(MP, fmtu)(int code, va_list *app,
			int put(int c, void *cl), void *cl,
			unsigned char flags[], int width, int precision)
{
	T x;
	char *buf;

	assert(app && flags);
	x = va_arg(*app, T);
	assert(x);
	buf = MODULE_FUN_NAME(MP, tostr)
			(NULL, 0, va_arg(*app, int), x);
	Fmt_putd(buf, strlen(buf), put, cl, flags, width, precision);
	free(buf);
}

void MODULE_FUN_NAME(MP, fmt)(int code, va_list *app,
			int put(int c, void *cl), void *cl,
			unsigned char flags[], int width, int precision)
{
	T x;
	int base;
	int size;
	int sx;
	char *buf;

	assert(app && flags);

	x = va_arg(*app, T);
	assert(x);
	base = va_arg(*app, int);
	assert(base >= 2 && base <= 36);
	sx = SIGN(x);

//	<386>;
	if (sx)
	{
		MODULE_FUN_NAME(XP, neg)(nbytes, tmp[0], x, 1);
		x = tmp[0];
		x[nbytes -1 ] &= msb;
	}

	//<400>;
	int k;
	for (k = 5; (1 << k) > base; k--)
		;
	size = nbits / k + 1 + 1;

	buf = calloc(1, (size + 1) * sizeof(char));

	if (sx)
	{
		buf[0] = '-';
		MODULE_FUN_NAME(MP, tostr)(buf + 1, size, base, x);
	}
	else
	{
		MODULE_FUN_NAME(MP, tostr)(buf, size, base, x);
	}

	Fmt_putd(buf, strlen(buf), put, cl, flags, width, precision);
	free(buf);
}



