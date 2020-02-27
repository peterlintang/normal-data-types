/*
 * this implements the udp interfaces, which depends on the socket
 * interfaces.
 *
 */
/* why need these head files? */
#include <sys/socket.h>
#include <arpa/inet.h>		/* for inet_ntoa */
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include "udp.h"

/*
 * udp api interfaces
 */

udp_t *_connect_udp(const char *host, uint16_t port)
{
	udp_t *udp = calloc(1, sizeof(udp_t));

	if (!udp) 
		return udp;

	udp->sock = _new_socket(SOCKET_DGRAM);

	if (udp->sock) {

		struct hostent *hp = gethostbyname(host);

		if (hp) {

			udp->dest_host_addr.sin_addr = \
			*(struct in_addr *) (hp->h_addr_list[0]);
			udp->dest_host_addr.sin_family = AF_INET;
			udp->dest_host_addr.sin_port = htons(port);

			/* too deep,  should prevent happening */
			if (connect(udp->sock->skd,  \
				(struct sockaddr *)&udp->dest_host_addr,
				sizeof(struct sockaddr_in)) != 0) {
				perror("connect");
				_destroy_udp(&udp);
			}
		} else {
			_destroy_udp(&udp);
		}
	} else {
		_destroy_udp(&udp);
	}
	
	return udp;
}

udp_t *_bconnect_udp(const char *host, uint16_t port)
{
	udp_t *udp = calloc(1, sizeof(udp_t));
	if (!udp)
		return udp;

	udp->sock = _new_socket(SOCKET_DGRAM);
	if (!udp->sock) {
		_destroy_udp(&udp);
		return udp;
	}

	struct hostent *hp = gethostbyname(host);
	if (!hp) {
		_destroy_udp(&udp);
		return udp;
	}

	udp->dest_host_addr.sin_addr = *(struct in_addr *) (hp->h_addr_list[0]);
	udp->dest_host_addr.sin_family = AF_INET;
	udp->dest_host_addr.sin_port = htons(port);

	if (connect(udp->sock->skd, \
		(struct sockaddr *)&udp->dest_host_addr, 
		sizeof(struct sockaddr_in)) != 0) {
		perror("connect");
		_destroy_udp(&udp);
	}

	return udp;
}

void _destroy_udp(udp_t **udp_ptr)
{
	if (udp_ptr && *udp_ptr) {
		_destroy_socket((*udp_ptr)->sock);
		free(*udp_ptr);
		*udp_ptr = NULL;
	}
}





