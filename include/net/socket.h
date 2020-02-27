/*
 * this will implements the socket 
 * interfaces, 
 * wow, first time to code network program
 * socket is the basic concepts which will 
 * be used by udp and tcp concepts
 */
#ifndef __socket_H__
#define __socket_H__

#include <netinet/in.h>
#include <sys/un.h>

#ifndef IFNAMESIZE
# define IFNAMESIZE 16
#endif

enum _socket_types { SOCKET_DUMMY, SOCKET_DGRAM, \
			SOCKET_NETLINK, SOCKET_RAW,\
			SOCKET_STREAM, SOCKET_UNIX };
const char *_socket_descr[] = { "DUMMY", "DGRAM", "NETLINK", \
				"RAW", "STREAM", "UNIX" };

enum _opt_types { BCAST };

typedef struct {
	int32_t skd;
	enum _socket_types type;
	unsigned char flags;

	socklen_t len;
	union {
		struct sockaddr_in in;
		struct sockaddr_un un;
	} addr;

} socket_t;


/* the socket interfaces */
socket_t *_new_socket(enum _socket_typse type);
void _destroy_socket(socket_t **socket_ptr);
int _bind_interface_socket(socket_t *sock, const char *ifname);
int _bind_socket(socket_t *socket, struct sockaddr *addr);
int _set_opt_socket(socket_t *socket, int opt);
int _set_flag_socket(socket_t *socket, const char *ifname, short flag);
int _clear_flag_socket(socket_t *socket, const char *ifname, short flag);
/* in futrue we will implement these two */
//int _verify_arptype_socket(socket_t *sock, const char *ifname, uint16_t type);
//int _set_address_socket(socket_t *sock, const char *to, int port);


typedef struct {
	int 	fds[2];
} socket_pair_t;

socket_pair_t *_new_socket_pair(void);
void _close_socket_pair(socket_pair_t *socket_pair);
void _destroy_socket_pair(socket_pair_t **socket_pair_ptr);

#endif
