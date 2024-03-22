
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int a[10] = { [5] = 8, 9, 10, 11, 12, };

	for (int i = 0; i < 10; i++)
	{
		printf("i: %d, value: %d\n", i, a[i]);
	}
	return 0;
}

