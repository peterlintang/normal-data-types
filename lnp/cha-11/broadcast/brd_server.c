
/*
 * test ip braodcast 
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MCAST_PORT	11111

#define  IP_FOUND "IP_FOUND"
#define  IP_FOUND_ACK "IP_FOUND_ACK"

#define BUFFER_LEN	32


void HandleIPFound(void *arg)
{
	int ret = -1;
	int sock = -1;
	struct sockaddr_in local_addr;
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

	memset((void *)&local_addr, 0, sizeof(struct sockaddr_in));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(MCAST_PORT);

	ret = bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if (ret != 0)
	{
		fprintf(stderr, "bind failed: %d\n", ret);
		return ;
	}

	while (1)
	{
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
					if (count > 0 && strstr(buf, IP_FOUND))
					{
						memcpy(buf, IP_FOUND_ACK, strlen(IP_FOUND_ACK) + 1);
						count = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&from_addr, from_len);
					}
				}
				break;
		}
	}

	return ;
}


int main(int argc, char *argv[])
{
	HandleIPFound(NULL);
	return 0;
}



