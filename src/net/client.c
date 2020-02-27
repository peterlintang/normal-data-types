/*
 *
 * implements client 
 *
 */

#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>		/* for stdout */
#include <stdlib.h>		/* for calloc */
#include <string.h>		/* for memcpy */
#include "client.h"

//client *new_client(server_addr *serveraddr)
client_t *new_client(const int af, 
		const uint16_t port, 
		const char *ipstr)
{
	client_t *client = NULL;
	if (ipstr) {
		client = (client_t *) calloc(1, sizeof(client_t));
		if (client) {
			client->serveraddr = new_server_addr(af, port, ipstr);
//			client->clientaddr = calloc(1, sizeof(client_addr));
//			if (!client->serveraddr || !client->serveraddr) {
			if (!client->serveraddr) {
				destroy_client(&client);
				return NULL;
			}
			client->connectfd = -1;
		}
	}
	return client;
}

void destroy_client(client_t **ppclient)
{
	if (ppclient && *ppclient) {
		if ((*ppclient)->connectfd != -1) {
			close((*ppclient)->connectfd);
		}
		destroy_server_addr(&(*ppclient)->serveraddr);
		if ((*ppclient)->clientaddr) {
			destroy_client_addr(&(*ppclient)->clientaddr);
		}
		free(*ppclient);
		*ppclient = NULL;
	}
}

int connect_to_server_tcp(client_t *client)
{
	struct sockaddr_in	serveraddr;

	if (!client) {
		fprintf(stderr, "%s: arguments error\n", __func__);
		return -1;
	}

//	client->connectfd = socket(client->serveraddr->af, 
//				SOCK_STREAM,
//				0);

	client->connectfd = socket(AF_INET,
				SOCK_STREAM,
				0);

	if (client->connectfd < 0) {
		fprintf(stderr, "%s: socket error\n", __func__);
		perror("socket");
		return -1;
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family	= client->serveraddr->af;
	serveraddr.sin_port	= htons(client->serveraddr->port);
	inet_pton(client->serveraddr->af,
			client->serveraddr->ipstr,
			&serveraddr.sin_addr);

	if (connect(client->connectfd, 
		(struct sockaddr *) &serveraddr, 
		sizeof(serveraddr)) < 0) {
		fprintf(stderr, "%s: connect error\n", __func__);
		perror("connect");
		return -1;
	}

	return 1;
}

ssize_t send_mesg_to_server_tcp(client_t *client, char *mesg, size_t count)
{
	int	writecount;

	if (!client) {
		fprintf(stderr, "%s: arguments error\n", __func__);
		return -1;
	}

	if (!mesg) return 0;

	writecount = write(client->connectfd, mesg, count);

	return writecount;
}

ssize_t recv_mesg_from_server_tcp(client_t *client, char *mesg, size_t count)
{
	int 	readcount;

	if (!client || !mesg) {
		fprintf(stderr, "%s: arguments error\n", __func__);
		return -1;
	}

	readcount = read(client->connectfd, mesg, count);
	mesg[readcount] = '\0';

	return readcount;
}

client_t *next_client(client_t *client)
{
	if (client)
		return client->next;
	return NULL;
}

client_addr_t *get_client_addr(client_t *client)
{
	if (client) {
		return client->clientaddr;
	}
	return NULL;
}

client_addr_t *set_client_addr(client_t *client, 
		struct sockaddr *sockaddr)
{
	int		af;
	uint16_t	port;
	char		ipstr[256];
	struct sockaddr_in	*clientaddr;

	if (!client || !sockaddr)
		return NULL;

	if (sockaddr->sa_family == AF_INET) {

		clientaddr = (struct sockaddr_in *) sockaddr;
		af 	= clientaddr->sin_family;
		port	= ntohs(clientaddr->sin_port);
		inet_ntop(af, &clientaddr->sin_addr, ipstr, 200);

		if (!client->clientaddr) {
			client->clientaddr = new_client_addr(af, port, ipstr);
			return client->clientaddr;
		}
		else {
			client->clientaddr->af 	 = af;
			client->clientaddr->port = port;
			if (client->clientaddr->ipstr) free(client->clientaddr->ipstr);
			client->clientaddr->ipstr = strdup(ipstr);
			return client->clientaddr;
		}
		
	} else {
		fprintf(stderr, "%s: unsupported address family\n", __func__);
	}
	return NULL;
}

int get_client_connectfd(client_t *client)
{
	if (client) 
		return client->connectfd;
	return -1;
}

void show_client_info(client_t *client, int detail)
{
	if (!client) return ;

	if (detail) {
		fprintf(stdout, "%s: client's server info\n", __func__);
		fprintf(stdout, "ip : %s\t port : %d\n", 
				client->serveraddr->ipstr,
				client->serveraddr->port);
	}

	if (client->clientaddr) {
		fprintf(stdout, "ip : %s\t port : %d\t conn fd : %d\n", 
				client->clientaddr->ipstr,
				client->clientaddr->port,
				client->connectfd);
	}
}

/* 
 * return value -1  error
 *    		 0  end of file
 *		 1  success
 */
int execute_cmd(client_t *client)
{
	int	recvcount;
	int	sendcount;
	char	recvbuf[MAX_CMD_LENGTH];
	char	sendbuf[MAX_SEND_BUF];
	FILE	*fd;

	if (!client) return -1;

	recvcount = recv_mesg_from_server_tcp(client, recvbuf, MAX_CMD_LENGTH);
	if (recvcount == -1) {
		fprintf(stderr, "%s: recv data error\n", __func__);
		return -1;
	} else if (recvcount == 0) {
		fprintf(stdout, "end of file\n");
		return 0;
	} else {
		fd = popen(recvbuf, "r");
		recvcount = read(fileno(fd), sendbuf, MAX_CMD_LENGTH); 	/* hope we can read all the data only one time */
		if (recvcount == -1) {
			fprintf(stderr, "%s: read error\n", __func__);
			return -1;
		} else if (recvcount == 0) {
			return 0;
		} else {
			;
		}
		sendcount = send_mesg_to_server_tcp(client, sendbuf, recvcount);
	}
	return 1;
}
