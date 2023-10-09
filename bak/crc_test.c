
#include <stdio.h>
#include <stdlib.h>

#include "checksum.h"

int main(int argc, char *argv[])
{
	int ret = 0;
	uint32_t crc = 0;
	uint8_t crc8 = 0;
	long length = 0;
	FILE *fp = NULL;
	unsigned char *buff = NULL;

	fp = fopen(argv[1], "r");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	buff = (unsigned char *)calloc(1, length);
	fseek(fp, 0, SEEK_SET);
	ret = fread(buff, 1, length, fp);
//	fprintf(stdout, "length: %ld, ret: %d\n", length, ret);
	fclose(fp);
	fp = NULL;
	crc = crc_32(buff, ret);
	crc8 = crc_8(buff, ret);

	fprintf(stdout, "%0x, %0x\n", crc, crc8);
	free(buff);
	return 0;
}

