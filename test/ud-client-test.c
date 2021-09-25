
/*
 * test the unix domain client's interfaces
 */

#include "udsock-server.h"
#include "udsock-client.h"

#include "module.h"

#define UNIX_DOMAIN_SERVER_PATH "/tmp/johnlin_hello.txt"

int main(int argc, char *argv[])
{
	int	ret = 0;
	int	recv_bytes = 0;
	char	buf[256];
	unix_sock_client *client = NULL;

	client = create_unix_sock_client(UNIX_DOMAIN_SERVER_PATH);
	if (!client) {
		fprintf(stderr, "create client error\n");
		exit(1);
	}

	ret = unix_sock_connect(client);
	recv_bytes = unix_sock_recv_client(client, buf, 256);
	buf[recv_bytes + 1] = '\0';
	fprintf(stdout, "recv %d bytes, %s, ret: %d\n", recv_bytes, buf, ret);
	destroy_unix_sock_client(&client);

	return ret;
}
