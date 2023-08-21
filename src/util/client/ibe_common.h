#ifndef _IBE_H_
#define _IBE_H_

#define IBE_DEVID_LEN 11
#define IBE_SERVER_LEN 32

// 命令定义
enum
{
	CMD_LOGIN_TCP = 0x0000,
	CMD_QUERY_PARAM,
	CMD_SETUP_PARAM,
	CMD_TAKE_PHOTO,
	CMD_POWEROFF,
};

struct Pack_Common_Header
{
};

typedef struct Pack_Common_Header PACK_HEADER;

typedef struct CommandHead
{
	short Command;
	short DataLen;
	char Type;	// 0 for request, 1 for response
	char Status;	// 0 for ok, others for error, 执行command得返回结果
	char OriginId[IBE_DEVID_LEN + 1];
} COMMAND_HEADER;

typedef struct IbeConfig
{
	char devid[IBE_DEVID_LEN + 1];
	char server[IBE_SERVER_LEN + 1];
	unsigned short tcp_port;
} IBE_CONFIG;

#define IBE_CONFIG_SIZE (sizeof(IBE_CONFIG))
#define PACK_HEADER_SIZE sizeof(PACK_HEADER)
#define COMMAND_HEADER_SIZE sizeof(COMMAND_HEADER)

#define IBE_SELECT_USTIME 10 // us, select超时时间

#define RETRY_TIMEOUT 5000000 // us, 重连超时时间

#define SENDBUF_SIZE 4096 // 发送包缓冲大小
#define RECVBUF_SIZE 4096 // 接收包缓冲大小

#define IBE_CONFIG_FILE "ibe.cfg"
#define IBE_DEFAULT_SERVER "127.0.0.1"
#define IBE_DEFAULT_TCP_PORT 5555
#define IBE_DEFAULT_DEVID "13000000000"

static inline int isRequest(char type)
{
	return type == 0;
}

static inline int isResponse(char type)
{
	return type == 1;
}

static inline int isEvent(char type)
{
	return type == 2;
}


int ibe_recve_cmd(int sock_fd, unsigned char *recv_buf, int recv_buf_len);
int ibe_send_request(int sock_fd, int cmd, unsigned char *data, int data_len);
int ibe_send_response(int sock_fd, int cmd, int status, unsigned char *data, int data_len);
int ibe_send_event(int sock_fd, int event, unsigned char *data, int data_len);

#endif


