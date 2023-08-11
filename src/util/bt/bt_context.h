
#ifndef _BT_BTCONTEXT_H_
#define _BT_BTCONTEXT_H_


#define BT_DEV "/dev/ttyS2"

int send_raw(const char *pData, int len);
int send_at(const char *pData, const char *urc, 
		char *pValue, int valueLen);
int send_blk(const char *pData, int blk_len);

#endif /* _BT_BTCONTEXT_H_ */

