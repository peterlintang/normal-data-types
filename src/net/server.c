/*
 *
 * implements server
 *
 */

#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>		/* for stderr */
#include <stdlib.h>		/* for calloc */
#include <string.h>		/* for strdup */
#include <poll.h>		/* for poll & struct poolfd */
#include "server.h"

server_t *new_server(char *name, int af, uint16_t port, char *ipstr, uint32_t backlog)
{
	server_t *server = NULL;

	if (!name || !ipstr) {
		fprintf(stderr, "%s: arguments error\n", __func__);
		return NULL;
	}

	server = (server_t *)calloc(1, sizeof(server_t));
	if (!server)
		return server;

	server->serveraddr 	= new_server_addr(af, port, ipstr);
	server->name		= strdup(name);
	if (!server->serveraddr || !server->name) {
		destroy_server_addr(&server->serveraddr);
		free(server->name);
		free(server);
		return NULL;
	}

	server->listen_fd = -1;
	server->backlog	  = (backlog == 0 ? MAXLOG : backlog);

	return server;
}

void destroy_server(server_t **ppserver)
{
	if (ppserver && *ppserver) {

		client_t *client = NULL;
		server_t *server = *ppserver;

		if (server->listen_fd > -1)
			close(server->listen_fd);

		destroy_server_addr(&server->serveraddr);

		free(server->name);

		while ((client = head_client_of_server(server)) != NULL) {
			del_client_from_server(server, client);
			destroy_client(&client);
		}

		if (server->pollfd) free(server->pollfd);	/* for new member, */
		free(server);
		*ppserver = NULL;

	}
}


client_t *del_client_from_server(server_t *server, client_t *old)
{
	client_t *client = NULL;
	client_t *found  = NULL;

	if (server && old) {

		client = head_client_of_server(server);

		if (old == client) {
			server->clients = old->next;
			return old;
		}

		found  = client;
		client = client->next;
		while (client != NULL) {
			if (client == old) break;
			found = client;
			client = client->next;
		}

		if (client == NULL) return NULL;
		found->next = old->next;

		return old;
	}

	return NULL;
}

client_t *add_client_to_server(server_t *server, client_t *new, client_t *after)
{
	if (server && new) {
		if (!after) {
			new->next = server->clients;
			server->clients = new;
		} else {
			new->next = after->next;
			after->next = new;
		}
		return new;
	}
	return NULL;
}

client_t *head_client_of_server(server_t *server)
{
	return server ? server->clients : NULL;
}

/*
 * return value 1 success 0 fail -1 error
 */
int create_listen_socket(server_t *server)
{
	struct sockaddr_in	serveraddr;

	if (!server)
		return -1;

	server->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->listen_fd < 0) {
		fprintf(stderr, "%s: socket error\n", __func__);
		perror("socket");
		return -1;
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = server->serveraddr->af;
	serveraddr.sin_port   = htons(server->serveraddr->port);
	inet_pton(server->serveraddr->af, 
			(const char *)&serveraddr.sin_addr,
			server->serveraddr->ipstr);

	if (bind(server->listen_fd, 
		(struct sockaddr *)&serveraddr, 
		sizeof(serveraddr)) < 0) {
		fprintf(stderr, "%s: bind error\n", __func__);
		perror("bind");
		return -1;
	}

	if (listen(server->listen_fd, server->backlog)) {
		fprintf(stderr, "%s: listen error\n", __func__);
		perror("listen");
		server->listen_fd = -1;
		return -1;
	}

	return 1;
}

/*
 * return value -1 error or not create yet
 * 	else return create socket
 */
int get_listen_socket(server_t *server)
{
	if (server)
		return server->listen_fd;
	return -1;
}

