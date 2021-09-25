
/*
 * test the unix domain server's interfaces
 */

#include "udsock-server.h"
#include "udsock-client.h"

#define UNIX_DOMAIN_SERVER_PATH	"/tmp/johnlin_hello.txt"
#define UNIX_DOMAIN_SERVER_BACKLOG	10

int main(int argc, char *argv[])
{
	int	ret = 0;
	int	send_bytes = 0;
	unix_sock_server *server  = NULL;
	unix_sock_client *client1 = NULL;
//	unix_sock_client *client2 = NULL;

	if (!(server = create_unix_sock_server(
					UNIX_DOMAIN_SERVER_PATH,
					UNIX_DOMAIN_SERVER_BACKLOG))) {
		fprintf(stderr, "create server failed\n");
		exit(1);
	}

	client1 = unix_sock_accept_server(server);
	if (!client1) {
		fprintf(stderr, "accept a client get an error\n");
	}

	send_bytes = unix_sock_send_server(client1, 
				"hello world", strlen("hello world"));
	fprintf(stdout, "send %d bytes\n", send_bytes);
	fprintf(stdout, "destroying client1\n");
	destroy_unix_sock_client(&client1);
	fprintf(stdout, "destroying server\n");
	destroy_unix_sock_server(&server);
	fprintf(stdout, "ok\n");
	return ret;
}
