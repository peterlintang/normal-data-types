

#ifndef __tcp_H__
#define __tcp_H__


#include <netinet/in.h>

#include "module.h"
#include "socket.h"

#define T_S Socket_T
#define T Tcp_T
typedef struct T *T;

struct T {
	T_S sock;

    int records;
    
	char              *destHostName;
	struct sockaddr_in destHostAddr;
	uint16_t           destHostPort;

	char              *srcHostName;
	struct sockaddr_in srcHostAddr;
	uint16_t           srcHostPort;
	
	unsigned char retryCounter;
    
};

#define TCP_CONNECT_TIMEOUT   10 /* wait 10 seconds */
#define TCP_CONNECT_MAX_RETRY 3
#define TCP_ACCEPT_MAX_RETRY  3

void MODULE_FUN_NAME(Tcp, destroy)(T *);
T MODULE_FUN_NAME(Tcp, connect)(const char *host, u_int16_t port);
T MODULE_FUN_NAME(Tcp, connect2)(const char *host, u_int16_t port, const char *ifname);
T_S MODULE_FUN_NAME(Tcp, listen)(u_int16_t port);
T MODULE_FUN_NAME(Tcp, accept)(T_S);

void  MODULE_FUN_NAME(Tcp, useRecords)(T , int state);
int MODULE_FUN_NAME(Tcp, setTimeout)(T , int32_t seconds);

uint32_t MODULE_FUN_NAME(Tcp, read)(T , char **buf, uint32_t size);
uint32_t MODULE_FUN_NAME(Tcp, write)(T , char  *buf, uint32_t size);


#undef T_S
#undef T

#endif


