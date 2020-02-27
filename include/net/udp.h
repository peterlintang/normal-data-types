/*
 * this implenents the udp interfaces, which depends on socket
 * 
 *
 */

#ifndef __udp_H__
#define __udp_H__

#include <netinet/in.h>
#include "socket.h"

typedef struct {
	socket_t *sock;
	char 			*dest_host_name;
	struct sockaddr_in	 dest_host_addr;
	uint16_t		 dest_host_port;

	char 			*src_host_name;
	struct sockaddr_in	 src_host_addr;
	uint16_t		 src_host_port;

	unsigned char 		 retry_counter;

} udp_t;


void _destroy_udp(udp_t **);
upp_t *_connect_udp(const char *host, uint16_t port);
upp_t *_bconnect_udp(const char *host, uint16_t port);
socket_t *_listen_udp(uint16_t port);
uint32_t _read_from_udp(udp_t *, char **buf, uint32_t size);
uint32_t _write_to_udp(udp_t *, char *buf, uint32_t size);

#endif
