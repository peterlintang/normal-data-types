
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
	UnixSockClient_T client = NULL;

	client = MODULE_FUN_NAME(UnixSockClient, create)(UNIX_DOMAIN_SERVER_PATH);
	if (!client) {
		fprintf(stderr, "create client error\n");
		exit(1);
	}

	ret = MODULE_FUN_NAME(UnixSockClient, connect)(client);
	recv_bytes = MODULE_FUN_NAME(UnixSockClient, recv)(client, buf, 256);
	buf[recv_bytes] = '\0';
//	buf[recv_bytes + 1] = '\0';
	fprintf(stdout, "recv %d bytes, %s, ret: %d\n", recv_bytes, buf, ret);
	MODULE_FUN_NAME(UnixSockClient, destroy)(&client);

	return ret;
}
