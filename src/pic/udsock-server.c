
/*
 * filename: 	udsock-server.c
 * description:	implement the unix domain's server
 * author: 		john lin
 * date:		2020-02-22
 * version:		0.0.1
 *
 * interfaces:
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "module.h"
#include "udsock-client.h"
#include "udsock-server.h"

#define CLIENT UnixSockClient_T

#define T UnixSockServer_T

struct T {
	int	listen_sock;
	int	backlog;
	char	*path;
};

/*
 * name: MODULE_FUN_NAME(UnixSockServer, create)
 * description:	allocate a new unix socket server
 * return value: pointer to unix socket server
 * 			else NULL error
 * args: @pathname: unix socket path
 * 		 @backlog: how many clients can connect
 */
T MODULE_FUN_NAME(UnixSockServer, create)(const char *pathname, int backlog)
{
	struct sockaddr_un addr;
	T us = NULL;

	assert(pathname != NULL);
	assert(backlog >= 1);
	
	if (!(us = calloc(1, sizeof(*us)))) {
		return NULL;
	}

	if ((us->listen_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		free(us);
		return NULL;
	}
	us->path 	= strdup(pathname);
	us->backlog 	= backlog;

	memset(&addr, 0, sizeof(addr));
	addr.sun_family	= AF_UNIX;
	sprintf(addr.sun_path, "%s", us->path);
	if ((bind(us->listen_sock, (const struct sockaddr *)&addr, 
		sizeof(addr)) < 0) || (listen(us->listen_sock, us->backlog))) {
		close(us->listen_sock);
		free(us->path);
		free(us);
		return NULL;
	}

	return us;
}

/*
 * name: MODULE_FUN_NAME(UnixSockServer, destroy)
 * description:	destroy unix socket server
 * return value: return void
 * args: @serverpp: pointer's pointer to unix socket server
 */
void MODULE_FUN_NAME(UnixSockServer, destroy)(T *uspp)
{
	T us = NULL;

	assert(uspp != NULL && (*uspp) != NULL);

	if (uspp && (us = *uspp)) {
		if (us->listen_sock != -1)
			close(us->listen_sock);
		if (access(us->path, F_OK) == 0) 
			unlink(us->path);	// remove the file
		free(us->path);
		free(us);
		*uspp = NULL;
	}
}

/*
 * name: MODULE_FUN_NAME(UnixSockServer, accept)
 * description:	accept new client connectting to server
 * return value: pointer to new client
 * args: @serverpp:  pointer to unix socket server
 */
CLIENT MODULE_FUN_NAME(UnixSockServer, accept)(T server)
{
	CLIENT client = NULL;

	assert(server != NULL);

	if (!(client = MODULE_FUN_NAME(UnixSockClient, create)(server->path))) {
		return NULL;
	}

	if ((client->conn_sock = accept(server->listen_sock, 
					NULL, NULL)) == -1) {
		perror("accept");
		MODULE_FUN_NAME(UnixSockClient, destroy)(&client);
		return NULL;
	}

	return client;
}


/*
 * name: MODULE_FUN_NAME(UnixSockServer, send)
 * description:	send data to client
 * return value: -2: arg error
 * 				 -1: send data error
 * 				 >0: total data sent
 * args: @client:  pointer to unix socket client
 * 		 @buf:	data buf
 * 		 @len: data length
 */
int MODULE_FUN_NAME(UnixSockServer, send)(CLIENT client, 
				char *buf, unsigned int len)
{
	assert(client != NULL);
	assert(buf != NULL);

	return MODULE_FUN_NAME(UnixSockClient, send)(client, buf, len);
}

/*
 * name: MODULE_FUN_NAME(UnixSockServer, recv)
 * description:	recv data from client
 * return value: -2: arg error
 * 				 -1: recv data error
 * 				 -3: received data length is greater than len
 * 				 >0: total data recveived
 * args: @client:  pointer to unix socket client
 * 		 @buf:	data buf
 * 		 @len: data buf length
 */
int MODULE_FUN_NAME(UnixSockServer, recv)(CLIENT client, 
				char *buf, unsigned int len)
{
	assert(client != NULL);
	assert(buf != NULL);

	return MODULE_FUN_NAME(UnixSockClient, recv)(client, buf, len);
}




