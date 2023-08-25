

#ifndef __net_socket_H__
#define __net_socket_H__


#include <netinet/in.h>
#include <sys/un.h>
#include <stdio.h>

#include "module.h"


#ifndef IFNAMSIZ
# define IFNAMSIZ 16
#endif

enum _socketTypes {  SOCKET_DUMMY, SOCKET_DGRAM, SOCKET_NETLINK, SOCKET_RAW, SOCKET_STREAM, SOCKET_UNIX };
const char *_socketDescr[] = { "DUMMY", "DATAGRAM", "NETLINK", "RAW", "STREAM", "UNIX" };

enum _optTypes { BCAST };

#define T_S Socket_T
typedef struct T_S *T_S;

struct T_S {
	int32_t skd;
	enum _socketTypes type;
	unsigned char flags;
#define SOCKET_LISTEN_FLAG  0x1
#define SOCKET_ACCEPT_FLAG  0x2
#define SOCKET_CONNECT_FLAG 0x4
    socklen_t len;
    union {
        struct sockaddr_in in;
        struct sockaddr_un un;
    } addr;
};


T_S MODULE_FUN_NAME(Socket, new)(enum _socketTypes type);
void       MODULE_FUN_NAME(Socket, destroy)(T_S *psocket);
int MODULE_FUN_NAME(Socket, bindInterface)(T_S socket, const char *ifname);
int MODULE_FUN_NAME(Socket, bind)(T_S socket, struct sockaddr* addr);
int MODULE_FUN_NAME(Socket, getInterfaceIp)(T_S sock, const char *ifname, char *ip, int len);
int MODULE_FUN_NAME(Socket, setInterfaceIp)(T_S sock, const char *ifname, char *ip, int len);
int MODULE_FUN_NAME(Socket, setFlag)(T_S socket, const char *ifname, short flag);
int MODULE_FUN_NAME(Socket, setOpt)(T_S socket, int opt);
int MODULE_FUN_NAME(Socket, clearFlag)(T_S socket, const char *ifname, short flag);
int MODULE_FUN_NAME(Socket, verifyArpType)(T_S socket, const char *ifname, u_int16_t type);
int MODULE_FUN_NAME(Socket, setAddress)(T_S socket, const char *to, int port);

#undef T_S
    

/*
 * interface of socket pair
 */
#define T_P SocketPair_T
typedef struct T_P *T_P;
struct T_P{
    int	fds[2];
    int closed;
};

T_P MODULE_FUN_NAME(SocketPair, new)(void);
void MODULE_FUN_NAME(SocketPair, close)(T_P pair);
void MODULE_FUN_NAME(SocketPair, destroy)(T_P *ppair);
    
#undef T_P

// special section for some Endian magic functions...

inline uint32_t swapel(uint32_t a) {
    uint32_t b = 0;
    char *bp = (char *) &b;
    char *ap = (char *) &a;
    bp[0] = ap[3];
    bp[1] = ap[2];
    bp[2] = ap[1];
    bp[3] = ap[0];
    return b;
}

inline uint32_t htolel(uint32_t a) {
    uint32_t magic = 1;
    char *mp = (char *) &magic;
    if(mp[3] == 1) {     // big-endian 
        fprintf(stdout, "htolel: converting from big-endian to little-endian");
        return swapel(a);
    }
    return a;
}

inline uint32_t ltohel(uint32_t a) {
    uint32_t magic = 1;
    char *mp = (char *) &magic;
    if(mp[3] == 1) {     // big-endian
        fprintf(stdout, "htolel: converting from little-endian to big-endian");
        return swapel(a);
    }
    return a;
}

#endif


