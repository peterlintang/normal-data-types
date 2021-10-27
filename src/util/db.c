
#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

#define IDXLEN_SZ	4
#define SEP			':'
#define SPACE		' '
#define NEWLINE		'\n'

#define PTR_SZ		6
#define PTR_MAX		999999
#define NHASH_DEF	137
#define FREE_OFF	0
#define HASH_OFF	PTR_SZ

#define un_lock(x, y, z, h)		1
#define writew_lock(x, y, z, h)	1
#define readw_lock(x, y, z, h)	1

typedef unsigned long DBHASH;	/* hash value */
typedef unsigned long COUNT;	/* unsigned count */

typedef struct {
	int idxfd;
	int datfd;
	char *idxbuf;
	char *datbuf;
	char *name;
	off_t idxoff;

	size_t idxlen;

	off_t datoff;
	size_t datlen;

	off_t ptrval;
	off_t ptroff;
	off_t chainoff;
	off_t hashoff;
	DBHASH nhash;
	COUNT cnt_delok;
	COUNT cnt_delerr;
	COUNT cnt_fetchok;
	COUNT cnt_fetcherr;
	COUNT cnt_nextrec;
	COUNT cnt_stor1;
	COUNT cnt_stor2;
	COUNT cnt_stor3;
	COUNT cnt_stor4;
	COUNT cnt_storerr;
} DB;


static DB	*_db_alloc(int );
static void _db_dodelete(DB *);
static int _db_find_and_lock(DB *, const char *, int );
static int _db_findfree(DB *, int , int );
static void _db_free(DB *);
static DBHASH _db_hash(DB *, const char *);
static char *_db_readdat(DB *);
static off_t _db_readidx(DB *, off_t );
static off_t _db_readptr(DB *, off_t );
static void _db_writedat(DB *, const char *, off_t , int );
static void _db_writeidx(DB *, const char *, off_t, int , off_t );
static void _db_writeptr(DB *, off_t , off_t );


