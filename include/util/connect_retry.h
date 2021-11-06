
#ifndef CI_CONNECT_RETRY_H
#define CI_CONNECT_RETRY_H

#include <sys/types.h>
#include <sys/socket.h>


int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen);

#endif // end of CI_CONNECT_RETRY_H

