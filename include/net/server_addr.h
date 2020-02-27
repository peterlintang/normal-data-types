/*
 * for server address head file
 *
 */

#ifndef __server_addr_H__
#define __server_addr_H__

#include <netinet/in.h>

typedef struct __server_addr_t {

	char		*ipstr;
	uint16_t	port;
	int		af;
//	struct sockaddr_in6	serveraddr6;		/* for now we only consider inet 4 */
//	struct sockaddr_in	addr;
//	struct __server_addr 	*next;

//	client_addr		*client_addrs;

//	int			listen_fd;

} server_addr_t;


/* interfaces */
server_addr_t *new_server_addr(const int af, const uint16_t port, const char *ipstr);
void destroy_server_addr(server_addr_t **ppserveraddr);
/*
typedef struct __server_addr_ops {
	int (*setip)(struct server_addr *, const char *);
	char *(*getip)(struct server_addr *);
	int (*setport)(struct server_addr *, const int );
	int (*getport)(struct server_addr *);
	int (*setaf)(struct server_addr *, int );
	int (*getaf)(struct server_addr *);
} server_addr_ops;
*/

#endif
