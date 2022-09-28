
#ifndef __net_resolve_H__
#define __net_resolve_H__


#include <netinet/in.h>
#include <arpa/inet.h>			/* for inet_ntoa */

#include "module.h"

in_addr_t MODULE_FUN_NAME(Resolve, name2ip)(const char *h);
char     *MODULE_FUN_NAME(Resolve, ip2string)(in_addr_t ip);

#define T HostPort_T
typedef struct T *T;

struct T {
    char *host;
    int   port;
};

T MODULE_FUN_NAME(HostPort, new)(const char *);
void         MODULE_FUN_NAME(HostPort, destroy)(T *);
    
#undef T

#endif


