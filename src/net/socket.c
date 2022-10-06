
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <netinet/in.h> /* for htons() */

#include <net/if_arp.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/netlink.h>
#include <linux/if_ether.h>

#include <stdio.h>
#include <stdlib.h>

#include "resolve.h"
#include "socket.h"

#define T_S Socket_T

/*//////// API ROUTINES //////////////////////////////////////////*/
T_S MODULE_FUN_NAME(Socket, new)(enum _socketTypes type)
{
    T_S sock = (T_S) calloc(1, sizeof (*socket));
    if (sock) 
	{
      //  fprintf(stdout, "accessing socket type [%s]", _socketDescr[type]);
        sock->type = type;
        switch (type) 
		{
            case SOCKET_DGRAM:
                sock->skd = socket(AF_INET, SOCK_DGRAM, 0);
                break;

            case SOCKET_NETLINK:
                sock->skd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
                break;

            case SOCKET_RAW:
                sock->skd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
                break;

            case SOCKET_STREAM:
                sock->skd = socket(AF_INET, SOCK_STREAM, 0);
                break;

            case SOCKET_DUMMY:
                break;

            case SOCKET_UNIX:
                sock->skd = socket(AF_UNIX, SOCK_STREAM, 0);
                break;

            default:
                sock->skd = -1;
                break;
        }

        if (sock->skd == -1) {
            fprintf(stderr, "unable to create socket type [%d]", type);
            free(sock);
            sock = NULL;
        }
    }

    return sock;
}

void MODULE_FUN_NAME(Socket, destroy)(T_S *sock)
{
    if (sock && *sock) 
	{
        if ((*sock)->skd > 0)
            close((*sock)->skd);

        free(*sock);
        *sock = NULL;
    }
}

int MODULE_FUN_NAME(Socket, bindInterface)(T_S sock, const char *ifname)
{
    if (sock && ifname) 
	{
        struct ifreq req;

        struct sockaddr_ll skaddr;
        // set the interface
        memset(&req, 0, sizeof (struct ifreq));
        strncpy(req.ifr_name, ifname, IFNAMSIZ);
        req.ifr_name[IFNAMSIZ - 1] = '\0';
        // verify that the interface exists
        if (ioctl(sock->skd, SIOCGIFINDEX, &req) >= 0) 
		{
            // bind to the interface
            memset(&skaddr, 0, sizeof (skaddr));
            skaddr.sll_protocol = htons(ETH_P_ALL);
            skaddr.sll_ifindex = req.ifr_ifindex;
            skaddr.sll_family = AF_PACKET;

            if (bind(sock->skd, (struct sockaddr *) &skaddr, sizeof (struct sockaddr_ll)) >= 0)
			{
                return 1;
			}
            else 
			{
                fprintf(stderr, "unable to bind socket to %s:%s", ifname, strerror(errno));
            }
        } 
		else 
		{
            fprintf(stderr, "interface %s does not exist.", ifname);
        }
    }
    return 0;
}

int MODULE_FUN_NAME(Socket, bind)(T_S sock, struct sockaddr* addr)
{
    if (sock && addr) 
	{
        if (bind(sock->skd, (struct sockaddr *) addr, sizeof (struct sockaddr)) >= 0) 
		{
            return 1;
        }
        else 
		{
            fprintf(stderr, "unable to bind socket: %s", strerror(errno));
        }
    }

    return 0;
}

int MODULE_FUN_NAME(Socket, setOpt)(T_S sock, int opt)
{
    int32_t val = 0;
    int res = 0;

    if (sock) 
	{
        switch (opt) 
		{
            case BCAST:
//                fprintf(stdout, "enabling BROADCAST on socket #%u", sock->skd);
                val = 1;
                res = setsockopt(sock->skd, SOL_SOCKET, SO_BROADCAST, &val, sizeof (val));
                break;
        }

        if (res == 0) return 1;

        fprintf(stdout, "setsockopt() failed: %s", strerror(errno));
    }
    return 0;
}

