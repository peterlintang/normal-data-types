
#include <stdio.h>
#include <stdlib.h>

#define hash_hash # ## #
#define mkstr(a) # a
#define in_between(a) mkstr(a)
#define join(c, d) in_between(c hash_hash d)

int main(int argc, char *argv[])
{
	char p[] = join(x, y);
	printf("%s\n", p);
	printf("val: %d\n", __STDC_NO_VLA__);
	printf("threads: %d\n", __STDC_NO_THREADS__);
	_Pragma("listing on \"... \\listing.dir\"");
	return 0;
}
