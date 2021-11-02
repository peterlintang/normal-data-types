
#include <stdio.h>
#include <stdlib.h>

#include "share-mem.h"

#define SHARE_MEM_PATH	"./hello"
#define SHARE_MEM_ID	0
#define SHARE_MEM_SIZE	1024

int main(int argc, char *argv[])
{
	int ret = 0;
	int flags = 0;
	char *ptr = NULL;

	ret = MODULE_FUN_NAME(SharedMem, create)(SHARE_MEM_PATH, SHARE_MEM_ID, SHARE_MEM_SIZE, flags);
	if (ret != 0)
	{
		fprintf(stdout, "create shared mem failed: ret: %d\n", ret);
		return 0;
	}

	ptr = (char *)MODULE_FUN_NAME(SharedMem, open)(SHARE_MEM_PATH, SHARE_MEM_ID, NULL, flags);
	if (ptr == NULL)
	{
		fprintf(stdout, "failed to open share mem\n");
		goto out;
	}

	for (int i = 0; i < SHARE_MEM_SIZE; i++)
	{
		MODULE_FUN_NAME(SharedMem, write)(ptr + i, '2', 1);
	}

	sleep(15);

out:
	MODULE_FUN_NAME(SharedMem, destroy)(SHARE_MEM_PATH, SHARE_MEM_ID);

	return 0;
}

