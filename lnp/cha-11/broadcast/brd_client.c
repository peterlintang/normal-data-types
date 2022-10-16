
/*
 * test ip braodcast 
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <net/if.h>
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MCAST_PORT	11111

#define IFNAME "em1"

#define  IP_FOUND "IP_FOUND"
#define  IP_FOUND_ACK "IP_FOUND_ACK"

#define BUFFER_LEN	32


void IPFound(void *arg)
{
	int ret = -1;
	int sock = -1;
	int so_broadcast = 1;
	struct ifreq ifr;
	struct sockaddr_in broadcast_addr;
	struct sockaddr_in from_addr;
	int from_len;
	int count = -1;
	fd_set readfd;
	char buf[BUFFER_LEN];
	struct timeval timeout;

	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		fprintf(stderr, "create sock failed: %d\n", sock);
		return ;
	}

	strcpy(ifr.ifr_name, IFNAME);
	if (ioctl(sock, SIOCGIFBRDADDR, &ifr) == -1)
	{
		fprintf(stderr, "ioctl error: %s\n", strerror(errno));
		return ;
	}

	memcpy((void *)&broadcast_addr, &ifr.ifr_broadaddr, sizeof(struct sockaddr_in));
	broadcast_addr.sin_port = htons(MCAST_PORT);

	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));

	int times = 10;
	int i = 0;

	for (i = 0; i < times; i++)
	{
		ret = sendto(sock, IP_FOUND, strlen(IP_FOUND), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr));
		if (ret == -1) continue;

		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);

		ret = select(sock + 1, &readfd, NULL, NULL, &timeout);
		switch (ret)
		{
			case -1:
				break;
			case 0:
				break;
			default:
				if (FD_ISSET(sock, &readfd))
				{
					count = recvfrom(sock, buf, BUFFER_LEN, 0, (struct sockaddr *)&from_addr, &from_len);
					if (count > 0 && strstr(buf, IP_FOUND_ACK))
					{
						fprintf(stdout, "found server ip : %s\n", inet_ntoa(from_addr.sin_addr));
						return ;
					}
				}
				break;
		}

	}

	return ;
}


int main(int argc, char *argv[])
{
	IPFound(NULL);
	return 0;
}



