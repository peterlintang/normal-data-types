
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

#include "base64.h"



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
		ret = base64_encode(src, len, dst);
	else if (strcmp(argv[1], "-d") == 0)
		ret = base64_decode(src, len, dst);

	fprintf(stdout, "%s", dst);

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

