
#ifndef __net_route_H__
#define __net_route_H__


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "module.h"
#include "dlist.h"

#define T_L ListD_T

#define T RouteEntry_T
typedef struct T *T;

struct T {

    char iface[10];
    in_addr_t src;
    in_addr_t dst;
    in_addr_t mask;
    in_addr_t gateway;

};

int MODULE_FUN_NAME(Route, addRoute)(T entry);
int MODULE_FUN_NAME(Route, delRoute)(T entry);
int MODULE_FUN_NAME(Route, addHostRoute)(in_addr_t dst, in_addr_t gw, char *iface);
int MODULE_FUN_NAME(Route, delHostRoute)(in_addr_t dst, in_addr_t gw, char *iface);
in_addr_t MODULE_FUN_NAME(Route, getIfIP)(char *iface);
in_addr_t MODULE_FUN_NAME(Route, getIfGW)(char *iface);
T_L MODULE_FUN_NAME(Route, cacheLookup)(T entry);

#undef T
#undef T_L

#endif

