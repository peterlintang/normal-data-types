

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "ring_buf.h"


struct ring_buf_t {
	unsigned char *buf;
	unsigned int buf_max;
	unsigned int read_index;
	unsigned int write_index;
};


static int ring_buf_print(struct ring_buf_t *r, int len, char *s)
{
#if TEST_RING
	fprintf(stdout, "buf: %p, max: %d, read: %d, write: %d, %s, len: %d\n",
			r->buf, r->buf_max, r->read_index, r->write_index,
			s, len);
#endif
	return 0;
}


struct ring_buf_t *ring_buf_create(unsigned int num)
{
	assert(num > 0);

	struct ring_buf_t *r = NULL;

	r = (struct ring_buf_t *)calloc(1, sizeof(*r) + num);
	if (r)
	{
		r->buf_max = num;
		r->read_index = 0;
		r->write_index = 0;
		r->buf = ((unsigned char *)r) + sizeof(*r);
	}

	return r;
}

int ring_buf_destroy(struct ring_buf_t **ppr)
{
	assert(ppr && *ppr);

	free(*ppr);
	*ppr = NULL;
	return 0;
}


int ring_buf_is_empty(struct ring_buf_t *r)
{
	assert(r);

	if (r->read_index == r->write_index)
		return 1;
	return 0;
}

int ring_buf_is_full(struct ring_buf_t *r)
{
	assert(r);

	if (((r->write_index + 1) % r->buf_max) == r->read_index)
		return 1;
	return 0;
}

int ring_buf_left_space(struct ring_buf_t *r)
{
	assert(r);

	if (r->write_index >= r->read_index)
		return (r->buf_max - (r->write_index - r->read_index) - 1);
	else
		return r->read_index - r->write_index - 1;
}

int ring_buf_available_data(struct ring_buf_t *r)
{
	assert(r);

	if (r->write_index >= r->read_index)
	{
		return r->write_index - r->read_index;
	}
	else
	{
		return r->buf_max - (r->read_index - r->write_index);
	}
}

int ring_buf_put(struct ring_buf_t *r, unsigned char *data, unsigned int len)
{
	int ret = 0;
	int left = 0;

	assert(r != NULL);
	assert(data != NULL);
	assert(len > 0);

	if (ring_buf_is_full(r))
	{
		return -1;
	}

	ring_buf_print(r, len, "put b");

	left = ring_buf_left_space(r);
	len = (left >= len ? len : left);
	ret = len;

	if (r->write_index + len > r->buf_max)
	{
		memcpy(r->buf + r->write_index, data, r->buf_max - r->write_index);
		len = len - (r->buf_max - r->write_index);

		memcpy(r->buf, data + (r->buf_max - r->write_index), len);
		r->write_index = len;
	}
	else
	{
		memcpy(r->buf + r->write_index, data, len);
		r->write_index = (r->write_index + len) % r->buf_max;
	}

	ring_buf_print(r, len, "put a");

	return ret;
}

int ring_buf_get(struct ring_buf_t *r, unsigned char *data, unsigned int len)
{
	int ret = 0;
	int available = 0;

	assert(r != NULL);
	assert(data != NULL);
	assert(len > 0);

	if (ring_buf_is_empty(r))
	{
		return -1;
	}

	ring_buf_print(r, len, "get b");

	available = ring_buf_available_data(r);
	len = available >= len ? len : available;
	ret = len;

	if (r->read_index + len > r->buf_max)
	{
		memcpy(data, r->buf + r->read_index, r->buf_max - r->read_index);
		len = len - (r->buf_max - r->read_index);

		memcpy(data + r->buf_max - r->read_index, r->buf, len);
		r->read_index = len;
	}
	else
	{
		memcpy(data, r->buf + r->read_index, len);
		r->read_index = (r->read_index + len) % r->buf_max;
	}

	ring_buf_print(r, len, "get a");

	return ret;
}


