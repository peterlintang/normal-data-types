#include "apue.h"
#include <sys/socket.h>

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
