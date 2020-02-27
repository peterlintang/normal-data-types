/*
 *
 * handl file
 *
 */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

typedef struct __file_t{

	char 	*name;

	int 	eof;

	FILE	*fp;			/* file stream handler */

	uint32_t	size;		/* on init file size */
	uint32_t	read_bytes;	/* total read bytes */
	uint32_t 	write_bytes; 	/* total write bytes */

	uint32_t	nread;		/* number of last read count */

	char		*line;		/* buffer that holds getline results */
	size_t		line_len;	/* stores length of current line */

} file_t;

/* you can use this function in your program too */
int _handle_file_error(file_t *file, fpos_t *postion);

/* file interfaces */
file_t *_new_file(const char *name, const char *mode);

void 	_destroy_file(file_t **file);

int 	_read_file(file_t *file, void **buf, uint32_t size, uint16_t count);

int 	_write_file(file_t *file, void *buf, uint32_t size, uint16_t count);

int	_getpos_file(file_t *file, fpos_t *pos);

int	_setpos_file(file_t *file, fpos_t *pos);

int	_stat_file(file_t *file, struct stat *stat);

void	_revind_file(file_t *file);

void 	_truncate_file(file_t *file);

int	_isEOF_file(file_t *file);

char	*_getline_file(file_t *file);

/* not used now
 char	*_getline_file(file_t *file, int multiline);
 */

