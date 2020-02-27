/*
 * this implements the tcp interfaces
 *
 *
 */

#ifndef __tcp_H__
#define __tcp_H__

#include <netinet/in.h>
#include "socket.h"

typedef struct {
	socket_t *sock;

	unsigned char records;	/* boolent_t o or 1 */

	char			*dest_host_name;
	struct sockaddr_in 	dest_host_addr;
	uint16_t		dest_host_port;

	char			*src_host_name;
	struct sockaddr_in	src_host_addr;
	uint16_t		src_host_port;

	unsigned char 	retry_counter;
} tcp_t;

#define TCP_CONNECT_TIMEOUT	10
#define TCP_CONNECT_MAX_RETRY	3
#define TCP_ACCEPT_MAX_RETRY	3

/* tcp interfaces */
void _destroy_tcp(tcp_t **tcp_ptr);
tcp_t *_connect_tcp(const char *host, uint16_t port);
tcp_t *_connect2_tcp(const char *host, uint16_t port, const char *ifname);
socket_t *_listen_tcp(uint16_t port);
tcp_t *_accept_tcp(socket_t *);

void _use_records_tcp(tcp_t *tcp, unsigned char stat); /* state : 0 or 1 */
unsigned char _set_timeout_tcp(tcp_t *tcp, int32_t seconds);

uint32_t _read_tcp(tcp_t *tcp, char **buf, uint32_t size);
uint32_t _write_tcp(tcp_t *tcp, char *buf, uint32_t size);

#endif
