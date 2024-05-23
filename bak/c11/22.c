
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <unistd.h>

_Thread_local int x = 0;
static void *(test_thread)(void *noused)
{
	pthread_t pid = 0;
	pthread_detach(pthread_self());

	for (int i = 0; i < 30; i++)
	{
		fprintf(stdout, "pthread: %d, i: %d, x: %d\n", (int)pthread_self(), i, x++);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t pid = 0;
	pthread_create(&pid, NULL, test_thread, NULL);
	sleep(5);
	pthread_create(&pid, NULL, test_thread, NULL);
	sleep(5);

	return 0;
}

