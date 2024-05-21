
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

/*
int main(int argc, char *argv[])
{
	wchar_t wstr[] = {L'延', L'时', L'\0'};

	setlocale(LC_ALL, "en_US.UTF-8");
//	setlocale(LC_CTYPE, "en_US.UTF-8");
//	setlocale(LC_CTYPE, "C");
//	setlocale(LC_ALL, "zh_CN.UTF-8");
//	fprintf(stdout, "%s\n", setlocale(LC_ALL, NULL));
	fwprintf(stdout, L"%ls\n", wstr);
//	fwprintf(stdout, L"hello world\n");
//	fwprintf(stdout, L"ni hao\n");
	fflush(stdout);
	setlocale(LC_ALL, "en_NG");
	fflush(stdout);
	fprintf(stdout, "123456789\n");
	fflush(stdout);
	fprintf(stdout, "123456789\n");
	fflush(stdout);
	return 0;
}
*/

#include <wchar.h>

int main() 
{
	int ret = 0;
//    FILE *fp = fopen("test.txt", "w+"); // 打开文件用于写入
	setlocale(LC_ALL, "en_US.UTF-8");
	FILE *fp = fopen("test.txt", "w+, ccs=UTF-8");
    if (fp == NULL) {
        return -1; // 如果文件打开失败，返回-1
    }

    ret = fwprintf(stdout, L"你好，世界\n"); // 写入宽字符到文件
			fwprintf(fp, L"%d\n", ret);
	ret = fprintf(stdout, "12345678\n");
	perror("na");
	fprintf(stdout, "ret: %d\n", ret);
			fwprintf(fp, L"%d\n", ret);


    fclose(fp); // 关闭文件
    return 0;
}
