/*
 * this implements the resolve and host port interfaces 
 *
 */

#include "resolve.h"

in_addr_t _name2ip_resolve(const char *h)
{
	struct addrinfo *res;
	in_addr_t ip;
	if (h) {
		if ((ip = inet_addr(h)) != INADDR_NONE)
			return ip;

		if (getaddrinfo(h, NULL, NULL, &res) == 0) {
			struct sockaddr_in *sa = \
				(struct sockaddr_in *) res->ai_addr;
			ip = sa->sin_addr.s_addr;
			freeaddrinfo(res);
		} else {
			return INADDR_NONE;
		}
	} else {
		ip = INADDR_NONE;
	}
	return ip;
}

char *_ip2string(in_addr_t ip)
{
	struct in_addr addr = {
		.s_addr = ip
	};
	char *ipstr = NULL;

	ipstr = inet_ntoa(addr);
	if (ipstr) ipstr = strdup(ipstr);
	return ipstr;
}

host_port_t *_new_host_port_pair(const char *addr)
{
	if (!addr) return NULL;
	list_
}





























