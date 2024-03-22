
#include <stdio.h>
#include <stddef.h>
#include <fenv.h>

int main(int argc, char *argv[])
{
	const int a = 0;
	int * const p = &a;
	volatile int b = 11;
	float f = 1.1;
	printf("%d\n", fegetround());
	printf("%f\n", 2.9e-1F);
	printf("a: %d, b: %d\n", a, b);
	printf("p: %d\n", *p);
	printf("max align: %ld\n", _Alignof(max_align_t));
	printf("int align: %ld\n", _Alignof(int));
	printf("float f: %d\n", (int)f);
	return 0;
}

