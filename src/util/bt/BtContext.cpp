/*
 * BtContext.cpp
 *
 * 串口: BT
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <termio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <pthread.h>
#include "utils/Log.h"
#include "bt1026.h"
#include "BtContext.h"

// #include <vector>
// #include <string.h>
// #include <system/Mutex.h>
// #include "CommDef.h"
// #include "ProtocolData.h"

#define UART_DATA_BUF_LEN 16384 // 16KB

volatile AtStat at_sending = AtStIdle;

pthread_cond_t  btCond;
pthread_mutex_t btLock;
pthread_mutex_t btLock2;


// 用于拼接缓存数据，可能存在上一包数据解析未完全，残留有数据
char *mDataBufPtr = NULL;
int mDataBufLen = 0;

static const char *getBaudRate(UINT baudRate) {
    struct {
        UINT baud;
        const char *pBaudStr;
    } baudInfoTab[] = {
        {B1200, "B1200"},
        {B2400, "B2400"},
        {B4800, "B4800"},
        {B9600, "B9600"},
        {B19200, "B19200"},
        {B38400, "B38400"},
        {B57600, "B57600"},
        {B115200, "B115200"},
        {B230400, "B230400"},
        {B460800, "B460800"},
        {B921600, "B921600"}};

    int len = sizeof(baudInfoTab) / sizeof(baudInfoTab[0]);
    for (int i = 0; i < len; ++i) {
        if (baudInfoTab[i].baud == baudRate) {
            return baudInfoTab[i].pBaudStr;
        }
    }

    return NULL;
}

int isDataUrc(const char *pData) {
    if (strncmp(pData + 2, "+GATTDATA", strlen("+GATTDATA")) == 0) {
        return 9;
    } else if (strncmp(pData + 2, "+SSPDATA", strlen("+SSPDATA")) == 0) {
        return 8;
    }
    return 0;
}

static int isOk(char *data, int len)
{
        if (strcmp(data, "OK") == 0)
                        return 1;
        else
                        return 0;
}

static int isError(char *data, int len)
{
        if (strcmp(data, "ERROR") == 0)
                        return 1;
        else
                        return 0;
}

static inline int parse(char data)
{
	static char responseBuf[260] = { 0 };
	static int responseLen = 0;
	static int begin_cr = 0;
	static int begin_lf = 0;
	static int end_cr = 0;
	static int end_lf = 0;

//rintf("%s: data: %02x\n", __func__, data);
	if (begin_cr == 0)
	{
		if (data == CR)
		{
			begin_cr = 1;
			return 0;
		}
		else
		{
//	fprintf(stderr, "%d parse error: %d %d %d %d %02x\n", __LINE__, begin_cr, begin_lf,  end_cr, end_lf, data);
			return -1;
		}
	}
	else if (begin_cr == 1 && begin_lf == 0)
	{
		if (data == LF)
		{
			begin_lf = 1;
			responseLen = 0;
			return 0;
		}
		else
		{
//	fprintf(stderr, "%d parse error: %d %d %d %d %02x\n", __LINE__, begin_cr, begin_lf,  end_cr, end_lf, data);
			begin_cr = 0;
			return -1;
		}
	}
	else if (begin_cr == 1 && begin_lf == 1)
	{
		if (end_cr == 0)
		{
			if (data == CR)
			{
				end_cr = 1;
				return 0;
			}
		}
		else if (end_cr == 1 && end_lf == 0)
		{
			if (data == LF)
			{
				char tmp = '\n';
				end_lf = 1;

				responseBuf[responseLen] = '\0';
				/*
				fprintf(stdout, "response: %s\n", responseBuf);
				for (int i = 0; i < responseLen; i++)
				{
					fprintf(stdout, "%02x ", responseBuf[i]);
					if ((i + 1) % 16 == 0)
						fprintf(stdout, "\n");
				}
				fprintf(stdout, "\n");
				*/

				if (isEvent(responseBuf, responseLen) == 1)
				{
					int i;

					for (i = 0; i < BT_EVENTS_NUM; i++)
					{
						if (strncmp(responseBuf + 1, btEvents[i].name, strlen(btEvents[i].name)) == 0)
							btEvents[i].callback(btEvents[i].name, responseBuf, responseLen);
					}
				}
				else if((isOk(responseBuf, responseLen) == 1) || (isError(responseBuf, responseLen) == 1))
				{
					pthread_mutex_lock(&btLock);

					memcpy(mDataBufPtr + mDataBufLen , responseBuf, responseLen);
					mDataBufLen += responseLen;
//					memcpy(mDataBufPtr + mDataBufLen , &tmp, 1);
//					mDataBufLen += 1;
					mDataBufPtr[mDataBufLen ] = '\0';

					fprintf(stdout, "%s: %s\n", __func__, mDataBufPtr);
					fprintf(stdout, "%s: %d\n", __func__, mDataBufLen );

					pthread_cond_broadcast(&btCond);

					pthread_mutex_unlock(&btLock);
				}
				else
				{
					pthread_mutex_lock(&btLock);
					memcpy(mDataBufPtr + mDataBufLen , responseBuf, responseLen);
					mDataBufLen += responseLen;
					memcpy(mDataBufPtr + mDataBufLen , &tmp, 1);
					mDataBufLen += 1;
					pthread_mutex_unlock(&btLock);
				}


				responseLen = 0;
				begin_cr = 0;
				begin_lf = 0;
				end_cr = 0;
				end_lf = 0;

				return 0;
			}
			else
			{
		//fprintf(stderr, "%d parse error: %d %d %d %d %02x\n", __LINE__, begin_cr, begin_lf,  end_cr, end_lf, data);
				return -1;
			}
		}
		else 
		{
	//fprintf(stderr, "%d should not be here! parse error: %d %d %d %d %02x\n", __LINE__, begin_cr, begin_lf,  end_cr, end_lf, data);
			return -1;
		}
		responseBuf[responseLen++] = data;
		return 0;
	}
	else
	{
	//printf(stderr, "%d parse error: %d %d %d %d %02x\n", __LINE__, begin_cr, begin_lf,  end_cr, end_lf, data);
		begin_cr = 0;
		begin_lf = 0;
		return -1;
	}

	return 0;
}

