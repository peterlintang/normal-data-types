
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define RTCM_DATA_MAX	2048
static unsigned char rtcm_data[RTCM_DATA_MAX];
static int rtcm_data_len = 0;
static int rtcm_data_cur = 0;

static void print_rtcm_packet(unsigned char *data, int len)
{
	for (int i = 0; i < len; i++)
	{
		if ((i + 1) % 16 == 0)
			printf("%02x\n", data[i]);
		else
			printf("%02x ", data[i]);
	}
	if (len % 16 != 0)
		printf("\n");
}

static void get_rtcm_packet(unsigned char *data, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (rtcm_data_cur == 0)
		{
			if (data[i] == 0xD3)
			{
				rtcm_data[rtcm_data_cur++] = data[i];
			}
		}
		else
		{
                        if (rtcm_data_cur >= RTCM_DATA_MAX)
                        {
                                printf("rtcm over room %02x %02x %02x, reset\n", rtcm_data[0], rtcm_data[1], rtcm_data[2]);
                                rtcm_data_len = -1;
                                rtcm_data_cur = 0;
                                continue;
                        }
			
			rtcm_data[rtcm_data_cur++] = data[i];
			if (rtcm_data_cur == 3)
			{
				if (rtcm_data[0] == 0xD3 
					&& ((rtcm_data[1] & 0xfc) == 0x00))
				{
					rtcm_data_len = ((rtcm_data[1] & 0x03) << 8) | rtcm_data[2];
					printf("rtcm data len: %d\n", rtcm_data_len);
				}
				else
				{
					printf("rtcm error head %02x %02x %02x, reset\n", rtcm_data[0], rtcm_data[1], rtcm_data[2]);
					rtcm_data_len = -1;
					rtcm_data_cur = 0;
				}
			}
		}
		if (rtcm_data_len >= 0 && rtcm_data_len + 6 == rtcm_data_cur)
		{
			printf("get a rtcm packet:\n");
			print_rtcm_packet(rtcm_data, rtcm_data_cur);
			rtcm_data_len = -1;
			rtcm_data_cur = 0;
		}
	}
}

int main(int argc, char *argv[])
{
	int ret = 0;
	unsigned char data[128];
	FILE *fp = NULL;

	fp = fopen(argv[1], "r");

	while ((ret = fread(data, 1, 128, fp)) > 0)
	{
		printf("read %d data\n", ret);
		get_rtcm_packet(data, ret);
	}

	fclose(fp);
	return 0;
}

