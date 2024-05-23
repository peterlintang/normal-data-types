
/*
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>


int main(int argc, char *argv[])
{
	float _Imaginary a;
//	double imaginary a;

	
	a = 1.0;
	fprintf(stdout, "%f\n", a);
	
	return 0;
}
*/

#include <stdio.h>
#include <complex.h>
#include <stdbool.h>
//#pragma STDC CX_LIMITED_RANGE on
int main(void)
{
	_Bool isTrue = true;

	double imaginary i = 2.0*I; // pure imaginary	// bian yi bu guo
	double f = 1.0; // pure real
	double complex z = f + i; // complex number    
	printf("z = %.1f%+.1fi\n", creal(z), cimag(z));
}
