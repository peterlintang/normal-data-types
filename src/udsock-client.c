
/*
 * filename: 	udsock-client.c
 * description:	implement the unix domain's client
 * author: 		john lin
 * date:		2020-02-22
 * version:		0.0.1
 *
 * interfaces:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include "udsock-client.h"

#define T UnixSockClient_T


/*
 * name: MODULE_FUN_NAME(UnixSockClient, create)
 * description:	allocate a new unix socket client
 * return value: pointer to unix socket client
 * 			else NULL error
 * args: @pathname: unix socket path
 */
T MODULE_FUN_NAME(UnixSockClient, create)(const char *pathname)
{
	T client = NULL;

	assert(pathname != NULL);

	if (!(client = calloc(1, sizeof(*client)))) {
		return NULL;
	}

	if (!(client->path = strdup(pathname))) {
		free(client);
		return NULL;
	}

	client->conn_sock = -1;
	return client;
}

/*
 * name: MODULE_FUN_NAME(UnixSockClient, destroy)
 * description:	destroy a unix socket client
 * return value: return void
 * args: @clientpp: pointer's pointer to unix socke client
 */
void MODULE_FUN_NAME(UnixSockClient, destroy)(T *ppclient)
{
	T client = NULL;

	assert(ppclient && *ppclient);

	if (ppclient && (client = *ppclient)) {
		if (client->conn_sock != -1)
			close(client->conn_sock);
		free(client->path);
		free(client);
		*ppclient = NULL;
	}
}

/*
 * name: MODULE_FUN_NAME(UnixSockClient, connect)
 * description:	connect to unix sock server
 * return value: 
 * 				 -2: create socket failed
 * 				 -1: connect to server failed
 * 				 0: success
 * args: @client: pointer to unix socke client
 */
int MODULE_FUN_NAME(UnixSockClient, connect)(T client)
{
	struct sockaddr_un addr;

	assert(client);

	if ((client->conn_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		return -2;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	sprintf(addr.sun_path, "%s", client->path);

	if (connect(client->conn_sock, 
		(const struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(client->conn_sock);
		client->conn_sock = -1;
		return -1;
	}

	return 0;
}

/*
 * name: MODULE_FUN_NAME(UnixSockClient, send)
 * description:	send data to unix sock server
 * return value: 
 * 				 -1: send data failed
 * 				 >0: total data send
 * args: @client: pointer to unix socke client
 * 		 @buf: to store data
 * 		 @len: length of @buf
 */
int MODULE_FUN_NAME(UnixSockClient, send)(T client, char *buf, unsigned int len)
{
	int		total 		= 0;
	uint32_t	bytes_send 	= len;
	int		send_len 	= 0;

	assert(client != NULL);
	assert(buf != NULL);

	if ((total = write(client->conn_sock, 
			&bytes_send, sizeof(uint32_t))) == -1) {
		return -1;
	}

	total = 0;
	while (total < len) {
		send_len = write(client->conn_sock, buf + total, len - total);
//		if (send_len == -1) {
		if (send_len <= 0) {
			if (errno == EINTR)
//				break;
				continue;
			else {
				perror("write");
				return -1;
			}
		}

//		if (send_len == 0) break; // write return zero means what ??
					  // treat it as an error ???

		total += send_len;
	}

	return total;
}


/*
 * name: MODULE_FUN_NAME(UnixSockClient, recv)
 * description:	recv data from unix sock server
 * return value: 
 * 				 -1: recv data error
 * 				 -2: received data length is greater than len
 * 				 >0: total data received
 * args: @client: pointer to unix socke client
 * 		 @buf: to store data
 * 		 @len: length of @buf
 */
int MODULE_FUN_NAME(UnixSockClient, recv)(T client, char *buf, unsigned int len)
{
	int	 total 		= 0;
	int	 recv_len	= 0;
	uint32_t bytes_recv	= 0;

	assert(client != NULL);
	assert(buf != NULL);

	if ((recv_len = read(client->conn_sock, 
			&bytes_recv, sizeof(uint32_t))) == -1) {
		return -1;
	}

//	recv_len = ntohl(bytes_recv); // doesnot need this since on the same machine
	recv_len = bytes_recv;

	if (recv_len > len) {
		return -2;
	}

	while (total < recv_len) {
		len = read(client->conn_sock, buf + total, recv_len - total);
		if (len == -1) {
			if (errno == EINTR)
				continue;
			else {
				perror("read");
				return -1;
			}
		}
		if (len == 0) break;
		total += len;
	}

	return total;
}