DBHANDLE 	db_open(const char *pathname, int oflag, ... /* mode */)
{
	DB	*db = NULL;
	int len = 0;
	int mode = 0;
	size_t i;
	char asciiptr[PTR_SZ + 1];
	char hash[(NHASH_DEF + 1) * PTR_SZ + 2];
	struct stat statbuf;

	len = strlen(pathname);
	if ((db = _db_alloc(len)) == NULL)
	{
		return NULL;
	}

	db->nhash = NHASH_DEF;
	db->hashoff = HASH_OFF;
	strcpy(db->name, pathname);
	strcat(db->name, ".idx");

	if (oflag & O_CREAT)
	{
		va_list ap;

		va_start(ap, oflag);
		mode = va_arg(ap, int);
		va_end(ap);

		db->idxfd = open(db->name, oflag, mode);
		strcpy(db->name + len, ".dat");
		db->datfd = open(db->name, oflag, mode);
	}
	else
	{
		db->idxfd = open(db->name, oflag);
		strcpy(db->name + len, ".dat");
		db->datfd = open(db->name, oflag);
	}

	if (db->idxfd < 0 || db->datfd < 0)
	{
		_db_free(db);
		return NULL;
	}

	if ((oflag & (O_CREAT | O_TRUNC)) == (O_CREAT | O_TRUNC))
	{
		if (writew_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
			return NULL;
		if (fstat(db->idxfd, &statbuf) < 0)
			return NULL;
		if (statbuf.st_size == 0)
		{
			sprintf(asciiptr, "%*d", PTR_SZ, 0);
			hash[0] = 0;
			for (i = 0; i < NHASH_DEF + 1; i++)
				strcat(hash, asciiptr);
			strcat(hash, "\n");
			i = strlen(hash);
			if (write(db->idxfd, hash, i) != i)
				return NULL;
		}
		if (un_lock(db->idxfd, 0, SEEK_SET, 0) < 0)
			return NULL;
	}

	db_rewind(db);
	return db;
}

void db_close(DBHANDLE h)
{
	_db_free((DB *)h);
}

/*
 * store a record
 * return 0: success
 * 		 -1: replace error
 * 		 1: insert error
 */
int db_store(DBHANDLE h, const char *key, const char *data, int flag)	// flag: DB_INSERT DB_REPLACE DB_STORE
{
	DB	*db = NULL;
	int	rc = 0;
	int keylen = 0;
	int datlen = 0;
	off_t ptrval;

	db = (DB *)h;

	if (flag != DB_INSERT && flag != DB_REPLACE && flag != DB_STORE)
	{
		errno = EINVAL;
		return -1;
	}

	keylen = strlen(key);
	datlen = strlen(data) + 1;
	if (datlen < DATLEN_MIN || datlen > DATLEN_MAX)
		return -1;

	if (_db_find_and_lock(db, key, 1) < 0)
	{
		if (flag == DB_REPLACE)
		{
			rc = -1;
			db->cnt_storerr++;
			errno = ENOENT;
			goto doreturn;
		}

		ptrval = _db_readptr(db, db->chainoff);

		if (_db_findfree(db, keylen, datlen) < 0)
		{
			_db_writedat(db, data, 0, SEEK_END);
			_db_writeidx(db, key, 0, SEEK_END, ptrval);
			_db_writeptr(db, db->chainoff, db->idxoff);
			db->cnt_stor1++;
		}
		else
		{
			_db_writedat(db, data, db->datoff, SEEK_SET);
			_db_writeidx(db, key, db->idxoff, SEEK_SET, ptrval);
			_db_writeptr(db, db->chainoff, db->idxoff);
			db->cnt_stor2++;
		}
	}
	else
	{
		if (flag == DB_INSERT)
		{
			rc = 1;
			db->cnt_storerr++;
			goto doreturn;
		}

		if (datlen != db->datlen)
		{
			_db_dodelete(db);

			ptrval = _db_readptr(db, db->chainoff);

			_db_writedat(db, data, 0, SEEK_END);
			_db_writeidx(db, key, 0, SEEK_END, ptrval);

			_db_writeptr(db, db->chainoff, db->idxoff);
			db->cnt_stor3++;
		}
		else
		{
			_db_writedat(db, data, db->datoff, SEEK_SET);
			db->cnt_stor4++;
		}
	}

	rc = 0;

doreturn:
	if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
		return -1;

	return rc;
}

/*
 * get a record
 */
char *db_fetch(DBHANDLE h, const char *key)
{
	DB	*db = NULL;
	char *ptr = NULL;

	db = (DB *)h;

	if (_db_find_and_lock(db, key, 0) < 0)
	{
		ptr = NULL;
		db->cnt_fetcherr++;
	}
	else
	{
		ptr = _db_readdat(db);
		db->cnt_fetchok++;
	}

	if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
		return NULL;

	return ptr;
}

/*
 * delete a record
 * return 0: success
 * 		-1 : error
 */
int db_delete(DBHANDLE h, const char *key)
{
	DB	*db = NULL;
	int	rc = 0;

	db = (DB *)h;

	if (_db_find_and_lock(db, key, 1) == 0)
	{
		_db_dodelete(db);
		db->cnt_delok++;
	}
	else
	{
		rc = -1;
		db->cnt_delerr++;
	}

	if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
		return -1;

	return rc;
}

/*
 */
void db_rewind(DBHANDLE h)
{
	DB	*db = NULL;
	off_t	offset;

	db = (DB *)h;

	offset = (db->nhash + 1) * PTR_SZ;

	if ((db->idxoff = lseek(db->idxfd, offset + 1, SEEK_SET)) == -1)
		return ;
}

/*
 * get next record
 * if key != null, store the key in @key
 */
char *db_nextrec(DBHANDLE h, char *key)
{
	DB *db = NULL;
	char	c;
	char	*ptr = NULL;

	db = (DB *)h;

	if (readw_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		return NULL;

	do {
		if (_db_readidx(db, 0) < 0)
		{
			ptr = NULL;
			goto doreturn;
		}

		ptr = db->idxbuf;
		while ((c = *ptr++) != 0 && c == SPACE)
			;
	} while (c == 0);

	if (key != NULL)
		strcpy(key, db->idxbuf);

	ptr = _db_readdat(db);
	db->cnt_nextrec++;

doreturn:
	if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		return NULL;

	return ptr;
}




static DB *_db_alloc(int namelen)
{
	DB	*db = NULL;

	if ((db = calloc(1, sizeof(DB))) == NULL)
		return NULL;
	db->idxfd = -1;
	db->datfd = -1;

	if ((db->name = malloc(namelen + 5)) == NULL)
		return NULL;

	if ((db->idxbuf = malloc(IDXLEN_MAX + 2)) == NULL)
		return NULL;

	if ((db->datbuf = malloc(DATLEN_MAX + 2)) == NULL)
		return NULL;

	return db;
}

static void _db_free(DB *db)
{
	if (db->idxfd >= 0)
		close(db->idxfd);
	if (db->datfd >= 0)
		close(db->datfd);
	if (db->idxbuf != NULL)
		free(db->idxbuf);
	if (db->datbuf != NULL)
		free(db->datbuf);
	if (db->name != NULL)
		free(db->name);
	free(db);
}

static int _db_find_and_lock(DB *db, const char *key, int writelock)
{
	off_t	offset;
	off_t	nextoffset;

	db->chainoff = (_db_hash(db, key) * PTR_SZ) + db->hashoff;
	db->ptroff = db->chainoff;

	if (writelock)
	{
		if (writew_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
			return -1;
	}
	else
	{
		if (readw_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
			return -1;
	}

	offset = _db_readptr(db, db->ptroff);
//	fprintf(stdout, "%s: %s, offset: %ld, ptroff: %ld\n",
//				   	__func__, key, offset, db->ptroff);
	while (offset != 0)
	{
		nextoffset = _db_readidx(db, offset);
		if (strcmp(db->idxbuf, key) == 0)
			break;
		db->ptroff = offset;
		offset = nextoffset;
	}

	return (offset == 0 ? -1 : 0);
}


static DBHASH _db_hash(DB *db, const char *key)
{
	DBHASH		hval = 0;
	char		c = 0;
	int 		i;

	for (i = 1; (c = *key++) != 0; i++)
		hval += c * i;

	return (hval % db->nhash);
}

static off_t _db_readptr(DB *db, off_t offset)
{
	char asciiptr[PTR_SZ + 1];

	if (lseek(db->idxfd, offset, SEEK_SET) == -1)
	{
		perror("lseek");
		return -1;
	}
	if (read(db->idxfd, asciiptr, PTR_SZ) != PTR_SZ)
		return -1;

	asciiptr[PTR_SZ] = 0;
//	fprintf(stdout, "%s offset: %ld, asciiptr: %s\n", __func__, offset, asciiptr);

	return atol(asciiptr);
}

static off_t _db_readidx(DB *db, off_t offset)
{
	ssize_t		i;
	char		*ptr1 = NULL;
	char		*ptr2 = NULL;
	char		asciiptr[PTR_SZ + 1];
	char		asciilen[IDXLEN_SZ + 1];
	struct iovec iov[2];

	if ((db->idxoff = lseek(db->idxfd, offset, 
				offset == 0 ? SEEK_CUR : SEEK_SET)) == -1)
	{
//		perror("_db_readidx");
		return -1;
	}

	iov[0].iov_base = asciiptr;
	iov[0].iov_len = PTR_SZ;
	iov[1].iov_base = asciilen;
	iov[1].iov_len = IDXLEN_SZ;

	if ((i = readv(db->idxfd, &iov[0], 2)) != PTR_SZ + IDXLEN_SZ)
	{
		if (i == 0 && offset == 0)
			return -1;
		return -1;
	}

	asciiptr[PTR_SZ] = 0;
	db->ptrval = atol(asciiptr);

	asciilen[IDXLEN_SZ] = 0;
	if ((db->idxlen = atoi(asciilen)) < IDXLEN_MIN 
				|| db->idxlen > IDXLEN_MAX)
		return -1;

	if ((i = read(db->idxfd, db->idxbuf, db->idxlen)) != db->idxlen)
		return -1;
	if (db->idxbuf[db->idxlen - 1] != NEWLINE)
		return -1;
	db->idxbuf[db->idxlen - 1] = 0;

	if ((ptr1 = strchr(db->idxbuf, SEP)) == NULL)
		return -1;
	*ptr1++ = 0;

	if ((ptr2 = strchr(ptr1, SEP)) == NULL)
		return -1;
	*ptr2++ = 0;

	if (strchr(ptr2, SEP) != NULL)
		return -1;

	if ((db->datoff = atol(ptr1)) < 0)
		return -1;
	if ((db->datlen = atol(ptr2)) <= 0
			|| db->datlen > DATLEN_MAX)
		return -1;
//	fprintf(stdout, "%s idxbuf: %s, datoff: %ld, datlen: %ld, ptrval: %ld, idxlen: %ld, offset: %ld\n", 
//					__func__, db->idxbuf, db->datoff, db->datlen, db->ptrval, db->idxlen, offset);

	return db->ptrval;
}

static char *_db_readdat(DB *db)
{
	if (lseek(db->datfd, db->datoff, SEEK_SET) == -1)
		return NULL;
	if (read(db->datfd, db->datbuf, db->datlen) != db->datlen)
		return NULL;

	if (db->datbuf[db->datlen - 1] != NEWLINE)
		return NULL;

	db->datbuf[db->datlen - 1] = 0;
//	fprintf(stdout, "%s offset: %ld, data: %s\n", __func__, db->datoff, db->datbuf);

	return db->datbuf;
}

static void _db_dodelete(DB *db)
{
	int i;
	char *ptr = NULL;
	off_t freeptr;
	off_t saveptr;

	for (ptr = db->datbuf, i = 0; i < db->datlen - 1; i++)
	{
		*ptr ++ = SPACE;
	}

	*ptr = 0;
	ptr = db->idxbuf;
	while (*ptr)
		*ptr++ = SPACE;

	if (writew_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		return ;

	_db_writedat(db, db->datbuf, db->datoff, SEEK_SET);

	freeptr = _db_readptr(db, FREE_OFF);

	saveptr = db->ptrval;

	_db_writeidx(db, db->idxbuf, db->idxoff, SEEK_SET, freeptr);

	_db_writeptr(db, FREE_OFF, db->idxoff);

	_db_writeptr(db, db->ptroff, saveptr);
	if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		return ;
}

static void _db_writedat(DB *db, const char *data, off_t offset, int whence)
{
	struct iovec iov[2];
	static char newline = NEWLINE;

	if (whence == SEEK_END)
		if (writew_lock(db->datfd, 0, SEEK_SET, 0) < 0)
			return ;

	if ((db->datoff = lseek(db->datfd, offset, whence)) == -1)
		return ;
	db->datlen = strlen(data) + 1;

	iov[0].iov_base = (char *)data;
	iov[0].iov_len = db->datlen - 1;
	iov[1].iov_base = &newline;
	iov[1].iov_len = 1;
	if (writev(db->datfd, &iov[0], 2) != db->datlen)
		return ;

//	fprintf(stdout, "%s data: %s, offset: %ld\n", __func__, data, offset);
	if (whence == SEEK_END)
		if (un_lock(db->datfd, 0, SEEK_SET, 0) < 0)
			return ;
}

static void _db_writeidx(DB *db, const char *key,
			off_t offset, int whence, off_t ptrval)
{
	struct iovec	iov[2];
	char	asciiptrlen[PTR_SZ + IDXLEN_SZ + 1];
	int		len = 0;
	char	*fmt = NULL;

//	fprintf(stdout, "%s key: %s, whence: %d, offset: %ld, ptrval: %ld\n", __func__, key, whence, offset, ptrval);
	if ((db->ptrval = ptrval) < 0 || ptrval > PTR_MAX)
		return ;

	if (sizeof(off_t) == sizeof(long long))
		fmt = "%s%c%lld%c%d\n";
	else
		fmt = "%s%c%ld%c%d\n";

	sprintf(db->idxbuf, fmt, key, SEP, db->datoff, SEP, db->datlen);
//	fprintf(stdout, "%s key: %s, whence: %d, offset: %ld, ptrval: %ld 4 : %s min: %d, max: %d, len: %ld\n",
//				   	__func__, key, whence, offset, ptrval, db->idxbuf,
//					IDXLEN_MIN,  IDXLEN_MAX,
//					strlen(db->idxbuf));
	if ((len = strlen(db->idxbuf)) < IDXLEN_MIN || len > IDXLEN_MAX)
		return ;
	sprintf(asciiptrlen, "%*ld%*d", PTR_SZ, ptrval, IDXLEN_SZ, len);

//	fprintf(stdout, "%s key: %s, whence: %d, offset: %ld, ptrval: %ld 1\n", __func__, key, whence, offset, ptrval);
	if (whence == SEEK_END)
		if (writew_lock(db->idxfd, ((db->nhash + 1) * PTR_SZ) + 1,
						SEEK_SET, 0) < 0)
			return ;

//	fprintf(stdout, "%s key: %s, whence: %d, offset: %ld, ptrval: %ld 2\n", __func__, key, whence, offset, ptrval);
	if ((db->idxoff = lseek(db->idxfd, offset, whence)) == -1)
		return ;

//	fprintf(stdout, "%s key: %s, whence: %d, offset: %ld, ptrval: %ld 3\n", __func__, key, whence, offset, ptrval);
	iov[0].iov_base = asciiptrlen;
	iov[0].iov_len = PTR_SZ + IDXLEN_SZ;
	iov[1].iov_base = db->idxbuf;
	iov[1].iov_len = len;
	if (writev(db->idxfd, &iov[0], 2) != PTR_SZ + IDXLEN_SZ + len)
		return ;

//	fprintf(stdout, "%s key: %s, whence: %d, offset: %ld, ptrval: %ld\n", __func__, key, whence, offset, ptrval);
	if (whence == SEEK_END)
		if (un_lock(db->idxfd, ((db->nhash + 1) * PTR_SZ) + 1,
					SEEK_SET, 0) < 0)
			return ;
}

static void _db_writeptr(DB *db, off_t offset, off_t ptrval)
{
	char	asciiptr[PTR_SZ + 1];

	if (ptrval < 0 || ptrval > PTR_MAX)
		return ;

	sprintf(asciiptr, "%*ld", PTR_SZ, ptrval);

	if (lseek(db->idxfd, offset, SEEK_SET) == -1)
		return ;
	if (write(db->idxfd, asciiptr, PTR_SZ) != PTR_SZ)
		return ;
//	fprintf(stdout, "%s offset: %ld, ptrval: %ld\n", __func__, offset, ptrval);
}

static int _db_findfree(DB *db, int keylen, int datlen)
{
	int 	rc = 0;
	off_t	offset;
	off_t	nextoffset;
	off_t	saveoffset;

	if (writew_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		return -1;

	saveoffset = FREE_OFF;
	offset = _db_readptr(db, saveoffset);
	while (offset != 0)
	{
		nextoffset = _db_readidx(db, offset);
		if (strlen(db->idxbuf) == keylen && db->datlen == datlen)
			break;
		saveoffset = offset;
		offset = nextoffset;
	}

	if (offset == 0)
	{
		rc = -1;
	}
	else
	{
		_db_writeptr(db, saveoffset, db->ptrval);
		rc = 0;
	}

	if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		return -1;

	return rc;
}










