/*
 * client address head file
 *
 *
 */

#ifndef client_addr_H__
#define client_addr_H__

#include <netinet/in.h>		/* for struct sockaddr_in6 */

typedef struct __client_addr_t {

	char		*ipstr;
	uint16_t	port;
	int		af;
//	struct sockaddr_in6	clientaddr6;		/* for now we only consider inet 4 */
//	struct sockaddr_in	addr;

//	int			connectfd; 

} client_addr_t;

/* interfaces */
client_addr_t *new_client_addr(const int af, 
				const uint16_t port, 
				const char *ipstr);
void	destroy_client_addr(client_addr_t **ppclient);
/*
typedef struct __client_addr_ops {
	client_addr *(*new_client_addr)(struct sockaddr_in6 *pin6_addr);
	void (*destroy_client_addr)(client_addr *pclientaddr);
//	client_addr *(*get_next_client_addr)(const char *pip_str);
	client_addr *(*set_client_addr)(client_addr *pclientaddr, struct sockaddr_in6 *pin6_addr);
	client_addr *(*next_client_addr)(client_addr *pclientaddr);
//        int (*setip)(struct client_addr *, const char *);
//        char *(*getip)(struct client_addr *);
//        int (*setport)(struct client_addr *, const int );
//        int (*getport)(struct client_addr *);
//        int (*setaf)(struct client_addr *, int );
//        int (*getaf)(struct client_addr *);
} client_addr_ops;
*/


#endif
