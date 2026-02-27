
/*
 * gcc zlib_test.c -lz
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int main(int argc, char *argv[])
{
	char data[] = "hello world! this is compress/uncompress test! yes, another test may be good, hello world";
	char buff[512] = { 0 };
	char buff2[512] = { 0 };
	uLongf destLen = 512;
	uLong sourceLen = 0;
	int result = 0;

	sourceLen = strlen(data);
	result = compress(buff, &destLen, data, sourceLen);
	printf("compress result: %d, %lu\n", result, destLen);
	printf("%s\n", buff);

	sourceLen = destLen;
	destLen = 512;
	result = uncompress(buff2, &destLen, buff, sourceLen);
	printf("uncompress result: %d, %lu\n", result, destLen);
	printf("%s\n", buff2);
	return 0;
}

