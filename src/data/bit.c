
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "bit.h"

#define T Bit_T

struct T {
	int length;
	unsigned char *bytes;
	unsigned long *words;
};

// < macros 203 >
#define BPW		(8 * sizeof(unsigned long))	// (8 bits per byte)  * (bytes of unsigned long)
#define NWORDS(len)	((((len) + BPW - 1) & (~(BPW - 1))) / BPW)
#define NBYTES(len) ((((len) + 8 - 1) & (~(8 - 1))) / 8)
#define SETOP(sequal, snull, tnull, op) \
		if (s == t) { assert(s); return sequal; } \
		else if (NULL == s) { assert(t); return snull; } \
		else if (NULL == t) { return tnull; } \
		else { \
			int i;	\
			T set = NULL;	\
								\
			assert(s->length == t->length); \
			set = MODULE_FUN_NAME(Bit, new)(s->length); \
			for (i = NWORDS(s->length); --i >= 0; ) \
			{ \
				set->words[i] = s->words[i] op t->words[i]; \
			} \
			return set; \
		}


// < static data 207 >
static unsigned char msbmask[] = {
	0xFF, 0xFE, 0xFC, 0xF8,
	0xF0, 0xE0, 0xC0, 0x80
};

static unsigned char lsbmask[] = {
	0x01, 0x03, 0x07, 0x0F,
	0x1F, 0x3F, 0x7F, 0xFF
};

// < static function 212 >
static T copy(T t)
{
	T set = NULL;

	assert(t);

	set = MODULE_FUN_NAME(Bit, new)(t->length);
	if (t->length > 0)
		memcpy(set->bytes, t->bytes, NBYTES(t->length));

	return set;
}

/*
 * name: MODULE_FUN_NAME(Bit, new)
 * description: create a new bit set
 * return value: return new bit set
 * args: @length: total length of bits
 */
T MODULE_FUN_NAME(Bit, new)(int length)
{
	T set = NULL;

	assert(length >= 0);

	set = (T)calloc(1, sizeof(*set));
	if (set)
	{
		if (length > 0)
		{
			set->words = (unsigned long *)calloc(1, NWORDS(length) * sizeof(unsigned long));
		}
		else
		{
			set->words = NULL;
		}

		set->bytes = (unsigned char *)set->words;
		set->length = length;
	}

	return set;
}

/*
 * name: MODULE_FUN_NAME(Bit, free)
 * description: free a allocated bit set
 * return value: void
 * args: @length: pointer's pointer to bit set
 */
void MODULE_FUN_NAME(Bit, free)(T *set)
{
	assert(set && *set);

	free((*set)->words);
	free(*set);
	*set = NULL;
}

/*
 * name: MODULE_FUN_NAME(Bit, length)
 * description: get the length of bit set 
 * return value: length of bit set
 * args: @set: pointer to bit set
 */
int MODULE_FUN_NAME(Bit, length)(T set)
{
	assert(set);

	return set->length;
}

/*
 * name: MODULE_FUN_NAME(Bit, count)
 * description: get the total number of 1 in bit set 
 * return value: total number bits setted to 1 in bit set
 * args: @set: pointer to bit set
 */
int MODULE_FUN_NAME(Bit, count)(T set)
{
	unsigned char c = 0;
	int length = 0;
	int n = 0;

	/*
	 * index  bit0 bit1 bit2 bit3 number of 1
	 *   0    	0   0    0    0    0
	 *   1    	1   0    0    0    1
	 *   2    	0   1    0    0    1
	 *   3		1   1    0    0    2
	 *   4		0   0    1    0    1
	 *   5		1   0    1    0    2
	 *   6		0   1    1    0    2
	 *   7		1   1    1    0    3
	 *   8		0   0    0    1    1
	 *   9		1   0    0    1    2
	 *   10		0   1    0    1    2
	 *   11		1   1    0    1    3
	 *   12		0   0    1    1    2
	 *   13		1   0    1    1    3
	 *   14		0   1    1    1    3
	 *   15		1   1    1    1    4
	 */
	static char count[] = {
		0, 1, 1, 2, 
		1, 2, 2, 3, 
		1, 2, 2, 3, 
		2, 3, 3, 4
	};

	assert(set);

	for (n = NBYTES(set->length); --n >= 0; )
	{
		c = set->bytes[n];
		length += count[c & 0xF] + count[c >> 4];
	}

	return length;
}

