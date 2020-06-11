
#include <stdio.h>
#include <stdlib.h>

long int queeze(long int n)
{
	if (n == 0) return 1;
	
	return n * queeze(n - 1);
}

int main(int argc, char *argv[])
{
	long int ret = 0;
	long int num = 0;


	num = atoi(argv[1]);
	ret = queeze(num);
	fprintf(stdout, " %ld! = %ld\n", num, ret);

	return 0;
}
