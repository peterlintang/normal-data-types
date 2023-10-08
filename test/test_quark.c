
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "quark.h"

static int quark_cb(void *arg)
{
	printf("%s hello world\n", __func__);
	usleep(1000 * 100);
	return 1;
}

int main(int argc, char *argv[])
{
	quark_t *quark = NULL;
	quark = _new_quark(quark_cb, NULL, 1024);
	_spin_quark(quark);
	sleep(1);
	_stop_quark(quark, 1000 * 3);
	_destroy_quark(&quark);
	return 0;
}


