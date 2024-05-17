
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	unsigned char str[] = "?a???b,,,#c";
	unsigned char *t = NULL;

	printf("%s\n", str);

	t = strtok(str, "?");
	printf("%s\n", t);

	t = strtok(NULL, ",");
	printf("%s\n", t);

	t = strtok(NULL, "#,");
	printf("%s\n", t);

	t = strtok(NULL, "?");
	printf("%s\n", t == NULL ? "NULL": t);

	return 0;
}

