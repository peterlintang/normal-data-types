
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char str[] = "abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < strlen(str); i++)
	{
		printf("%c: %d %x\n", str[i], str[i], str[i]);
	}

	return 0;
}

