

/*
 * duo po  client
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MCAST_PORT 8888
#define MCAST_ADDR	"224.0.0.88"
#define MCAST_INTERVAL	5
#define BUFF_SIZE	256


int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in local_addr;
	int ret = 0;
	int loop = 0;
	struct ip_mreq mreq;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1)
	{
		perror("socket");
		return -1;
	}

	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	local_addr.sin_port = htons(MCAST_PORT);

	ret = bind(s, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if (ret < 0)
	{
		perror("bind");
		return ret;
	}

	loop = 1;

	ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	if (ret < 0)
	{
		perror("setsockopt multicast loop");
		return ret;
	}

	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	ret = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if (ret < 0)
	{
		perror("setsockopt add ship");
		return ret;
	}

	int times = 0;
	int addr_len = 0;
	char buff[BUFF_SIZE];
	int n = 0;

	for (times = 0; times < 5; times++)
	{
		addr_len = sizeof(local_addr);
		memset(buff, 0, BUFF_SIZE);

		n = recvfrom(s, buff, BUFF_SIZE, 0, (struct sockaddr *)&local_addr, &addr_len);
		if (n == -1)
		{
			fprintf(stdout, "recvfrom: %d error\n", times);
		}

		printf("recv %d st message from server: %s\n", times, buff);
		sleep(MCAST_INTERVAL);
	}

	ret = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
	close(s);

	return 0;

}



