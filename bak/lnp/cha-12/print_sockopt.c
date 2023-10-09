

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "print_sockopt.h"


SockOpts opts[] = {
	{SOL_SOCKET, SO_BROADCAST, "SO_BROADCAST", OPTVALINT},
	{SOL_SOCKET, SO_DEBUG, "SO_DEBUG", OPTVALINT},
	{SOL_SOCKET, SO_DONTROUTE, "SO_DONTROUTE", OPTVALINT},
	{SOL_SOCKET, SO_ERROR, "SO_ERROR", OPTVALINT},
	{SOL_SOCKET, SO_KEEPALIVE, "SO_KEEPALIVE", OPTVALINT},
	{0, 0, NULL, OPTVALMAX},
};

void print_sockopt(SockOpts *opt)
{
	if (opt == NULL) return ;

	fprintf(stdout, "name: %s, value: ", opt->name);
	switch (opt->valtype)
	{
		case OPTVALINT:
			fprintf(stdout, "%d\n", opt->val.val);
			break;
		case OPTVALINGER:
			break;
		case OPTVALTIMEVAL:
			break;
		case OPTVALUCHAR:
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int sock = -1;
	int i = 0;
	int len = 0;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) return -1;

	while (opts[i].valtype != OPTVALMAX)
	{
		len = sizeof(opts[i].val.val);
		ret = getsockopt(sock, opts[i].level, opts[i].optname, &(opts[i].val.val), &len);
		if (ret == 0)
			print_sockopt(opts + i);
		i++;
	}

	close(sock);
	return 0;
}




