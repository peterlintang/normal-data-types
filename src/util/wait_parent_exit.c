
#include <stdio.h>
#include <stdlib.h>

void wait_parent_exit(void)
{
	while (getppid() != 1)
		sleep(1);
}
