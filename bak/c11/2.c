
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int n = 4;
	int m = 3;
	int a[n][m];
	int (*p)[m] = a;
	p += 1;

	(*p)[2] = 99;
	printf("%d\n", a[1][2]);
	n = p - a;
	return 0;
}

