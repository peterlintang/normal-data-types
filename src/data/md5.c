
/*
 * implement md5 interface
 */

#include <stdio.h>	// for sprintf
#include <stdlib.h>	// for calloc	and free
#include <string.h>	// for strcpy in debug.h
#include "module.h"
#include "interface.h"
#include "debug.h"
#include "data/md5.h"

THIS = {
	.name = "md5",
	.version = "1.0",
	.author = "john lin",
	.description = "md 5",
	.implements = { "MD5", "MD5Transform", NULL },
};

typedef unsigned char *POINTER;

typedef uint16_t UINT2;

typedef uint32_t UINT4;

#define MD5_KEYLEN	16

#define S11	7
#define S12	12
#define S13	17
#define S14	22
#define S21	5
#define S22	9
#define S23	14
#define S24	20
#define S31	4
#define S32	11
#define S33	16
#define S34	23
#define S41	6
#define S42	10
#define S43	15
#define S44	21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define J(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac) {	\
	(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT((a), (s));		\
	(a) += (b);	\
	}
#define GG(a, b, c, d, x, s, ac) {	\
	(a) += G((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT((a), (s));	\
	(a) += (b);	\
	}
#define HH(a, b, c, d, x, s, ac) {	\
	(a) += H((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT((a), (s));	\
	(a) += (b);	\
	}
#define II(a, b, c, d, x, s, ac) {	\
	(a) += J((b), (c), (d)) + (x) + (UINT4)(ac);	\
	(a) = ROTATE_LEFT((a), (s));	\
	(a) += (b);	\
	}

static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static void
__encode(unsigned char *output, UINT4 *input, unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/*
 * assumes @len is multiple of 4
 */
static void 
__decode(UINT4 *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((UINT4)input[j]) | (((UINT4)input[j + 1]) << 8) |
			(((UINT4)input[j + 2]) << 16) | (((UINT4)input[j + 3]) << 24);
}

static void
__transform(UINT4 state[4], unsigned char block[64])
{
	UINT4 a = state[0];
	UINT4 b = state[1];
	UINT4 c = state[2];
	UINT4 d = state[3];
	UINT4 x[16];

	__decode(x, block, 64);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	memset((POINTER)x, 0, sizeof(x));
}

static md5_t *
md5_new(void)
{
	md5_t *digest = calloc(MD5_KEYLEN, sizeof(unsigned char));
	return digest;
}

static void
md5_destroy(md5_t **dpp)
{
	md5_t *mp = NULL;
	if (dpp && (mp = *dpp)) {
		free(mp);
		*dpp = NULL;
	}
}

static int 
md5_compare(const md5_t *a, const md5_t *b)
{
	if (a && b)
		return memcmp(a, b, MD5_KEYLEN);
	return 0;
}

static char *
md5_tostring(md5_t *digest)
{
	char	*string = (char *)calloc(MD5_KEYLEN * 2 + 1, sizeof(char));

	if (string && digest) {
		int i;
		for (i = 0; i < MD5_KEYLEN; i++)
			sprintf(string + (i * 2), "%02x", (unsigned int)digest[i]);
		string[MD5_KEYLEN * 2] = '\0';
	}
	return string;
}

IMPLEMENT_INTERFACE(MD5) = {
	.new = md5_new,
	.destroy = md5_destroy,
	.compare = md5_compare,
	.tostring = md5_tostring
};


static md5_ctx_t *
md5transform_new(void)
{
	md5_ctx_t *context = (md5_ctx_t *)calloc(1, sizeof(*context));
	if (context) {
		context->count[0] = 0;
		context->count[1] = 0;
		context->state[0] = 0x67452301;
		context->state[1] = 0xefcdab89;
		context->state[2] = 0x98badcfe;
		context->state[3] = 0x10325476;
	}
	return context;
}

static void
md5transform_destroy(md5_ctx_t **contextpp)
{
	md5_ctx_t *context = NULL;

	if (contextpp && (context = *contextpp)) {
		memset((POINTER)context, 0, sizeof(*context));
		free(context);
		*contextpp = NULL;
	}
}

static void
md5transform_update(md5_ctx_t *context, 
		unsigned char *input, 
		unsigned int len)
{
	unsigned int i;
	unsigned int index;
	unsigned int partLen;

	if (!context || !input || !len)
		goto out;

	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	if ((context->count[0] += ((UINT4)len << 3)) < (UINT4)len << 3)
		context->count[1]++;
	context->count[1] += ((UINT4)len >> 29);
	partLen = 64 - index;

	if (len >= partLen) {
		memcpy((POINTER)&context->buffer[index], 
				(POINTER)input, partLen);
		__transform(context->state, context->buffer);
		for (i = partLen; i + 63 < len; i += 64)
			__transform(context->state, &input[i]);
	} else {
		i = 0;
	}
	memcpy((POINTER)&context->buffer[index], 
			(POINTER)&input[i], len - i);

out:
	return ;
}

static md5_t *
md5transform_finalize(md5_ctx_t *context)
{
	unsigned char bits[8];
	unsigned int index;
	unsigned int padLen;
	md5_t *digest = I(MD5)->new();

	if (!digest)
		goto out;

	__encode(bits, context->count, 8);
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	md5transform_update(context, PADDING, padLen);
	md5transform_update(context, bits, 8);
	__encode((unsigned char *)digest, context->state, 16);
	memset((POINTER)context, 0, sizeof(*context));

out:
	return digest;
}

IMPLEMENT_INTERFACE(MD5Transform) = {
	.new = md5transform_new,
	.destroy = md5transform_destroy,
	.update = md5transform_update,
	.final = md5transform_finalize
};