/*
 * name: MODULE_FUN_NAME(Bit, get)
 * description: get the value of @n bit in bit set
 * return value: get the value of @n bit in bit set
 * args: @set: pointer to bit set
 * 		 @n: n bit in bit set
 */
int MODULE_FUN_NAME(Bit, get)(T set, int n)
{
	assert(set);
	assert(n >= 0 && n < set->length);

	return (((set->bytes[n / 8]) >> (n % 8)) & 1);
}


/*
 * name: MODULE_FUN_NAME(Bit, put)
 * description: put the value of @n bit in bit set to be @bit
 * return value: return the prev value of @n bit in bit set
 * args: @set: pointer to bit set
 * 		 @n: n bit in bit set
 * 		 @bit: value to set
 */
int MODULE_FUN_NAME(Bit, put)(T set, int n, int bit)
{
	int prev = 0;

	assert(set);
	assert(0 == bit || 1 == bit);
	assert(n >= 0 && n < set->length);

	prev = (((set->bytes[n / 8]) >> (n % 8)) & 1);

	if (1 == bit)
	{
		set->bytes[n / 8] |= 1 << (n % 8);
	}
	else
	{
		set->bytes[n / 8] &= ~(1 << (n % 8));
	}

	return prev;
}

/*
 * name: MODULE_FUN_NAME(Bit, set)
 * description: set the @lo to @hi bits in bit set to be 1
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @lo: the start position of bit set to set
 * 		 @hi: the end position of bit set to set
 */
void MODULE_FUN_NAME(Bit, set)(T set, int lo, int hi)
{
	int i;
	int lo_bytes = 0;
	int hi_bytes = 0;
	int lo_mod = 0;
	int hi_mod = 0;

	assert(set);
	assert(0 <= lo && hi < set->length);
	assert(lo <= hi);

	lo_bytes = lo / 8;
	hi_bytes = hi / 8;
	lo_mod = lo % 8;
	hi_mod = hi % 8;

	if (lo_bytes < hi_bytes)
	{
		set->bytes[lo_bytes] |= msbmask[lo_mod];

		for (i = lo_bytes + 1; i < hi_bytes; i++)
		{
			set->bytes[i] = 0xFF;
		}

		set->bytes[hi_bytes] |= lsbmask[hi_mod];
	}
	else
	{
		set->bytes[lo_bytes] |= lsbmask[hi_mod - lo_mod] << (lo_mod);
	}
}

/*
 * name: MODULE_FUN_NAME(Bit, clear)
 * description: clear the @lo to @hi bits in bit set
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @lo: the start position of bit set to clear
 * 		 @hi: the end position of bit set to clear
 */
void MODULE_FUN_NAME(Bit, clear)(T set, int lo, int hi)
{
	int i;
	int lo_bytes = 0;
	int hi_bytes = 0;
	int lo_mod = 0;
	int hi_mod = 0;

	assert(set);
	assert(0 <= lo && hi < set->length);
	assert(lo <= hi);

	lo_bytes = lo / 8;
	hi_bytes = hi / 8;
	lo_mod = lo % 8;
	hi_mod = hi % 8;

	if (lo_bytes < hi_bytes)
	{
		set->bytes[lo_bytes] &= ~msbmask[lo_mod];
		
		for (i = lo_bytes + 1; i < hi_bytes; i++)
			set->bytes[i] = 0;

		set->bytes[hi_bytes] &= ~lsbmask[hi_mod];
	}
	else
	{
		set->bytes[lo_bytes] &= ~(lsbmask[hi_mod - lo_mod] << lo_mod);
	}
}

/*
 * name: MODULE_FUN_NAME(Bit, not)
 * description: set the @lo to @hi bits in bit set to be ^
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @lo: the start position of bit set to set ^
 * 		 @hi: the end position of bit set to set ^
 */
