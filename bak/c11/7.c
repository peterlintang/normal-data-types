
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int a = 0;
	_Static_assert(0, "hello error\n");
	return 0;
}

