
#include <stdio.h>

struct test {
	char a;
	int b;
};

struct test1 {
	char a;
	int b;
} __attribute__((packed));

struct test2 {
	int a;
} __attribute__((aligned(16)));

void __attribute__((constructor)) before(void)
{
	printf("before main routine\n");
}

void __attribute__((destructor)) after(void)
{
	printf("after main routine\n");
}

int main(int argc, char *argv[])
{
	struct test2 a;
	printf("main execute\n");
	printf("%lu %lu\n", sizeof(struct test), sizeof(struct test1));
	printf("&a: %lu\n", &a);
	return 0;
}
