

#ifndef __udp_H__
#define __udp_H__


#include <netinet/in.h>

#include "socket.h"

#include "module.h"

#define T_S Socket_T
#define T Udp_T 
typedef struct T *T;

struct T {
    T_S sock;

    char              *destHostName;
    struct sockaddr_in destHostAddr;
    uint16_t           destHostPort;

    char              *srcHostName;
    struct sockaddr_in srcHostAddr;
    uint16_t           srcHostPort;
	
    unsigned char retryCounter;

};


void MODULE_FUN_NAME(Udp, destroy)(T *);
T MODULE_FUN_NAME(Udp, connect)    (const char *host, u_int16_t port);
T MODULE_FUN_NAME(Udp, bconnect)    (const char *host, u_int16_t port);
T_S MODULE_FUN_NAME(Udp, listen)     (u_int16_t port);
T_S MODULE_FUN_NAME(Udp, listen2)     (in_addr_t host, u_int16_t port);
uint32_t   MODULE_FUN_NAME(Udp, read)    (T , char **buf, uint32_t size);
uint32_t   MODULE_FUN_NAME(Udp, write)   (T , char  *buf, uint32_t size);

#undef T_S
#undef T

#endif


