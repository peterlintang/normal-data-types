
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>


#include "netutil.h"

#define MAXSLEEP 128

int
connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
	int	nsec;

	/*
	 *try to connect with exponential backoff
	 */
	for (nsec = 1; nsec <= MAXSLEEP; nsec <<= 1)
	{
		if (connect(sockfd, addr, alen) == 0)
		{
			/*
			 *connection accepted
			 */
			return (0);
		}
		if (nsec <= MAXSLEEP / 2)
			sleep(nsec);
	}
	return (-1);
}



int get_ip_by_ifname(char *ifname, char *ip, int ip_len)
{
	int sockfd = -1;
	struct ifreq ifr;
	char str[64] = { 0 };

	if (ifname == NULL || ip == NULL)
	{
		return -2;
	}

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, ifname);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		return -1;
	}

	if (ioctl(sockfd, SIOCGIFADDR, &ifr) != 0)
	{
		close(sockfd);
		return -1;
	}

	close(sockfd);

	inet_ntop(AF_INET, &(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr), str, 64);
	strncpy(ip, str, ip_len);

	return 0;
}

