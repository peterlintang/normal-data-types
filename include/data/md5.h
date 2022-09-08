#ifndef __md5_H__
#define __md5_H__

#include <stdint.h>

typedef struct {

    uint32_t state[4];            /* state (ABCD) */
    uint32_t count[2];     /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];     /* input buffer */
    
} md5_ctx_t;

typedef unsigned char md5_t;

    



/***** MD5Transform Interface Implementation *****/

md5_ctx_t *md5_newMD5Transform (void);

void md5_destroyMD5Transform (md5_ctx_t **contextPtr);

void md5_updateMD5Transform (md5_ctx_t *context, unsigned char *input, unsigned int inputLen);

md5_t *md5_finalizeMD5Transform (md5_ctx_t *context);


/***** MD5 Interface Implementation *****/

md5_t *md5_newMD5 (void);

void md5_destroyMD5 (md5_t **digestPtr);

int md5_compareMD5 (const md5_t *a, const md5_t *b);

char *md5_toString (md5_t *digest);

#endif

