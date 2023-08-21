#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "ibe_common.h"


int ibe_recve_cmd(int sock_fd, unsigned char *recv_buf, int recv_buf_len)
{
	int recv_len = 0;
	int left = 0;
	int ret = 0;
	PACK_HEADER *packHead = NULL;
	COMMAND_HEADER *cmdHead = NULL;

	assert(recv_buf_len >= PACK_HEADER_SIZE + COMMAND_HEADER_SIZE);

	recv_len = 0;
	left = sizeof(PACK_HEADER);
	while (left > 0)
	{
		ret = recv(sock_fd, recv_buf + recv_len, left, 0);
		if (ret > 0)
		{
			left -= ret;
			recv_len += ret;
		}
		else
		{
			fprintf(stderr, "%s ret: %d, recv: %d, left: %d, strerror: %s\n", 
							__func__, ret, recv_len, left, strerror(errno));
			return -2;
		}
	}

	recv_len = 0;
	left = sizeof(COMMAND_HEADER);
	while (left > 0)
	{
		ret = recv(sock_fd, recv_buf + PACK_HEADER_SIZE + recv_len, left, 0);
		if (ret > 0)
		{
			left -= ret;
			recv_len += ret;
		}
		else
		{
			fprintf(stderr, "%s ret: %d, recv: %d, left: %d, strerror: %s\n", 
							__func__, ret, recv_len, left, strerror(errno));
			return -2;
		}
	}

	cmdHead = (COMMAND_HEADER *)(recv_buf + PACK_HEADER_SIZE); 
	recv_len = 0;
	left = cmdHead->DataLen;
	if (left + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE > recv_buf_len)
	{
		fprintf(stderr, "%s recv_buf len: %d, left: %d, pack header len: %d, cmd header len: %d\n", 
						__func__, recv_buf_len, left, (int)PACK_HEADER_SIZE, (int)COMMAND_HEADER_SIZE);
		return -1;
	}

	while (left > 0)
	{
		ret = recv(sock_fd, recv_buf + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE + recv_len, left, 0);
		if (ret > 0)
		{
			left -= ret;
			recv_len += ret;
		}
		else
		{
			fprintf(stderr, "%s ret: %d, recv: %d, left: %d, strerror: %s\n", 
							__func__, ret, recv_len, left, strerror(errno));
			return -2;
		}
	}

#ifdef CLIENT_DEBUG
	fprintf(stdout, "%s \n", __func__);
	for (int i = 0; i < cmdHead->DataLen + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE; i++)
	{
		fprintf(stdout, "0x%0x ", recv_buf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	return cmdHead->DataLen + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE;
}

int ibe_send_request(int sock_fd, int cmd, unsigned char *data, int data_len)
{
	int left = 0;
	int sent = 0;
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	unsigned char sendBuf[SENDBUF_SIZE] = { 0 };

	pCmdHead = (COMMAND_HEADER *)(sendBuf + PACK_HEADER_SIZE);

	pCmdHead->Command = 0x0000FFFF & cmd;
	pCmdHead->DataLen = data_len;
	pCmdHead->Type = 0;
	pCmdHead->Status = 0;
	memcpy(pCmdHead->OriginId, "12345678", sizeof("12345678"));

	if (SENDBUF_SIZE - PACK_HEADER_SIZE - COMMAND_HEADER_SIZE >= data_len)
	{
		if (data && data_len > 0)
		{
			memcpy(sendBuf + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE, data, data_len);
		}
	}
	else
	{
		fprintf(stderr, "%s send_buf len: %d, data_len: %d\n", 
						__func__, SENDBUF_SIZE, data_len);
		return -1;
	}

	left = data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE ;
	sent = 0;
	while (left > 0)
	{
		ret = send(sock_fd, sendBuf + sent, left, 0);
		if (ret > 0)
		{
			left -= ret;
			sent += ret;
		}
		else
		{
			fprintf(stderr, "%s ret: %d, sent: %d, left: %d, strerror: %s\n", 
							__func__, ret, sent, left, strerror(errno));
			return -2;
		}
	}

#ifdef CLIENT_DEBUG
	fprintf(stdout, "%s \n", __func__);
	for (int i = 0; i < data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE; i++)
	{
		fprintf(stdout, "0x%0x ", sendBuf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	return 0;
}

int ibe_send_response(int sock_fd, int cmd, int status, unsigned char *data, int data_len)
{
	int left = 0;
	int sent = 0;
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	unsigned char sendBuf[SENDBUF_SIZE] = { 0 };

	assert(data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE <= SENDBUF_SIZE);

	pCmdHead = (COMMAND_HEADER *)(sendBuf + PACK_HEADER_SIZE);

	pCmdHead->Command = 0x0000FFFF & cmd;
	pCmdHead->Type = 1;
	pCmdHead->Status = status;
	pCmdHead->DataLen = data_len;
	memcpy(pCmdHead->OriginId, "12345678", sizeof("12345678"));

	if (SENDBUF_SIZE - PACK_HEADER_SIZE - COMMAND_HEADER_SIZE >= data_len)
	{
		if (data && data_len > 0)
		{
			memcpy(sendBuf + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE, data, data_len);
		}
	}
	else
	{
		fprintf(stderr, "%s send_buf len: %d, data_len: %d\n", 
						__func__, SENDBUF_SIZE, data_len);
		return -1;
	}

	left = data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE ;
	sent = 0;
	while (left > 0)
	{
		ret = send(sock_fd, sendBuf + sent, left, 0);
		if (ret > 0)
		{
			left -= ret;
			sent += ret;
		}
		else
		{
			fprintf(stderr, "%s ret: %d, sent: %d, left: %d, strerror: %s\n", 
							__func__, ret, sent, left, strerror(errno));
			return -2;
		}
	}

#ifdef CLIENT_DEBUG
	fprintf(stdout, "%s \n", __func__);
	for (int i = 0; i < data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE; i++)
	{
		fprintf(stdout, "0x%0x ", sendBuf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	return 0;
}

int ibe_send_event(int sock_fd, int event, unsigned char *data, int data_len)
{
	int left = 0;
	int sent = 0;
	int ret = 0;
	COMMAND_HEADER *pCmdHead = NULL;
	unsigned char sendBuf[SENDBUF_SIZE] = { 0 };

	pCmdHead = (COMMAND_HEADER *)(sendBuf + PACK_HEADER_SIZE);

	pCmdHead->Command = 0x0000FFFF & event;
	pCmdHead->DataLen = data_len;
	pCmdHead->Type = 2;
	pCmdHead->Status = 0;
	memcpy(pCmdHead->OriginId, "12345678", sizeof("12345678"));

	if (SENDBUF_SIZE - PACK_HEADER_SIZE - COMMAND_HEADER_SIZE >= data_len)
	{
		if (data && data_len > 0)
		{
			memcpy(sendBuf + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE, data, data_len);
		}
	}
	else
	{
		fprintf(stderr, "%s send_buf len: %d, data_len: %d\n", 
						__func__, SENDBUF_SIZE, data_len);
		return -1;
	}

	left = data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE ;
	sent = 0;
	while (left > 0)
	{
		ret = send(sock_fd, sendBuf + sent, left, 0);
		if (ret > 0)
		{
			left -= ret;
			sent += ret;
		}
		else
		{
			fprintf(stderr, "%s ret: %d, sent: %d, left: %d, strerror: %s\n", 
							__func__, ret, sent, left, strerror(errno));
			return -2;
		}
	}

#ifdef CLIENT_DEBUG
	fprintf(stdout, "%s \n", __func__);
	for (int i = 0; i < data_len + PACK_HEADER_SIZE + COMMAND_HEADER_SIZE; i++)
	{
		fprintf(stdout, "0x%0x ", sendBuf[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
#endif

	return 0;
}



