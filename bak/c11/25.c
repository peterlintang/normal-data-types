
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

int main(int argc, char *argv[])
{
	_Atomic int i = 8;

	atomic_store(&i, 12);
	fprintf(stdout, "%d\n", i);
	return 0;
}