void MODULE_FUN_NAME(Bit, not)(T set, int lo, int hi)
{
	int i;
	int lo_bytes = 0;
	int hi_bytes = 0;
	int lo_mod = 0;
	int hi_mod = 0;

	assert(set);
	assert(0 <= lo && hi < set->length);
	assert(lo <= hi);

	lo_bytes = lo / 8;
	hi_bytes = hi / 8;
	lo_mod = lo % 8;
	hi_mod = hi % 8;

	if (lo_bytes < hi_bytes)
	{
		set->bytes[lo_bytes] ^= msbmask[lo_mod];
		
		for (i = lo_bytes + 1; i < hi_bytes; i++)
			set->bytes[i] ^= 0xFF;

		set->bytes[hi_bytes] ^= lsbmask[hi_mod];
	}
	else
	{
		set->bytes[lo_bytes] ^= (lsbmask[hi_mod - lo_mod] << lo_mod);
	}
}

/*
 * name: MODULE_FUN_NAME(Bit, map)
 * description: for each bit in @set, call @apply
 * return value: return void
 * args: @set: pointer to bit set
 * 		 @apply: function to call
void apply(int n(index of bit), int bit(bit value), void *cl(user's data)),
 * 		 @cl: user's private data
 */
void MODULE_FUN_NAME(Bit, map)(T set,
			void apply(int n, int bit, void *cl),
			void *cl)
{
	int n;
	int bit = 0;

	assert(set);

	for (n = 0; n < set->length; n++)
	{
		bit = (((set->bytes[n / 8]) >> (n % 8)) & 1);
		apply(n, bit, cl);
	}
}


/*
 * name: MODULE_FUN_NAME(Bit, eq)
 * description: compare @s and @t are equal or not
 * return value: 1: equal; 0 : no equal
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
int MODULE_FUN_NAME(Bit, eq)(T s, T t)
{
	int i;

	assert(s && t);
	assert(s->length == t->length);

	for (i = NWORDS(s->length); --i >= 0; )
	{
		if (s->words[i] != t->words[i])
			return 0;
	}

	return 1;
}

/*
 * name: MODULE_FUN_NAME(Bit, leq)
 * description: compare @s and @t, if @s is sub set of @t or @s=@t,
 * 			return 1, else return 0; 
 * return value: 1:  @s is sub set of @t, or @s = @t
 * 				0: @s is not a sub set of @t
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
int MODULE_FUN_NAME(Bit, leq)(T s, T t)
{
	int i;

	assert(s && t);
	assert(s->length == t->length);

	for (i = NWORDS(s->length); --i >= 0; )
	{
		if ((s->words[i] & ~t->words[i]) != 0)
			return 0;
	}

	return 1;
}

/*
 * name: MODULE_FUN_NAME(Bit, lt)
 * description: compare @s and @t, if @s is sub set of @t,
 * 			return 1, else return 0; 
 * return value: 1:  @s is sub set of @t,
 * 				0: @s is not a sub set of @t
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
int MODULE_FUN_NAME(Bit, lt)(T s, T t)
{
	int i;
	int lt = 0;

	assert(s && t);
	assert(s->length == t->length);

	for (i = NWORDS(s->length); --i >= 0; )
	{
		if ((s->words[i] & ~t->words[i]) != 0)
			return 0;
		else if (s->words[i] != t->words[i])
			lt |= 1;
	}

	return lt;
}

/*
 * name: MODULE_FUN_NAME(Bit, union)
 * description: @s + @t or @s | @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, union)(T s, T t)
{
	SETOP(copy(t), copy(t), copy(s), |);
}

/*
 * name: MODULE_FUN_NAME(Bit, inter)
 * description: @s & @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, inter)(T s, T t)
{
	SETOP(copy(t), MODULE_FUN_NAME(Bit, new)(t->length), 
				MODULE_FUN_NAME(Bit, new)(s->length), &);
}

/*
 * name: MODULE_FUN_NAME(Bit, minus)
 * description: @s &~ @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, minus)(T s, T t)
{
	SETOP(MODULE_FUN_NAME(Bit, new)(s->length), 
				MODULE_FUN_NAME(Bit, new)(t->length), 
				copy(s), & ~);
}

/*
 * name: MODULE_FUN_NAME(Bit, diff)
 * description: @s ^ @t
 * return value: new bit set
 * args: @s: pointer to bit set @s
 * 		 @t: pointer to bit set @t
 */
T MODULE_FUN_NAME(Bit, diff)(T s, T t)
{
	SETOP(MODULE_FUN_NAME(Bit, new)(s->length), 
				copy(t), copy(s), ^);
}



