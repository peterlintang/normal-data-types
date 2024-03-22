
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	_Alignas(8) int a = 0x12345678;
	printf("%x\n", a);
	return 0;
}

