
/*
 * description: implement the base64 encode decode
 * file: base64.c
 * date: 09222022
 * author: john lin
 */

/* for main */
#include <sys/stat.h>
#include <fcntl.h>

/*****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "base64.h"


int base64_encode(char *src, unsigned int src_len, char *dst, unsigned int dst_len)
{
	int i;
	int count = 0;
	int left = 0;
	static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	count = src_len / 3;
	left = src_len % 3;

	for (i = 0; i < count; i++)
	{
		dst[i * 4 + 0] = table[(src[i * 3 + 0] & 0xfc) >> 2];
		dst[i * 4 + 1] = table[((src[i * 3 + 1] & 0xf0) >> 4) | ((src[i * 3 + 0] & 0x03)) << 4];
		dst[i * 4 + 2] = table[((src[i * 3 + 1] & 0x0f) << 2) | ((src[i * 3 + 2] & 0xc0) >> 6)];
		dst[i * 4 + 3] = table[src[i * 3 + 2] & 0x3f];
	}

	if (left == 1)
	{
		dst[i * 4] = table[(src[i * 3] & 0xfc) >> 2];
		dst[i * 4 + 1 ] = table[(src[i * 3] & 0x03) << 4];
		dst[i * 4 + 2 ] = '=';
		dst[i * 4 + 3 ] = '=';
	}
	else if (left == 2)
	{
		dst[i * 4 + 0] = table[(src[i * 3 + 0] & 0xfc) >> 2];
		dst[i * 4 + 1] = table[((src[i * 3 + 1] & 0xf0) >> 4) | ((src[i * 3 + 0] & 0x03)) << 4];
		dst[i * 4 + 2] = table[((src[i * 3 + 1] & 0x0f) << 2)];
		dst[i * 4 + 3 ] = '=';
	}
	
	return (i + (left == 0 ? 0 : 1)) * 4 ;
}

int base64_decode(char *src, unsigned int src_len, char *dst, unsigned int dst_len)
{
	int sub = 0;
	int count = 0;
	int i;
	static char table[] = {
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  62, 0,  0,  0,  63, 52, 53,
		54, 55, 56, 57, 58, 59, 60, 61,  0,  0,
		0,  0,  0,  0,  0,   0,  1,  2,  3,  4,
		5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 
		25, 0,  0,  0,  0,  0,  0,  26, 27, 28, 
		29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
		49, 50, 51
	};

/*
	if (src_len % 4 != 0)
		return -1;
*/

	count = src_len / 4;
	for (i = 0; i < count; i++)
	{
		dst[i * 3 + 0] = (table[src[i * 4 + 0]] & 0x3f) << 2 | (table[src[i * 4 + 1]] & 0x30) >> 4;
		dst[i * 3 + 1] = ((table[src[i * 4 + 1]] & 0x0f) << 4) | ((table[src[i * 4 + 2]] & 0x3c) >> 2);
		dst[i * 3 + 2] = ((table[src[i * 4 + 2]] & 0x03) << 6) | (table[src[i * 4 + 3]] & 0x3f);
	}

	if ((src[src_len - 1] == '=') && (src[src_len - 2] == '='))
		sub = 2;
	else if (src[src_len - 1] == '=')
		sub = 1;

//	dst[count * 3 - sub] = '\0';

	return count * 3 - sub;
}



#if 1
int main(int argc, char *argv[])
{
#define SRC_LEN 1280 
#define DST_LEN 2560
	char src[SRC_LEN] = { 0 };
	char dst[DST_LEN] = { 0 };
	int ret = 0;
	int i;
	int fd = -1;
	int len = -1;
	int max_read = 0;

	if (strcmp(argv[1], "-e") == 0)
		max_read = 3 * 10;
	else if (strcmp(argv[1], "-d") == 0)
		max_read = 4 * 10;

	fd = open(argv[2], O_RDONLY);

	while ((len = read(fd, src, max_read)) > 0)
	{

	memset(dst, 0, sizeof(dst));
	if (strcmp(argv[1], "-e") == 0)
		ret = base64_encode(src, len, dst, DST_LEN);
	else if (strcmp(argv[1], "-d") == 0)
		ret = base64_decode(src, len, dst, DST_LEN);

	if (ret != write(1, dst, ret))
	{
		fprintf(stderr, "write failed %d %s\n", ret, strerror(errno));
		return -1;
	}

	}

	close(fd);

/* for same with base64
	for (i = 0; i < ret; i++)
	{
		fprintf(stdout, "%c", dst[i]);
		if ((i + 1) % 76 == 0)
			fprintf(stdout, "\n");
	}

	if ((i + 1) % 76 != 0)
		fprintf(stdout, "\n");
*/


	return 0;
}
#endif

