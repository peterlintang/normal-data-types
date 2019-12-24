
#ifndef IC_ARENA_INCLUDED
#define IC_ARENA_INCLUDED

#include "except.h"

#define T Arena_T
typedef struct T *T;


// < functions 91>
extern T Arena_new(void);
extern void Arena_dispose(T *ap);

extern void *Arena_alloc(T arena, long nbytes, const char *file, int line);
extern void *Arena_calloc(T arena, long count, long nbytes, 
				const char *file, int line);
extern void Arena_free(T arena);

#undef T

#endif
