
#ifndef BUFFER_DATA_H__
#define BUFFER_DATA_H__

#define T buffer_data_t

typedef struct T *T;

T buffer_data_create(int element_size, int total_elements);
void buffer_data_destroy(T *bp);
void *buffer_data_calloc(T b);
void buffer_data_free(T b, void *p);

#undef T

#endif // end of BUFFER_DATA_H__

