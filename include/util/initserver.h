
#ifndef CI_INITSERVER_H
#define CI_INITSERVER_H

#include <sys/types.h>
#include <sys/socket.h>

int initserver(
		int type, 
		const struct sockaddr *addr, 
		socklen_t alen, 
		int qlen);

int initserver_reuseaddr(
		int type, 
		const struct sockaddr *addr, 
		socklen_t alen, 
		int qlen);

#endif // end of CI_INITSERVER_H

