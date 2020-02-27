/*
 * this implements teh resolve and host port interfaces
 *
 */
#ifndef __resolve_H__
#define __resolve_H__

#include <netinet/in.h>
#include <arpa/inet.h>

/* resolve interfaces */
in_addr_t _name2ip_resolve(const char *h);
char *_ip2string_resolve(in_addr_t ip);

typedef struct {
	char 	*host;
	int	port;
} host_port_t;

/* host port interfaces */
host_port_t *_new_host_port_pair(const char *);
void _destroy_host_port_pair(host_port_t **);

#endif
