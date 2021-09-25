
#include <stdio.h>
#include <stdlib.h>

#include "thread-signal.h"


void *thread_func(void *arg)
{
	int		i = 0;
	pthread_t	mtid;

	if (!arg) {
		fprintf(stderr, "arguments error\n");
		return NULL;
	}

	fprintf(stdout, "<%s> thread: %ld running\n", 
			__func__, pthread_self());

	mtid = *(pthread_t *)arg;
	while (i++ < 31) {
		if (i != 9 && i != 19) {		// 9 and 19 is unblockable
			fprintf(stdout, "hello thread: %ld , %d, send signal: %d\n", 
				pthread_self(), i, i);
			pthread_kill(mtid, i);
			sleep(1);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int		signo;
	int		i   = 0;
	int		ret = 0;
	pthread_t	tid;
	pthread_t	mtid;
	sigset_t	newset;
	sigset_t	oldset;

	sigemptyset(&newset);	// needn't set empty
	sigemptyset(&oldset);

	ret = thread_block_signals(&newset, &oldset);
	if (ret) {
		fprintf(stderr, "cannot block signals\n");
		exit(1);
	}

	fprintf(stdout, "create thread...\n");
	mtid = pthread_self();
	pthread_create(&tid, NULL, thread_func, &mtid);	// child pthread will inherit the signal mask from parent pthread

	while (i++ < 40) {
		sigwait(&newset, &signo);
		fprintf(stdout, "%d times, get signal: %d\n", 
				i, signo);
	}

	return ret;
}
