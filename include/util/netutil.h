
#ifndef CI_NETUTIL_H
#define CI_NETUTIL_H

#include <sys/types.h>
#include <sys/socket.h>


int connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen);

int get_ip_by_ifname(char *ifname, char *ip, int ip_len);

#endif // end of CI_NETUTIL_H

