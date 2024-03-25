
#include <stdio.h>
#include <stdlib.h>

#define hash_hash # ## #
#define TO_STRING(T)	#T
#define NUM_B		9
#define IN_BETWEEN(a) TO_STRING(a)
#define COMBINE(X,T)	X##T

int main(int argc, char *argv[])
{
	printf("hello world: %s\n", TO_STRING(balidy));
	printf("hello world: %d\n", COMBINE(NUM_, B));
	printf("hello world: %s\n", COMBINE(TO_, STRING)(111));
	printf("hello world: %s\n", TO_STRING((111 hash_hash dd)));
	printf("hello world: %s\n", IN_BETWEEN((111 hash_hash dd)));
	printf("%%\n");
	return 0;
}

