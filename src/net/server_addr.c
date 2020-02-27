/*
 *
 * implements server_addr
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for strdup */
#include "server_addr.h"

server_addr_t *new_server_addr(const int af, const uint16_t port, const char *ipstr)
{
        server_addr_t *serveraddr = NULL;

	if (!ipstr)
		return serveraddr;
	else 
		serveraddr = (server_addr_t *)calloc(1, sizeof(server_addr_t));

        if (serveraddr) {
                if (AF_INET == af) {
                        serveraddr->af  	= AF_INET;
                        serveraddr->port  	= htons(port);
			serveraddr->ipstr	= strdup(ipstr);
			if (!serveraddr->ipstr)
				destroy_server_addr(&serveraddr);
/*              } else if (AF_INET6 == af) {
                        client->clientaddr6.sin6_family         = AF_INET6;
                        client->clientaddr6.sin6_port   = htons(port);
                        inet_pton(AF_INET6, ipstr,
                                &client->clientaddr6.sin6_addr);
*/
                } else {
                        fprintf(stdout, "%s: unsupported address family %d\n",
                                 __func__,
                                af);
			destroy_server_addr(&serveraddr);
                }
        }

        return serveraddr;
}

void destroy_server_addr(server_addr_t **ppserveraddr)
{
        if (ppserveraddr && *ppserveraddr) {
		if ((*ppserveraddr)->ipstr) {
			free((*ppserveraddr)->ipstr);
		}
		
                free(*ppserveraddr);
                *ppserveraddr = NULL;
        }
}
