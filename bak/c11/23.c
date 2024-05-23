
#include <stdio.h>
#include <complex.h>

int main(int argc, char *argv[])
{
	float complex a;
	float b;
	float c;

	a = 1 + 3 * I;
	b = a;
	c = cimag(a);
//	printf("%d\n", imaginary(a));
	
	printf("%f\n", (b));
	printf("%f\n", (c));


	return 0;
}

