
/*
 * filename: 	udsock-server.h
 * description:	implement the unix domain's server
 * author: 		john lin
 * date:		2020-02-22
 * version:		0.0.1
 *
 * interfaces:
 */

#ifndef CI_UDSOCK_SERVER_H__
#define CI_UDSOCK_SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "module.h"
#include "udsock-client.h"

#define CLIENT UnixSockClient_T

#define T UnixSockServer_T
typedef struct T *T;


/*
 * name: MODULE_FUN_NAME(UnixSockServer, create)
 * description:	allocate a new unix socket server
 * return value: pointer to unix socket server
 * 			else NULL error
 * args: @pathname: unix socket path
 * 		 @backlog: how many clients can connect
 */
T MODULE_FUN_NAME(UnixSockServer, create)(const char *pathname, int backlog);

/*
 * name: MODULE_FUN_NAME(UnixSockServer, destroy)
 * description:	destroy unix socket server
 * return value: return void
 * args: @serverpp: pointer's pointer to unix socket server
 */
void MODULE_FUN_NAME(UnixSockServer, destroy)(T *serverpp);

/*
 * name: MODULE_FUN_NAME(UnixSockServer, accept)
 * description:	accept new client connectting to server
 * return value: pointer to new client
 * 			error NULL
 * args: @serverpp:  pointer to unix socket server
 */
CLIENT MODULE_FUN_NAME(UnixSockServer, accept)(T server);

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
				char *buf, unsigned int len);

/*
 * name: MODULE_FUN_NAME(UnixSockServer, recv)
 * description:	recv data from client
 * return value: -2: arg error
 * 				-1: receive data error
 * 				-3: received data length is greater than len
 * 				>0: total data received
 * args: @client:  pointer to unix socket client
 * 		 @buf:	data buf
 * 		 @len: data buf length
 */
int MODULE_FUN_NAME(UnixSockServer, recv)(CLIENT client, 
				char *buf, unsigned int len);

#undef T
#undef CLIENT

#endif 	// CI_UDSOCK_SERVER_H__


