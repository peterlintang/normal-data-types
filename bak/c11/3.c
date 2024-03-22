
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	static int i = 2 || 1 / 0;

	printf("%d\n", i);
	return 0;
}
