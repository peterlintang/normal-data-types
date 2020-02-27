/*
 *
 * client class head file
 *
 */

#ifndef __client_H__
#define __client_H__

#include "server_addr.h"
#include "client_addr.h"

#define MAX_CMD_LENGTH	1024
#define MAX_SEND_BUF	8192
#define MAX_RECV_BUF	8192

typedef struct __client_t {

	server_addr_t 		*serveraddr;
	client_addr_t 		*clientaddr;
	struct __client_t	*next;

	int	connectfd;

} client_t;

/* interfaces for client */
client_t *new_client(const int af, const uint16_t port, const char *ipstr);
//client *new_client(int af, uint16_t port, char *ipstr);
void destroy_client(client_t **ppclient);
int connect_to_server(client_t *client);	/* return value 1 true -1 error 0 fail */
ssize_t send_mesg_to_server_tcp(client_t *client, char *mesg, size_t count);
ssize_t recv_mesg_from_server_tcp(client_t *client, char *mesg, size_t count);
client_t *next_client(client_t *client);

client_addr_t *get_client_addr(client_t *client);
client_addr_t *set_client_addr(client_t *client, struct sockaddr *sockaddr);

int get_client_connectfd(client_t *client);
void show_client_info(client_t *client, int detail);
int execute_cmd(client_t *client);

#endif
