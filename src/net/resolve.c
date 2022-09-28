
/*
 * 实现从ip到名字或者名字到ip的转换
 */

#include <netdb.h>              /* for hostent and gethostbyname */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resolve-2.h"

#define T HostPort_T

in_addr_t MODULE_FUN_NAME(Resolve, name2ip)(const char *h)
{
    struct addrinfo hints, *res;
    in_addr_t ip;

    if (h) 
	{
        if ((ip = inet_addr (h)) != INADDR_NONE)
            return ip;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        if (getaddrinfo (h, NULL, &hints, &res) == 0) {
            struct sockaddr_in *sa = (struct sockaddr_in *) res->ai_addr;
            ip = sa->sin_addr.s_addr;
            freeaddrinfo (res);
        } else
            return INADDR_NONE;
    } else {
        ip = INADDR_NONE;
    }
    return ip;
}

/*
 * user should release the return mem
 */
char *MODULE_FUN_NAME(Resolve, ip2string)(in_addr_t ip)
{
    struct in_addr addr = {
        .s_addr = ip
    };
    char *ipstr = NULL;
//    MODULE_LOCK ();
    ipstr = inet_ntoa (addr);
    if (ipstr) ipstr = strdup (ipstr);
//    MODULE_UNLOCK ();
    return ipstr;
}


/*
 * interface of host port
 */
T MODULE_FUN_NAME(HostPort, new)(const char *addr)
{
	char *p = NULL;
	char *oldp = NULL;
	char *host = NULL;
	char *port = NULL;

    if (addr) 
	{
		if ((oldp = strsep(&p, ":")) != NULL)
		{
			host = oldp;
		}
		else
		{
			fprintf(stderr, "%s: host format: %s error, "
					"should be: ip_address:port\n", __func__, addr);
			return NULL;
		}

		if ((oldp = strsep(&p, ":")) != NULL)
		{
			port = oldp;
		}
		else
		{
			fprintf(stderr, "%s: port format: %s error, "
					"should be: ip_address:port\n", __func__, addr);
			return NULL;
		}

		if (p != NULL)
		{
			fprintf(stderr, "%s: format: %s error, "
					"should be: ip_address:port\n", __func__, addr);
			return NULL;
		}

		if (host && port) 
		{
			T target = (T)calloc (1, sizeof(*target));
			if (target) 
			{
				target->host = strdup(host);
				target->port = atoi(port);
				return target;
			}
		}
    }
    return NULL;
}

void MODULE_FUN_NAME(HostPort, destroy)(T *ptr)
{
    if (ptr) 
	{
        T hp = *ptr;

        if (hp->host) 
			free (hp->host);
        free (hp);
        *ptr = NULL;
    }
}





