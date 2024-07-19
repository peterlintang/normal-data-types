
#include <stdio.h>
#include <stdlib.h>

#include <uapi/asm/cachectl.h>
#include <syscall.h>

int main(int argc, char *argv[])
{
	char *addr = 0x0;
	int nbytes = 1024;
	int ret = 0;

	addr = strtol(argv[1], NULL, 10);
	nbytes = strtol(argv[2], NULL, 10);

	ret = cacheflush(addr, nbytes, DCACHE);
	fprintf(stdout, "ret: %d\n", ret);
	return 0;
}

