
/*
 *
 * 实现环形缓冲区
 * 在num数组中，当write index + 1 == read index时，
 * 数组满， 此时数组有一个空元素，所以真正的大小式num-1
 *
 */

#ifndef RING_BUF_H
#define RING_BUF_H


struct ring_buf_t;


struct ring_buf_t *ring_buf_create(unsigned int num);

int ring_buf_destroy(struct ring_buf_t **ppr);

int ring_buf_is_empty(struct ring_buf_t *r);

int ring_buf_is_full(struct ring_buf_t *r);

int ring_buf_left_space(struct ring_buf_t *r);

int ring_buf_available_data(struct ring_buf_t *r);

int ring_buf_put(struct ring_buf_t *r, unsigned char *data, unsigned int len);

int ring_buf_get(struct ring_buf_t *r, unsigned char *data, unsigned int len);


#endif
