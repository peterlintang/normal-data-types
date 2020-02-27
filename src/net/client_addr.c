

#include <stdio.h>			/* for stdout */
#include <stdlib.h>			/* for calloc.... */
#include <string.h>
#include "client_addr.h"

client_addr_t *new_client_addr(
		const int af, 
		const uint16_t port, 
		const char *ipstr)
{
	client_addr_t *clientaddr = (client_addr_t *)calloc(1, sizeof(client_addr_t));
	if (clientaddr) {
		if (AF_INET == af) {
			clientaddr->af 		= AF_INET;
			clientaddr->port 	= htons(port);
			clientaddr->ipstr	= strdup(ipstr);
			if (!clientaddr->ipstr)
				destroy_client_addr(&clientaddr);
/*		} else if (AF_INET6 == af) {
*/
		} else {
			fprintf(stdout, "%s: unsupported address family %d\n",
				 __func__, 
				af);
			destroy_client_addr(&clientaddr);
		}
	}
	return clientaddr;
}

void destroy_client_addr(client_addr_t **ppclientaddr)
{
	if (ppclientaddr && *ppclientaddr) {
		if ((*ppclientaddr)->ipstr) 
			free((*ppclientaddr)->ipstr);
		free(*ppclientaddr);
		*ppclientaddr = NULL;
	}
}
