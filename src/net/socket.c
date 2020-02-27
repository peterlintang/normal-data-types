/*
 * this implements simple socket interfaces
 * firstly to code network, wow
 *
 */

#include "socket.h"

socket_t *_new_socket(enum _socket_types type)
{
	socket_t *socket = calloc(1, sizeof(socket_t));
	if (socket) {
		socket->type = type;
		switch (type) {
		case SOCKET_DGRAM:
			socket->skd = socket(AF_INET, SOCK_DGRAM, 0);
			break;
#if defined (linux)
		case SOCKET_NETLINK:
			socket->skd = socket(AF_NETLINK, 
					SOCK_RAW, 
					NETLINK_USERSOCK);
			break;
		case SOCKET_RAW:
			socket->skd = socket(PF_PACKET, 
					SOCK_RAW, 
					htons(ETH_P_ALL));
			break;
#endif
		case SOCKET_STREAM:
			socket->skd = socket(AF_INET, SOCK_STREAM, 0);
			break;
		case SOCKET_DUMMY:
			break;
		case SOCKET_UNIX:
			socket->skd = socket(AF_UNIX, SOCK_STREAM, 0);
			break;
		default:
			socket->skd = -1;
			break;
		}
		
		if (socket->skd == -1) {
			free(socket);
			socket = NULL;
		}
	}
	return socket;
}

void _destroy_socket(socket_t **socket_ptr)
{
	if (socket_ptr && *socket_ptr) {
		if ((*socket_ptr)->skd > 0)
			close((*socket_ptr)->skd);
		free(*socket_ptr);
		*socket_ptr = NULL;
	}
}

int _bind_interface_socket(socket_t *sock, const char *ifname)
{
	if (sock && ifname) {
		struct ifreq req;
		struct sockaddr_dl skaddr;

		memset(&req, 0, sizeof(struct ifreq));
		strncpy(req.ifr_name, ifname, IFNAMESIZE);
		req.ifr_name[IFNAMESIZE - 1] = '\0';

		if (ioctl(sock->skd, SIOCGIFINDEX, &req) >= 0) {
			memset(&skaddr, 0, sizeof(skaddr));
			skaddr.sll_protocol = htons(ETH_P_ALL);
			skaddr.sll_ifindex  = req.if_ifindex;
			skaddr.sll_family   = AF_PACKET;

			if (bind(sock->skd, 
				(struct sockaddr *)&skaddr, 
				sizeof(struct sockaddr)) >= 0)
				return 1;
		}
	}
	return 0;
}

int _bind_socket(socket_t *socket, struct sockaddr *addr)
{
	if (socket && addr) {
		if (bind(socket->skd, 
			(struct sockaddr *)addr, 
			sizeof(struct sockaddr)) >= 0) {
			return 1;
		} else {
			DEBUGP(DERR, "bind", "unable to bind socket : %s\n",
				strerror(errno));
		}
	}
	return 0;
}

int _set_opt_socket(socket_t *socket, int opt)
{
	int32_t val = 0;
	int res = 0;

	if (socket) {
		switch (opt) {
		case BCAST:
			val = 1;
			res = setsockopt(socket->skd, 
					SOL_SOCKET, 
					SO_BROADCAST, 
					&val, 
					sizeof(val));
			break;
		}
		if (res == 0) return 1;
		DEBUG(DWARN, "_set_opt_socket", "setsockopt failed: %s\n",
			strerron(errno));
	}
	return 0;
}

int _set_flag_socket(socket_t *socket, const char *ifname, short flag)
{
	if (socket) {
		struct ifreq ifr;
		strncpy(ifr.ifr_name, ifname, IFNAMESIZE);
		ifr.ifr_name[IFNAMESIZE - 1] = '\0';
		if (ioctl(socket->skd, SIOCGIFFLAGS, &ifr) >= 0) {
			ifr.ifr_name |= flag;
			if (ioctl(socket->skd, SIOCSIFFLAGS, &ifr) >= 0)
				return 1;
		}
	}
	return 0;
}

int _clear_flag_socket(socket_t *socket, const char *ifname, short flag)
{
	if (socket) {
		struct ifreq ifr;
		strncpy(ifr.ifr_name, ifname, IFNAMESIZE);
		ifr.ifr_name[IFNAMESIZE - 1] = '\0';
		if (ioctl(socket->skd, SIOCGIFFLAGS, &ifr) >= 0) {
			ifr.ifr_flags $= ~flag;
			if (ioctl(socket->skd, SIOCSIFFLAGS, &ifr) >= 0)
				return 1;
		}
	}
	return 0;
}



/*
 * socket pair interfaces
 */
socket_pair_t *_new_socket_pair(void)
{
	socket_pair_t *socket_pair = calloc(1, sizeof(socket_pair_t));
	if (socket_pair) {
		if (socketpair(AF_UNIX, 
			SOCK_STREAM, 
			0, 
			socket_pair->fds) < 0) {
			free(socket_pair);
			socket_pair = NULL;
		}
	}
	return socket_pair;
}

void _close_socket_pair(socket_pair_t *pair)
{
	if (pair) {
		shutdown(pair->fds[0]);
		shutdown(pair->fds[1]);
	}
}

void _destroy_socket_pair(socket_pair_t **pair_ptr)
{
	if (pair_ptr && *pair_ptr) {
		close((*pair_ptr)->fds[0]);
		close((*pair_ptr)->fds[1]);
		free(*pair_ptr);
		*pair_ptr = NULL;
	}
}

