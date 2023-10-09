#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "ibe_common.h"
#include "ibe_client.h"

static unsigned char sendBuf[SENDBUF_SIZE];
static unsigned char recvBuf[RECVBUF_SIZE];

static pthread_t ibe_tid = 0;
static int ibe_quit_flag = 0;
static int ibe_sock = -1;
static int ibe_need_reconnect = 0;
static IBE_CONFIG ibe_config, *pcfg = &ibe_config;


static int request_default(COMMAND_HEADER *cmdHead)
{
	fprintf(stdout, "%s get cmd: 0x%x, data len: %d\n", 
			__func__, cmdHead->Command, cmdHead->DataLen);
	ibe_send_response(ibe_sock, cmdHead->Command, 0, NULL, 0);
	return 0;
}

static int response_default(COMMAND_HEADER *cmdHead)
{
	fprintf(stdout, "%s get response cmd: 0x%x, status: %d, data len: %d\n", 
			__func__, cmdHead->Command, cmdHead->Status, cmdHead->DataLen);
	return 0;
}

static int process_request(unsigned char *buf, int len)
{
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	
	pCmdHead = (COMMAND_HEADER *)(buf + PACK_HEADER_SIZE);

	switch (pCmdHead->Command)
	{
		case CMD_LOGIN_TCP:
			request_default(pCmdHead);
		break;

		case CMD_QUERY_PARAM:
			request_default(pCmdHead);
		break;

		case CMD_SETUP_PARAM:
			request_default(pCmdHead);
		break;

		case CMD_TAKE_PHOTO:
			request_default(pCmdHead);
		break;

		case CMD_POWEROFF:
			request_default(pCmdHead);
		break;

		default:
			fprintf(stdout, "%s unknow cmd %x\n", __func__, pCmdHead->Command);
		break;
	}

	return ret;
}

static int process_response(unsigned char *buf, int len)
{
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	
	pCmdHead = (COMMAND_HEADER *)(buf + PACK_HEADER_SIZE);

	switch (pCmdHead->Command)
	{
		case CMD_LOGIN_TCP:
			response_default(pCmdHead);
		break;

		case CMD_QUERY_PARAM:
			response_default(pCmdHead);
		break;

		case CMD_SETUP_PARAM:
			response_default(pCmdHead);
		break;

		case CMD_TAKE_PHOTO:
			response_default(pCmdHead);
		break;

		case CMD_POWEROFF:
			response_default(pCmdHead);
		break;

		default:
			fprintf(stdout, "%s unknow response %x\n", __func__, pCmdHead->Command);
		break;
	}

	return ret;
}

static int ibe_dispatch(unsigned char *buf, int len)
{
	COMMAND_HEADER *pCmdHead = NULL;
	
	pCmdHead = (COMMAND_HEADER *)(buf + PACK_HEADER_SIZE);

	assert(len >= PACK_HEADER_SIZE + COMMAND_HEADER_SIZE);

	printf("client pCmdHead->Command=%0x, len: %d, isRequest: %d, isResponse: %d\n", pCmdHead->Command, len, isRequest(pCmdHead->Type), isResponse(pCmdHead->Type));
	if (isRequest(pCmdHead->Type))
	{
		process_request(buf, len);
	}
	else if (isResponse(pCmdHead->Type))
	{
		process_response(buf, len);
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
	struct hostent *h;
	int rc, recvlen, max_ibe_sock;
	static struct sockaddr_in /*localAddr,*/ servAddr;
	fd_set fdRead, fdexcept;
	struct timeval tv;
	unsigned short tcp_port;

reconnect: // 连接失败后重试
	if (ibe_sock > 0)
	{
		close(ibe_sock);
		ibe_sock = -1;
	}
	server = pcfg->server;
	tcp_port = pcfg->tcp_port;
	h = gethostbyname(server);
	if (h == NULL)
	{
		printf("Cannot resolve %s, retry...\n", server);
		usleep(RETRY_TIMEOUT);
		goto reconnect;
	}
	servAddr.sin_family = h->h_addrtype;
	memcpy((char *)&servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	servAddr.sin_port = htons(tcp_port);

	ibe_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (ibe_sock < 0)
	{
		printf("Cannot open sock, retry...\n");
		usleep(RETRY_TIMEOUT);
		goto reconnect;
	}
	printf("Connecting %s:%d ...\n", server, tcp_port);
	rc = connect(ibe_sock, (struct sockaddr *)&servAddr, sizeof(servAddr));
	if (rc < 0)
	{
		printf("Cannot connect %s, retry...\n", server);
		usleep(RETRY_TIMEOUT);
		goto reconnect;
	}

	rc = ibe_send_request(ibe_sock, CMD_LOGIN_TCP, NULL, 0); // 发送登陆命令
	if (rc < 0)
	{
		printf("Cannot connect %s, retry...\n", server);
		usleep(RETRY_TIMEOUT);
		goto reconnect;
	}
	printf("Connected %s:%d.\n", server, tcp_port); // 连接成功
	tv.tv_sec = IBE_SELECT_USTIME / 1000000;
	tv.tv_usec = IBE_SELECT_USTIME % 1000000;
	for (;;)
	{
		FD_ZERO(&fdRead);
		FD_SET(ibe_sock, &fdRead);
		FD_ZERO(&fdexcept);
		FD_SET(ibe_sock, &fdexcept);
		max_ibe_sock = ibe_sock;
		if (ibe_quit_flag)
		{
			goto exit;
		}
		if (ibe_need_reconnect)
		{
			ibe_need_reconnect = 0;
			goto reconnect;
		}
		rc = select(max_ibe_sock + 1, &fdRead, NULL, &fdexcept, &tv);
		if (ibe_quit_flag)
		{
			goto exit;
		}

		switch (rc)
		{
		case -1:
			goto reconnect; // 失败重连服务器
		case 0:
			break;
		default:
			if (FD_ISSET(ibe_sock, &fdRead))
			{
				recvlen = ibe_recve_cmd(ibe_sock, recvBuf, RECVBUF_SIZE);
				if (recvlen <= 0)
					goto reconnect;
				if (ibe_dispatch(recvBuf, recvlen) < 0) // 分发命令消息
					goto reconnect;
			}
			break;
		}
	}
exit: // 主循环退出
	printf("ibe_exit\n");
	if (ibe_sock > 0)
	{
		close(ibe_sock);
		ibe_sock = -1;
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


int ibe_main(void)
{
	printf("ibe_main\n");
	ibe_quit_flag = 0;
	ibe_init();

	pthread_create(&ibe_tid, NULL, ibe_daemon, NULL);
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

int ibe_quit(void)
{
	printf("ibe_quit\n");
	ibe_quit_flag = 1;
	usleep(IBE_SELECT_USTIME + 1000);
	return 0;
}

/*
 * example
 */
int main(int argc, char *argv[])
{
	ibe_init();
	ibe_main();
	while (1)
		sleep(5);

	return 0;
}