/**
 * 功能：解析协议
 * 参数：data 待解析数据，
 * 返回值：实际解析的长度
 */
static int parseCodes(char *data, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
//printf("%s: i: %d, data: %02x\n", __func__, i, data[i]);
		parse(data[i]);
	}

	return 0;
}

// BtContext::BtContext(EnumUartId uartNum) :
BtContext::BtContext() :
    mIsOpen(false),
    reOpening(false),
    mUartID(0)
// ,mUartNumber(uartNum)
{
		pthread_cond_init(&btCond, NULL);
		pthread_mutex_init(&btLock, NULL);
		pthread_mutex_init(&btLock2, NULL);
}

BtContext::~BtContext() 
{
    delete[] mDataBufPtr;
    mDataBufPtr = NULL;
    mDataBufLen = 0;
    closeUart();
	pthread_cond_destroy(&btCond);
	pthread_mutex_destroy(&btLock);
	pthread_mutex_destroy(&btLock2);
}

BtContext *BtContext::getInstance() 
{
    static BtContext sUC;
    return &sUC;
}

UINT BtContext::nextBaud() 
{
    UINT baudRate = B115200;

    switch (mBaudRate) {
    case B9600:
        baudRate = B115200;
        break;
    case B115200:
        baudRate = B9600;
        break;
    // case B921600:
    //     baudRate = B9600;
    //     break;
    default:
        break;
    }
    return baudRate;
}

// 指定波特率打开串口(若指定0则使用下一个波特率)
bool BtContext::openUart(const char *pFileName, UINT baudRate) {
    if (mIsOpen) {
        // AT握手时需要用不同波特率 reopen
        pln("reopen ...");
        reOpening = true;
        close(mUartID);
        mUartID = 0;
        mIsOpen = false;
    }
    mBaudRate = baudRate;

    const char *sBaud = getBaudRate(baudRate);
    LOGD("openUart pFileName = %s, baudRate = %s\n", pFileName, sBaud);
    mUartID = open(pFileName, O_RDWR | O_NOCTTY);

    if (mUartID <= 0) {
        mIsOpen = false;
        reOpening = false;
    } else {
        struct termios oldtio = {0};
        struct termios newtio = {0};
        tcgetattr(mUartID, &oldtio);

        newtio.c_cflag = baudRate | CS8 | CLOCAL | CREAD;
        newtio.c_iflag = 0;     // IGNPAR | ICRNL
        newtio.c_oflag = 0;
        newtio.c_lflag = 0;     // ICANON
        newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
        newtio.c_cc[VMIN] = 1;  /* blocking read until 1 character arrives */
        tcflush(mUartID, TCIOFLUSH);
        tcsetattr(mUartID, TCSANOW, &newtio);

        fcntl(mUartID, F_SETFL, O_NONBLOCK);

        if (reOpening) {
            mIsOpen = true;
            reOpening = false;
        } else {
            mIsOpen = run("uart4bt");
            if (!mIsOpen) {
                close(mUartID);
                mUartID = 0;
            }
        }

        LOGD("openUart mIsOpen = %d\n", mIsOpen);
    }

    return mIsOpen;
}