int MODULE_FUN_NAME(Socket, setFlag)(T_S sock, const char *ifname, short flag)
{
    if (sock) 
	{
        struct ifreq ifr;
        strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';
        if (ioctl(sock->skd, SIOCGIFFLAGS, &ifr) >= 0) 
		{
            ifr.ifr_flags |= flag;
            if (ioctl(sock->skd, SIOCSIFFLAGS, &ifr) >= 0)
                return 1;
        }
    }
    return 0;
}

int MODULE_FUN_NAME(Socket, clearFlag)(T_S sock, const char *ifname, short flag)
{
    if (sock) 
	{
        struct ifreq ifr;
        strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';
        if (ioctl(sock->skd, SIOCGIFFLAGS, &ifr) >= 0) 
		{
            ifr.ifr_flags &= ~flag;
            if (ioctl(sock->skd, SIOCSIFFLAGS, &ifr) >= 0)
                return 1;
        }
    }
    return 0;
}

int MODULE_FUN_NAME(Socket, verifyArpType)(T_S sock, const char *ifname, u_int16_t type)
{
    if (sock) 
	{
        struct ifreq ifr;
        strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = '\0';
        if (ioctl(sock->skd, SIOCGIFHWADDR, &ifr) >= 0) 
		{

            if (ifr.ifr_hwaddr.sa_family == type)
                return 1;
        }
        fprintf(stderr, "verifyArpType: %s", strerror(errno));
    }
    return 0;
}

int MODULE_FUN_NAME(Socket, setAddress)(T_S socket, const char *to, int port)
{
    if (socket && to) 
	{
        switch (socket->type) 
		{
            case SOCKET_DGRAM:
            case SOCKET_STREAM:
            {
                in_addr_t ip = MODULE_FUN_NAME(Resolve, name2ip)(to);
                if (ip != INADDR_NONE) 
				{
                    socket->addr.in.sin_family = AF_INET;
                    socket->addr.in.sin_addr.s_addr = ip;
                    socket->addr.in.sin_port = htons(port);
                    socket->len = sizeof (socket->addr.in);
                    return 1;
                } 
				else 
				{
                    fprintf(stderr, "unable to resolve %s for setting socket address!", to);
                }
                break;
            }
            case SOCKET_UNIX:
            {
                memset((char *) &socket->addr.un, 0, sizeof (socket->addr.un));
                socket->addr.un.sun_family = AF_UNIX;
                strncpy(socket->addr.un.sun_path, to, sizeof (socket->addr.un));
                socket->len = sizeof (socket->addr.un);
                return 1;
            }
            case SOCKET_DUMMY:
            case SOCKET_NETLINK:
            case SOCKET_RAW:
            default:
                fprintf(stdout, "cannot set address for '%s' socket!", _socketDescr[socket->type]);
        }
    }
    return 0;
}

#undef T_S

/*//////// SocketPair Interface Implementation //////////////////////////////////////////*/

#define T_P SocketPair_T

T_P MODULE_FUN_NAME(SocketPair, new)(void)
{
    T_P pair = (T_P) calloc(1, sizeof (*pair));
    if (pair) 
	{
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, pair->fds) < 0) 
		{
            fprintf(stderr, "failed to create a new socket pair!");
            free(pair);
            pair = NULL;
        }
    }
    return pair;
}

void MODULE_FUN_NAME(socketPair, close)(T_P pair)
{
    if (pair) 
	{
        /* do a soft close until it is explicitly destroyed! */
//        fprintf(stdout, "a soft close of %d and %d", pair->fds[0], pair->fds[1]);
        shutdown(pair->fds[0], SHUT_RDWR);
        shutdown(pair->fds[1], SHUT_RDWR);
    }
}

void MODULE_FUN_NAME(socketPair, destroy)(T_P *ppair)
{
    if (ppair) 
	{
        T_P pair = *ppair;
        if (pair) 
		{
//            fprintf(stdout, "a full close of %d and %d", pair->fds[0], pair->fds[1]);
            close(pair->fds[0]);
            close(pair->fds[1]);
            free(pair);
            *ppair = NULL;
        }
    }
}


#undef T_P

    
