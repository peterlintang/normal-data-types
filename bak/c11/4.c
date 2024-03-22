
#include <stdio.h>
#include <stdlib.h>

_Noreturn void hello(void)
{
	printf("%s", "hello world\n");
}

int main(int argc, char *argv[])
{
	hello();
	return 0;
}