void BtContext::closeUart() {
    LOGD("closeUart mIsOpen: %d...\n", mIsOpen);
    if (mIsOpen) {
        requestExit();

        close(mUartID);
        mUartID = 0;
        mIsOpen = false;
        mBaudRate = 0;
    }
}

int BtContext::send(const char *pData, int len)
{
	int ret = 0;
	int sent = 0;
	int left = 0;

    if (!mIsOpen) 
	{
        LOGD("BT uart not opend");
        return false;
    }

	left = len;
	while (left > 0)
	{
    	ret = write(mUartID, pData + sent, left);
		sent += ret;
		left -= ret;
    }

	if (ret <= 0)
	{
        LOGD("send Fail\n");
        return false;
	}

    return true;
}

int BtContext::sendAt(const char *pData,
                  const char *urc = NULL, 
				  char *pValue = NULL, 
				  int valueLen = 0,
				  int dataLen = 0,
				  int timeout = 100)
{
    char send_buf[1024] = {0};
    int len = 0; 
    int ret = 0;
	struct timeval  now;
	struct timespec to;
	int timeout_ms = timeout;

	len = dataLen;

    assert(len + 2 <= 1024);

    memcpy(send_buf, pData, len);
    send_buf[len] = '\r';
    send_buf[len + 1] = '\n';

//    pln("ttyS2 tx %s", send_buf);

	pthread_mutex_lock(&btLock2);

    if (!this->send(send_buf, len + 2)) 
	{
        ret = -1;
		goto err1;
    }



    pthread_mutex_lock(&btLock);

    fprintf(stdout, "%s no wait timeout_ms: %d\n", __func__, timeout_ms);
    if (timeout_ms <= 0)
    {
    	goto out;
    }

    gettimeofday(&now, NULL);

    to.tv_sec = now.tv_sec + timeout_ms / 1000;
    to.tv_nsec = (now.tv_usec * 1000) + (timeout_ms % 1000 * 1000000);
    if (to.tv_nsec >= 1000000000)
    {
    	to.tv_sec += 1;
    	to.tv_nsec %= 1000000000;
    }

    if (pthread_cond_timedwait(&btCond, &btLock, (const struct timespec *)&to) != 0)
    {
    	ret = -2;
    	goto err2;
    }


	/*
	for (int i = 0; i < mDataBufLen; i++)
	{
		fprintf(stdout, "%02x ", mDataBufPtr[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");

	fprintf(stdout, "\n1111\n");
	fprintf(stdout, "%s, %d, %d, %s, %s\n", mDataBufPtr, mDataBufLen, strlen(OK), mDataBufPtr + mDataBufLen - strlen(OK) - 1, OK);
	fprintf(stdout, "\n1111\n");
	*/

	if (strncmp(mDataBufPtr + mDataBufLen - strlen(OK), OK, strlen(OK)) == 0)
	{
		fprintf(stdout, "cmd response: %s, len: %d\n", mDataBufPtr, mDataBufLen);
		for (int i = 0; i < mDataBufLen; i++)
		{
			fprintf(stdout, "%02x ", mDataBufPtr[i]);
			if ((i + 1) % 16 == 0)
				fprintf(stdout, "\n");
		}
		fprintf(stdout, "\n");

	}
	else if (strncmp(mDataBufPtr + mDataBufLen - strlen(ERROR), ERROR, strlen(ERROR)) == 0)
	{
		ret = -3;
		goto err2;
	}
	else
	{
		ret = -4;
		fprintf(stderr, "unknow: %s, ret: %d\n", mDataBufPtr, ret); 
		goto err2;
	}

	if (urc != NULL)
	{
		if (strncmp(urc, mDataBufPtr, strlen(urc)) == 0)
		{
			fprintf(stdout, "%s: %s, %s, %d, %d, %d\n", __func__, mDataBufPtr, mDataBufPtr + strlen(urc), mDataBufLen, strlen(OK), strlen(urc));
			if (pValue && valueLen > (mDataBufLen - strlen(OK)))
			{
				memcpy(pValue, mDataBufPtr, mDataBufLen - strlen(OK));
				pValue[mDataBufLen - strlen(OK)] = '\0';
			}
			//memcpy(pValue , mDataBufPtr + strlen(urc), mDataBufLen - strlen(OK) - strlen(urc));
			//pValue[mDataBufLen - strlen(OK) - strlen(urc)] = '\0';
			//fprintf(stdout, "%s: %s\n", __func__, pValue);
		}
		else
		{
			ret = -5;
			fprintf(stderr, "urc: %s, unknow: %s, ret: %d\n", urc, mDataBufPtr, ret); 
			goto err2;
		}
	}

out:
err2:
	memset(mDataBufPtr, 0, UART_DATA_BUF_LEN);
	mDataBufLen = 0;
	pthread_mutex_unlock(&btLock);
err1:
	pthread_mutex_unlock(&btLock2);
    return ret;
}

