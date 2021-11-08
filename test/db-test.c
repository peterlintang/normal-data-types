
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "db.h"

int main(int argc, char *argv[])
{
	int ret = 0;
	DBHANDLE h = NULL;

	h = db_open("fruits", O_CREAT|O_TRUNC|O_RDWR, S_IRUSR | S_IWUSR);

	fprintf(stdout, "store %s %s\n", "apple", "100");
	db_store(h, "apple", "100", DB_INSERT);

	fprintf(stdout, "store %s %s\n", "orange", "10");
	db_store(h, "orange", "10", DB_INSERT);

	fprintf(stdout, "store %s %s\n", "banana", "500");
	db_store(h, "banana", "500", DB_INSERT);

	fprintf(stdout, "store %s %s\n", "grape", "200");
	db_store(h, "grape", "200", DB_STORE);

	fprintf(stdout, "fetch key: %s, dat: %s\n", "apple", db_fetch(h, "apple"));
	fprintf(stdout, "fetch key: %s, dat: %s\n", "banana", db_fetch(h, "banana"));
	fprintf(stdout, "fetch key: %s, dat: %s\n", "grape", db_fetch(h, "grape"));
	fprintf(stdout, "fetch key: %s, dat: %s\n", "banana", db_fetch(h, "banana"));
	fprintf(stdout, "fetch key: %s, dat: %s\n", "orange", db_fetch(h, "orange"));

	db_close(h);

	return ret;
}
