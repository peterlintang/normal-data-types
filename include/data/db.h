
#ifndef APUE_DB_H
#define APUE_DB_H

typedef void *DBHANDLE;

#define DB_INSERT	1	// insert new record
#define DB_REPLACE	2	// replace existing record
#define DB_STORE	3	// replace or insert

#define IDXLEN_MIN	6	/* key,sep,start,sep,length,\n */
#define IDXLEN_MAX	1024
#define DATLEN_MIN	2	/*data byte, newline */
#define DATLEN_MAX	1024

DBHANDLE 	db_open(const char *pathname, int oflag, ... /* mode */);
void db_close(DBHANDLE db);
/*
 * store a record
 * return 0: success
 * 		 -1: replace error
 * 		 1: insert error
 */
int db_store(DBHANDLE db, const char *key, const char *data, int flag);	// flag: DB_INSERT DB_REPLACE DB_STORE

/*
 * get a record
 */
char *db_fetch(DBHANDLE db, const char *key);

/*
 * delete a record
 * return 0: success
 * 		-1 : error
 */
int db_delete(DBHANDLE db, const char *key);

/*
 * return the start position of db
 */
void db_rewind(DBHANDLE db);

/*
 * get next record
 * if key != null, store the key in @key
 */
char *db_nextrec(DBHANDLE db, char *key);

#endif // APUE_DB_H
