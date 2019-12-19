
#include <stdio.h>
#include <stdlib.h>

#include "bit.h"

int main(int argc, char *argv[])
{
	int ret = 0;

	Bit_T set = NULL;
	set = MODULE_FUN_NAME(Bit, new)(36);
	if (NULL == set)
	{
		return 0;
	}
	MODULE_FUN_NAME(Bit, set)(set, 5, 10);
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));
	MODULE_FUN_NAME(Bit, put)(set, 20, 1);
	fprintf(stdout, "set bit: %d\n", MODULE_FUN_NAME(Bit, count)(set));

	return ret;
}
