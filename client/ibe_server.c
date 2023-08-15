#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <arpa/inet.h>

#include "ibe_common.h"
#include "ibe_server.h"


static unsigned char sendBuf[SENDBUF_SIZE];
static unsigned char recvBuf[RECVBUF_SIZE];

static pthread_t ibe_tid = 0;
static int ibe_quit_flag = 0;
static int ibe_need_reconnect = 0;
static IBE_CONFIG ibe_config, *pcfg = &ibe_config;
static int listen_fd = -1;
static int accept_fd = -1;

static int request_default(int sock_fd, COMMAND_HEADER *cmdHead)
{
	fprintf(stdout, "%s get cmd: 0x%x, data len: %d\n", 
			__func__, cmdHead->Command, cmdHead->DataLen);
	ibe_send_response(sock_fd, cmdHead->Command, 0, NULL, 0);
	return 0;
}

static int response_default(int sock_fd, COMMAND_HEADER *cmdHead)
{
	fprintf(stdout, "%s get response cmd: 0x%x, status: %d, data len: %d\n", 
			__func__, cmdHead->Command, cmdHead->Status, cmdHead->DataLen);
	return 0;
}

static int process_request(int sock_fd, unsigned char *buf, int len)
{
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	
	pCmdHead = (COMMAND_HEADER *)(buf + PACK_HEADER_SIZE);

	switch (pCmdHead->Command)
	{
		case CMD_LOGIN_TCP:
			request_default(sock_fd, pCmdHead);
		break;

		case CMD_QUERY_PARAM:
			request_default(sock_fd, pCmdHead);
		break;

		case CMD_SETUP_PARAM:
			request_default(sock_fd, pCmdHead);
		break;

		case CMD_TAKE_PHOTO:
			request_default(sock_fd, pCmdHead);
		break;

		case CMD_POWEROFF:
			request_default(sock_fd, pCmdHead);
		break;

		default:
			fprintf(stdout, "%s unknow cmd %x\n", __func__, pCmdHead->Command);
		break;
	}

	return ret;
}

static int process_response(int sock_fd, unsigned char *buf, int len)
{
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	
	pCmdHead = (COMMAND_HEADER *)(buf + PACK_HEADER_SIZE);

	switch (pCmdHead->Command)
	{
		case CMD_LOGIN_TCP:
			response_default(sock_fd, pCmdHead);
		break;

		case CMD_QUERY_PARAM:
			response_default(sock_fd, pCmdHead);
		break;

		case CMD_SETUP_PARAM:
			response_default(sock_fd, pCmdHead);
		break;

		case CMD_TAKE_PHOTO:
			response_default(sock_fd, pCmdHead);
		break;

		case CMD_POWEROFF:
			response_default(sock_fd, pCmdHead);
		break;

		default:
			fprintf(stdout, "%s unknow response %x\n", __func__, pCmdHead->Command);
		break;
	}

	return ret;
}

static int ibe_dispatch(int sock_fd, unsigned char *buf, int len)
{
	COMMAND_HEADER *pCmdHead = NULL;
	
	pCmdHead = (COMMAND_HEADER *)(buf + PACK_HEADER_SIZE);

	assert(len >= PACK_HEADER_SIZE + COMMAND_HEADER_SIZE);

	printf("server pCmdHead->Command=%0x, len: %d, isRequest: %d, isResponse: %d\n", pCmdHead->Command, len, isRequest(pCmdHead->Type), isResponse(pCmdHead->Type));
	if (isRequest(pCmdHead->Type))
	{
		process_request(sock_fd, buf, len);
	}
	else if (isResponse(pCmdHead->Type))
	{
		process_response(sock_fd, buf, len);
	}
	else
	{
		fprintf(stderr, "unknow cmd: %x\n", pCmdHead->Command);
	}

	return 0;
}

/*
 *
 */
int ibe_uninit(void);

