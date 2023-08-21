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
               const char *urc = NULL, char *pValue = NULL, int valueLen = 0, int dataLen = 0, int timeout = 100);
    int sendBlk(const char *pData, int blk_len, int timeout = 100);

protected:
    virtual bool
    readyToRun();
    virtual bool threadLoop();

private:
    bool mIsOpen;
    int mUartID; // fp
    bool reOpening;
    UINT mBaudRate;


};

#endif /* _BT_BTCONTEXT_H_ */