int BtContext::sendBlk(const char *pData, int blk_len, int timeout = 100)
{
    assert(blk_len <= 241);
    int ret = 0;
    char send_buf[512] = "";

    int slen = sprintf(send_buf, "AT+GATTSEND=%d,", blk_len);
    memcpy(send_buf + slen, pData, blk_len);

//    logw("sendBlk %d %s %d", blk_len, pData, slen);

    ret = this->sendAt(send_buf, NULL, NULL, 0, slen + blk_len, timeout);
    if (ret != 0)
    {
    	fprintf(stdout, "%s send data error %d\n", __func__, ret);
    	return AtStErr;
    }

    return AtStOk;

    /*
    int len = slen + blk_len;
    int res = AtStWaiting;
    strcpy(at_urc_kw, "OK");
    at_urc_values[0] = 0;

    assert(len + 2 < 1024);
    send_buf[len] = '\r';
    send_buf[len + 1] = '\n';
    send_buf[len + 2] = 0;
//    pln("ttyS2 tx %s", send_buf);
    at_sending = AtStWaiting;
    if (!this->send(send_buf, len + 2)) {
        at_sending = AtStIdle;
        return AtStFalt;
    }


    return AtStOk;

    // 等待响应 1s
    for (int i = 0; i < 20; i++) {
        if (at_sending == AtStOk) {
            res = at_sending;
            break;
        } else if (at_sending == AtStErr) {
            res = at_sending;
            break;
        }
        Thread::sleep(50);
    }
    if (at_sending == AtStWaiting) {
        res = AtStTimeout;
    }

    at_sending = AtStIdle;

    return res;
    */
}

bool BtContext::readyToRun() {
    if (mDataBufPtr == NULL) {
        mDataBufPtr = new char[UART_DATA_BUF_LEN];
    }

    if (mDataBufPtr == NULL) {
        closeUart();
    }

    return (mDataBufPtr != NULL);
}

bool BtContext::threadLoop() 
{
	int readNum = 0;
	char buffer[230] = { 0 };

    if (reOpening) 
	{
        Thread::sleep(50);
        return true;
    }

    if (mIsOpen) 
	{
        readNum = read(mUartID, buffer, 230);
        if (readNum <= 0) {
            Thread::sleep(50);
            return true;
        }


        pln("ttyS2 rx %d, %s", readNum, buffer);
      /*
      {
    	fprintf(stdout, "origin data: %d\n", readNum);
        int ret = 0;
        int len = 0;
        char buf_tmp[1024] = { 0 };
        for (int i = 0; i < readNum; i++)
        {
                ret = sprintf(&(buf_tmp[len]), "%02x ", buffer[i]);
                len += ret;
//                fprintf(stdout, "ret: %d, len: %d\n", ret, len);
                if ((i + 1) % 16 == 0)
                {
                        ret = sprintf(&(buf_tmp[len]), "%s", "\n");
                        len += ret;
//                fprintf(stdout, "ret: %d, len: %d\n", ret, len);
                }
        }
        fprintf(stdout, "%s", buf_tmp);
        fprintf(stdout, "origin data: end\n");
      }
      */

		parseCodes(buffer, readNum);


        return true;
    }

    return false;
}