client_t *accept_connect_from_client(server_t *server)
{
	int			len;
	server_addr_t 		*serveraddr 	= NULL;
	client_t 		*new 		= NULL;
	struct sockaddr_in	clientaddr;

	if (!server)
		return NULL;

	serveraddr = server->serveraddr;
	if (!serveraddr) {
		fprintf(stderr, "%s: server address not set\n", __func__);
		return NULL;
	}

	new = new_client(serveraddr->af, serveraddr->port, serveraddr->ipstr);
	if (!new) {
		fprintf(stderr, "%s: create new client error\n", __func__);
		return new;
	}

	len = sizeof(clientaddr);
	memset(&clientaddr, 0, len);
	new->connectfd = accept(server->listen_fd, 
				(struct sockaddr *)&clientaddr, 
				&len);

	if (new->connectfd < 0) {
		fprintf(stderr, "%s: accept error\n", __func__);
		perror("accept");
		destroy_client(&new);
	} else {
		if (!set_client_addr(new, (struct sockaddr *)&clientaddr)) {
			fprintf(stderr, "%s: unable to set client address\n", __func__);
			destroy_client(&new);
		}
//		new->clientaddr->af 	= clientadd.sin_af;
//		new->clientaddr->port	= ntohs(clientadd.sin_port);
	}
	
	return new;
}

int set_server_name(server_t *server, const char *name)
{
	char	*tmp;
	if (server && name) {
		tmp = strdup(name);
		if (tmp) {
			if (server->name)
				free(server->name);
			server->name = tmp;
			return 1;
		}
		return -1;
	}
	return -1;
}


char *get_server_name(server_t *server)
{
	return server ? server->name : NULL;
}


void show_clients_info(server_t *server)
{
	client_t *client;

	if (!server) return ;

	client = head_client_of_server(server);
	while (client != NULL) {
		show_client_info(client, 0);
		client = client->next;
	}
}

client_t *search_client_by_connectfd(server_t *server, int fd)
{
	client_t *client = NULL;

	if (!server) return NULL;

	client = head_client_of_server(server);
	while (client != NULL) {
		if (client->connectfd == fd) break;
		client = client->next;
	}
	return client;
}

ssize_t send_mesg_to_client_tcp(client_t *client, char *mesg, size_t count)
{
	return send_mesg_to_server_tcp(client, mesg, count);
}

ssize_t recv_mesg_from_client_tcp(client_t *client, char *mesg, size_t count)
{
	return recv_mesg_from_server_tcp(client, mesg, count);
}

/* need this ?  we will consider later */
int send_cmd_to_client(FILE *in, FILE *out, client_t *client)
{
	int	recvcount;
	char	recvbuf[MAX_SEND_BUF];
	char	sendbuf[MAX_CMD_LENGTH];

	if (!client) return -1;

	in = in ? in : stdin;
	out = out ? out : stdout;

	while (fgets(sendbuf, MAX_CMD_LENGTH, in) != NULL) {
		send_mesg_to_client_tcp(client, sendbuf, strlen(sendbuf));
		recvcount = recv_mesg_from_client_tcp(client, recvbuf, MAX_SEND_BUF);
		recvbuf[recvcount] = '\0';
		fputs(recvbuf, out);
	}

	return 1;
}

/* return value 0 fail 1 success */
int add_fd_to_pollfd(server_t *server, int fd)
{
	int	index;

	if (!server || fd < 0) return 0;

	server->count++;
	server->pollfd = (struct pollfd *)realloc(server->pollfd, 
						server->count * sizeof(struct pollfd));
	if (!server->pollfd) return 0;

	index = server->count - 1;
	server->pollfd[index].fd = fd;
	server->pollfd[index].events |= POLLIN;
/*
#ifdef _GNU_SOURCE
	server->pollfd[index].events |= POLLIN | POLLRDHUP;
#else
# error "POLLRDHUP not defined"
#endif
*/
	//server->pollfd[index].events |= POLLIN | POLLRDHUP;
	return 1;
}

/* del fd from pollfd */
int del_fd_from_pollfd(server_t *server, int index)
{
	if (!server || index < 0 || index > server->count) return 0;

	server->pollfd[server->count -1].fd = server->pollfd[index].fd;
	server->pollfd = (struct pollfd *) realloc(server->pollfd,
						(--server->count) * sizeof(struct pollfd));

	return 1;
}

