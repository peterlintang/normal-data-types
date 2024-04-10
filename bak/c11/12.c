
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	int x = 0;
	double fra = 0.0;
	fprintf(stdout, "%.12f\n", exp(1.0));
	fprintf(stdout, "%.12f\n", exp2(1.0));
	fra = frexp(exp2(2.0), &x);
	fprintf(stdout, "%.12f %d\n", fra, x);
	fprintf(stdout, "%d\n", ilogb(5.5));
	fprintf(stdout, "%f\n", log2(4));
	return 0;
}