static void *ibe_daemon(void *arg)
{
	char *server;
	unsigned short tcp_port;
	int recvlen = 0;
        struct sockaddr_in      serveraddr;
	fd_set fdRead, fdexcept;
	struct timeval tv;
	int max_ibe_sock = 0;
	int reuse = 1;

	server = pcfg->server;
	tcp_port = pcfg->tcp_port;

        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_fd < 0) 
	{
                fprintf(stderr, "%s: socket error\n", __func__);
                perror("socket");
                return NULL;
        }

	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
	{
                fprintf(stderr, "%s: socket reuseaddr error \n", __func__);
                perror("socket");
                return NULL;
	}

        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port   = htons(tcp_port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
//        inet_pton(AF_INET, &serveraddr.sin_addr, server);

        if (bind(listen_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) 
	{
                fprintf(stderr, "%s: bind error\n", __func__);
                perror("bind");
                return NULL;
        }

        if (listen(listen_fd, 1)) 
	{
                fprintf(stderr, "%s: listen error\n", __func__);
                perror("listen");
                listen_fd = -1;
                return NULL;
        }

	for (;;)
	{
		accept_fd = accept(listen_fd, NULL, NULL);
		if (accept_fd < 0) 
		{
			perror("accept");
			break;
		} else 
		{
			fprintf(stdout, "accept OK: %d\n", accept_fd);
		}

		while (1) 
		{
			recvlen = ibe_recve_cmd(accept_fd , recvBuf, RECVBUF_SIZE);
			if (recvlen <= 0)
				break;
			if (ibe_dispatch(accept_fd, recvBuf, recvlen) < 0)
				break;
		}
		if (accept_fd > 0)
		{
			close(accept_fd);
			accept_fd = -1;
		}

	}

	printf("ibe_exit\n");
	if (listen_fd> 0)
	{
		close(listen_fd);
		listen_fd = -1;
	}
	ibe_uninit();

	return NULL;
}

int ibe_read_config(void)
{
	FILE *fp;
	int len;
	fp = fopen(IBE_CONFIG_FILE, "rb");
	if (fp == NULL)
		return -1;
	len = fread(&ibe_config, 1, IBE_CONFIG_SIZE, fp);
	fclose(fp);
	if (len != IBE_CONFIG_SIZE)
		return -1;
	return 0;
}

int ibe_save_config(void)
{
	FILE *fp;
	int len;
	fp = fopen(IBE_CONFIG_FILE, "wb");
	if (fp == NULL)
		return -1;
	len = fwrite(&ibe_config, 1, IBE_CONFIG_SIZE, fp);
	fclose(fp);
	if (len != IBE_CONFIG_SIZE)
		return -1;
	return 0;
}


int ibe_set_default_config(void)
{
	memset(pcfg, 0, sizeof(IBE_CONFIG));
	strcpy(pcfg->devid, IBE_DEFAULT_DEVID);
	strcpy(pcfg->server, IBE_DEFAULT_SERVER);
	pcfg->tcp_port = IBE_DEFAULT_TCP_PORT;
	return 0;
}

int ibe_uninit(void)
{
	ibe_save_config();
	return 0;
}

int ibe_init(void)
{
	ibe_set_default_config();
	// TODO - open serial ports to UWB, ESC

	return 0;
}

int ibe_main(void)
{
	printf("ibe_main\n");
	ibe_quit_flag = 0;
	ibe_init();

	pthread_create(&ibe_tid, NULL, ibe_daemon, NULL);
	return 0;
}

int ibe_quit(void)
{
	printf("ibe_quit\n");
	ibe_quit_flag = 1;
	if (listen_fd > 0)
	{
		close(listen_fd);
		listen_fd = -1;
	}
	if (accept_fd > 0)
	{
		close(accept_fd);
		accept_fd= -1;
	}
	usleep(IBE_SELECT_USTIME + 1000);
	printf("ibe_quit done\n");
	return 0;
}

/*
 * example
 */
int main(int argc, char *argv[])
{
	int count = 0;
	ibe_init();
	ibe_main();

	while (count < 3) 
	{
		count++;
		sleep(5);
	}

	ibe_quit();

	return 0;
}
