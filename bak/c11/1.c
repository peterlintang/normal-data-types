
#include <stdio.h>
#include <stdlib.h>

int eval(struct int_list *p)
{
	return 0;
}

int main(int argc, char *argv[])
{
	struct int_list { int car; struct int_list *cdr; };
	struct int_list endless_zero = {0, &endless_zero};

	return 0;
}

