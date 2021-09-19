
#ifndef BIT_H
#define BIT_H

struct bit_t;



struct bit_t *bit_create(unsigned int num);
int bit_destroy(struct bit_t **bits);
int bit_set(struct bit_t *bits, unsigned int index);
int bit_clear(struct bit_t *bits, unsigned int index);
int bit_get(struct bit_t *bits, unsigned int index);


#endif
