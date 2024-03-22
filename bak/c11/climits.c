
#include <stdio.h>
#include <limits.h>
#include <wchar.h>
#include <stddef.h>

int main(int argc, char *argv[])
//<%
{
	float f = 2P-1;
	int \uaaaa = 0x1234;
	char a[] = "12345678\234";
	wchar_t b[] = L"12";
	unsigned int c[] = U"12345678\234";
	unsigned char d[] = u8"1234567890";
	printf("char_bit: %d\n", CHAR_BIT);
	printf("schar_min: %d\n", SCHAR_MIN);
	printf("schar_max: %d\n", SCHAR_MAX);
	printf("uchar_max: %u\n", UCHAR_MAX);
	printf("char_min: %d\n", CHAR_MIN);
	printf("mb_len_max: %d\n", MB_LEN_MAX);
	printf("shrt_min: %d\n", SHRT_MIN);
	printf("shrt_max: %d\n", SHRT_MAX);
	printf("ushrt_max: %u\n", USHRT_MAX);
	printf("int_min: %d\n", INT_MIN);
	/*
	printf("%x\n", \uaaaa);
	printf("%Lf\n", 0.11l);
	printf("11: %d\n", strlen("abc"));
	printf("22: %d\n", strlen(L"abc"));
	printf("%d\n", c);

	for (int i = 0; b[i] != '\0'; i++)
	{
		printf("%c\n", b[i]);
	}
	printf("22: %d\n\?", wcslen(b));
	printf("%d\n", '\xff');
	printf("%x\n", '\x12');
	printf("%s\n", d);
	*/
	printf("f: %f\n", f);

	return 0;
//}
%>


