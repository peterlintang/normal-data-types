
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "share-mem.h"

#define SHARE_MEM_PATH	"./hello"
#define SHARE_MEM_ID	0
#define SHARE_MEM_SIZE	1024

int main(int argc, char *argv[])
{
	int ret = 0;
	int flags = SHM_R | SHM_W;
	char *ptr = NULL;
	char tmp = '2';

	ptr = (char *)MODULE_FUN_NAME(SharedMem, open)(SHARE_MEM_PATH, SHARE_MEM_ID, NULL, flags);
	if (ptr == NULL)
	{
		fprintf(stdout, "failed to open share mem\n");
		return 0;
	}

	for (int i = 0; i < SHARE_MEM_SIZE; i++)
	{
		MODULE_FUN_NAME(SharedMem, read)(ptr + i, &tmp, 1);
		fprintf(stdout, "%c\n", tmp);
	}


	MODULE_FUN_NAME(SharedMem, close)(ptr);

	return 0;
}

