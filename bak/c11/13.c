
#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
	printf("%ld (%lx)\n", INTMAX_MIN, INTMAX_MIN);
	printf("%ld %ld (%lx %lx)\n", PTRDIFF_MIN, PTRDIFF_MAX, PTRDIFF_MIN, PTRDIFF_MAX);
	printf("%lx\n", INT64_C(0x123));
	return 0;
}
