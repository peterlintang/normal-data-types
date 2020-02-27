/*
 * for server head file 
 */
#ifndef __server_H__
#define __server_H__

#include <poll.h>			/* for struct pollfd */
#include "client.h"
#include "client_addr.h"
#include "server_addr.h"
//#include <netinet/in.h>		/* for struct sockaddr_in & 6 */

#define SERVER_NAME_LENGTH	64
#define MAXLOG			256

#define SERVER_PORT		10008
#define IP_STR			"192.168.253.103"
#define BACKLOG			124

typedef struct __server_t {

	server_addr_t 	  *serveraddr;
	struct __server_t *next;

	client_t 	  *clients;

	struct pollfd	  *pollfd;
	int		  count;

	int		listen_fd;
	int		backlog;
	
//	char		server_name[SERVER_NAME_LENGTH];
	char		*name;

} server_t;


/* interfaces */
server_t *new_server(char *name, 
		int af, 
		uint16_t port, 
		char *ipstr, 
		uint32_t backlog);

void	destroy_server(server_t **ppserver);

client_t *del_client_from_server(server_t *server, client_t *old);

client_t *add_client_to_server(server_t *server, client_t *new, client_t *after);

client_t *head_client_of_server(server_t *server);

int create_listen_socket(server_t *server);

int get_listen_socket(server_t *server);

client_t *accept_connect_from_client(server_t *server);

int set_server_name(server_t *pserver, const char *name);

char *get_server_name(server_t *pserver);

void show_clients_info(server_t *server);

client_t *search_client_by_connectfd(server_t *server, int fd);

ssize_t send_mesg_to_client_tcp(client_t *client, char *mesg, size_t count);

ssize_t recv_mesg_from_client_tcp(client_t *client, char *mesg, size_t count);

int send_cmd_to_client(FILE *in, FILE *out, client_t *client);

#endif 
