
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RTCM_DATA_MAX	2048
static char rtcm_data[RTCM_DATA_MAX];
static int rtcm_data_len = 0;
static int rtcm_data_cur = 0;

static int parse_rtcm_head(unsigned char *data, int len)
{
	printf("%s: %0x %0x %0x\n", __func__, data[0], data[1], data[2]);
	if ((len == 3) 
		&& data[0] == 0xD3 
		&& ((data[1] & 0xfc) == 0x00))
	{
		return ((data[1] & 0x03) << 8) | data[2];
	}
	else
	{
		return -1;
	}
}

static int parse_rtcm(FILE *fp)
{
	unsigned char data[3] = { 0 };
	int ret = 0;
	int body_len = 0;

while (1)
{
	ret = fread(data, 1, 3, fp);
	if (ret < 3) break;
	body_len = parse_rtcm_head(data, 3);
	printf("head: %0x %0x %0x\n", data[0], data[1], data[2]);
	printf("body_len: %d\n", body_len);

	for (int i = 0; i < body_len; i++)
	{
		fread(data, 1, 1, fp);
		printf("%02x ", data[0]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	ret = fread(data, 1, 3, fp);
	printf("\ncrc: %0x %0x %0x\n", data[0], data[1], data[2]);
}

	return 0;
}


int main(int argc, char *argv[])
{
	FILE *fp = NULL;

	fp = fopen(argv[1], "r");

	parse_rtcm(fp);

	fclose(fp);
	return 0;
}

