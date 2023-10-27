
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char calculate_crc(unsigned char *str, int len)
{
	unsigned char crc = 0;

	for (int i = 0; i < len; i++)
	{
		crc ^= str[i];
	}

	return crc;
}

int main(int argc, char *argv[])
{
//	char str[] = "PAIR066,1,0,0,0,0,0";
//	char str[] = "PAIR680,0";
	char str[] = "PAIR730,0";
	int len = 0;
	unsigned char crc = 0;

	len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		crc ^= str[i];
	}

	printf("%02x\n", calculate_crc(str, strlen(str)));
	printf("%02x\n", crc);

	return 0;
}

