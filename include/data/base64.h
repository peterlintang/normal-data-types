
/*
 * description: implement the base64 encode decode
 * file: base64.h
 * date: 09222022
 * author: john lin
 */
#ifndef __LIN_BASE64_H__
#define __LIN_BASE64_H__


int base64_encode(char *src, unsigned int src_len, char *dst, unsigned int dst_len);

int base64_decode(char *src, unsigned int src_len, char *dst, unsigned int dst_len);


#endif