void new_cmd_for_client(FILE *in, FILE *out, server_t *server)
{
	int		recvcount;
	int		clientfd;
	char		cmdbuf[MAX_CMD_LENGTH];
	client_t 	*client = NULL;

//	recvcount = fscanf(in, "%d %s", &clientfd, cmdbuf);	/* can't get arguments sucn as 4 ls -l will not get -l */
	recvcount = fscanf(in, "%d", &clientfd);
	fgets(cmdbuf, MAX_CMD_LENGTH, in);

	fprintf(stdout, "%s: client fd %d, cmd %s(%d)\n", __func__, clientfd, cmdbuf, (int)strlen(cmdbuf));

	if (recvcount == EOF) {
 	       fprintf(out, "%s: get EOF from file %d\n",
               			__func__,
		                fileno(in));
                return ;
	} else if (recvcount == 0) {
		fprintf(out, "%s: [socke fd] [command argumets]\n", __func__);
		return ;
	} else {
		;
	}


	if (clientfd == 0 && !strcmp(" show clients\n", cmdbuf)) {
		show_clients_info(server);
		clientfd = -1;
		return ;
	} else if (clientfd == 0) {
		fprintf(out, "%s: for show clients info\n", __func__);
		fprintf(out, "%s: 0 show clients\n", __func__);
		return ;
	} else {
		;
	}

        client = search_client_by_connectfd(server, clientfd);
        if (!client) {
               fprintf(out, "%s: client(%d) not found\n",
                               __func__,
                               clientfd);
               return ;
        }

        send_mesg_to_client_tcp(client, cmdbuf, strlen(cmdbuf));

	client = NULL;
	cmdbuf[0] = '\0';
	clientfd  = -1;
}

void accept_new_client(FILE *out, server_t *server)
{
	client_t *client = NULL;

        client = accept_connect_from_client(server);
        if (!add_client_to_server(server, client, NULL)) {
	        fprintf(out, "%s: add new client to server failed\n",
                             __func__);
        } else if (!add_fd_to_pollfd(server, client->connectfd)) {
                fprintf(out, "%s: add client socket to poll fd failed\n",
                             __func__);
        } else {
                fprintf(out, "%s: new client!\n",
                             __func__);
        }
        show_client_info(client, 0);
}

void do_it(FILE *in, FILE *out, server_t *server, int timeout)
{
	int		i;
	int		recvcount;
	int		clientfd;
	char		recvbuf[MAX_RECV_BUF];
	char		cmdbuf[MAX_CMD_LENGTH];
	client_t	*client = NULL;

        for ( ; ; ) {
                fprintf(out, "#");
                poll(server->pollfd, server->count, timeout);

                if (server->pollfd[0].revents & POLLIN) {
			new_cmd_for_client(in, out, server);
                }

                if (server->pollfd[1].revents & POLLIN) {
			accept_new_client(out, server);
                }

                for (i = 2; i < server->count; i++) {
			if (server->pollfd[i].fd < 0) continue;
                        if (server->pollfd[i].revents & POLLIN) {
                                client = search_client_by_connectfd(server, server->pollfd[i].fd);
                                if (!recv_mesg_from_client_tcp(client, 
							recvbuf, 
							MAX_RECV_BUF)) {
					del_client_from_server(server, client);
					destroy_client(&client);
					del_fd_from_pollfd(server, i);
				} else {
	                                fputs(recvbuf, out);
				}
/*                        } else if (server->pollfd[i].revents & POLLRDHUP) {
                                client = search_client_by_connectfd(server, server->pollfd[i].fd);
                                client = del_client_from_server(server, client);
				destroy_client(&client);
                                del_fd_from_pollfd(server, i);
*/
                        }
                }
	}
}

/* here we will use select to control multiple in & out put file descriptor */
/* need to add struct pollfd *pollfd  & the amount of pollfds in server_t for trace poll fd */
/* server->pollfd 0 & 1 elements will be for in out */
int get_user_cmd_send_to_client_print_result(FILE *in, 
					FILE *out, 
					server_t *server, 
					int timeout,
					int add_server_clients)
{
	client_t 	*client     = NULL;

	if (!in || !out || !server) return -1;

	/* add in & out */
	if (!add_fd_to_pollfd(server, fileno(in))) {
//		free(server->pollfd);
		return -1;
	}

	/* add listen socket */
	if (!add_fd_to_pollfd(server, server->listen_fd)) {
		return -1;
	}

	/* add clients socket */
	if (add_server_clients) {

		client = head_client_of_server(server);
		while (client) {
			add_fd_to_pollfd(server, client->connectfd);	/* ignor error */
			client = client->next;
		}
	}

	do_it(in, out, server, timeout);
}

