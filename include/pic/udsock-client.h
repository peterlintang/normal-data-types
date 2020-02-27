
/*
 * filename: 	udsock-client.h
 * description:	implement the unix domain's client
 * author: 		john lin
 * date:		2020-02-22
 * version:		0.0.1
 *
 * interfaces:
 */

#ifndef CI_UDSOCK_CLIENT_H__
#define CI_UDSOCK_CLIENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include "module.h"

#define T UnixSockClient_T
typedef struct T *T;

/* 
 * TODO
 * unix sock server implement need to access @conn_sock, 
 * so put the define in header file
 * */
struct T {
	int conn_sock;
	char	*path;
};

/*
 * name: MODULE_FUN_NAME(UnixSockClient, create)
 * description:	allocate a new unix socket client
 * return value: pointer to unix socket client
 * 			else NULL error
 * args: @pathname: unix socket path
 */
T MODULE_FUN_NAME(UnixSockClient, create)(const char *pathname);

/*
 * name: MODULE_FUN_NAME(UnixSockClient, destroy)
 * description:	destroy a unix socket client
 * return value: return void
 * args: @clientpp: pointer's pointer to unix socke client
 */
void MODULE_FUN_NAME(UnixSockClient, destroy)(T *clientpp);

/*
 * name: MODULE_FUN_NAME(UnixSockClient, connect)
 * description:	connect to unix sock server
 * return value: -3: arg error
 * 				 -2: create socket failed
 * 				 -1: connect to server failed
 * 				 0: success
 * args: @client: pointer to unix socke client
 */
int MODULE_FUN_NAME(UnixSockClient, connect)(T client);

/*
 * name: MODULE_FUN_NAME(UnixSockClient, recv)
 * description:	recv data from unix sock server
 * return value: -2: arg error
 * 				 -1: receive data error
 * 				 -3: received data length is greater than len
 * 				 >0: total received data
 * args: @client: pointer to unix socke client
 * 		 @buf: to store data
 * 		 @len: length of @buf
 */
int MODULE_FUN_NAME(UnixSockClient, recv)(T client, char *buf, unsigned int len);

/*
 * name: MODULE_FUN_NAME(UnixSockClient, send)
 * description:	send data to unix sock server
 * return value: -2: args error
 * 				 -1: send data error
 * 				 >0: total send data
 * args: @client: pointer to unix socke client
 * 		 @buf: to store data
 * 		 @len: length of @buf
 */
int MODULE_FUN_NAME(UnixSockClient, send)(T client, char *buf, unsigned int len);

#undef T

#endif	// CI_UDSOCK_CLIENT_H__

