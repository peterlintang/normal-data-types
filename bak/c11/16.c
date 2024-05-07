
#include <stdio.h>

int main(int argc, char *argv[])
{
	char s[512] = { 0 };
	char *p = NULL;
	int num = 0;

	scanf("%m[a-z]%n", &p, &num);
	printf("\nget: %s\n", p);
	printf("\nnum: %d\n", num);

	scanf("%d", &num);
	scanf("%[a-z]%n", s, &num);
	printf("\nget: %s\n", s);
	printf("\nnum: %d\n", num);
	return 0;
}

