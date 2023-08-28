/*
 * BtContext.h
 *
 */

#ifndef _BT_BTCONTEXT_H_
#define _BT_BTCONTEXT_H_

#include <vector>
#include "system/Thread.h"
#include "utils/Log.h"
#include "uart/CommDef.h"

#define BTCONTEXT BtContext::getInstance()
#define BT_DEV "/dev/ttyS2"

class BtContext : public Thread {
public:
    BtContext();
    virtual ~BtContext();

    static BtContext *getInstance(); // 单串口

    UINT nextBaud();
    bool openUart(const char *pFileName, UINT baudRate);
    void closeUart();

    bool isOpen() {
        return mIsOpen;
    }

    int send(const char *pData, int len);
    int sendAt(const char *pData,
               const char *urc, char *pValue, int valueLen, int dataLen, int timeout);
    int sendBlk(const char *pData, int blk_len, int timeout);


protected:
    virtual bool
    readyToRun();
    int readGattData(char *buf, int len);
    int parse(char data);
    int parseCodes(char *data, int len);
    virtual bool threadLoop();

private:
    bool mIsOpen;
    int mUartID; // fp
    bool reOpening;
    UINT mBaudRate;


};

#endif /* _BT_BTCONTEXT_H_ */
