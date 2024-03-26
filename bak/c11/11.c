
#include <stdio.h>
#include <inttypes.h>
#include <wchar.h>

int main(void)
{
	uintmax_t i = UINTMAX_MAX;
	wprintf(L"The largest integer value is %020" PRIxMAX "\n", i);
	return 0;
}


